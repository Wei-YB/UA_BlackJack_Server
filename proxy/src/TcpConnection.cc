#include <functional>
#include <netinet/in.h>
#include <fcntl.h>

#include "TcpConnection.h"
#include "ClientProxyProtocol.h"
#include "EventLoop.h"
#include "CircularBuffer.h"
#include "common.h"

#include "UA_BlackJack.pb.h"

using ua_blackjack::Request;
using ua_blackjack::Response;
using namespace Net;

static int setNonBlocking(int fd)
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

TcpConnection::TcpConnection(const char *ip, 
                            unsigned short port, 
                            EventLoop *loop)
                            : writeBuffer_(bufferSize_), readBuffer_(bufferSize_),
                            eventsSource_(socket(AF_INET, SOCK_STREAM, 0), loop,
                                            std::bind(&TcpConnection::OnInput, this), 
                                            std::bind(&TcpConnection::OnOutput, this), 
                                            std::bind(&TcpConnection::OnError, this)) 
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr_.sin_addr);
}

TcpConnection::TcpConnection(FileDesc connfd, 
                            const struct sockaddr_in &addr, 
                            EventLoop *loop)
                            : writeBuffer_(bufferSize_), readBuffer_(bufferSize_), 
                            addr_(addr), eventsSource_(connfd, loop,
                                        std::bind(&TcpConnection::OnInput, this), 
                                        std::bind(&TcpConnection::OnOutput, this), 
                                        std::bind(&TcpConnection::OnError, this)) 
{
    setNonBlocking(connfd);
    eventsSource_.Update(Net::EV_IN | Net::EV_ET | Net::EV_ERR);
}

TcpConnection::~TcpConnection() 
{
    close(eventsSource_.fd());
}

int TcpConnection::OnInput()
{
    // std::cout << "TcpConnection::OnInput." << std::endl;
    FileDesc clientfd = eventsSource_.fd();
    int byteRead, pkgProcessed = 0;
    bool needToRead = true;
    std::vector<Request> requests;
    std::vector<Response> responses;
    while (needToRead)
    {
        if ((byteRead = read(clientfd, readBuffer_)) <= 0)
        {   // std::cout << "TcpConnection::OnInput: connection shutdown by peer." << std::endl;
            return byteRead;
        }
        if (readBuffer_.size() != readBuffer_.capacity())
	    {   // the tcp recv buffer has been cleared
		    needToRead = false;
	    }
        
        while (readBuffer_.size() >= PACKAGE_HDR_LEN)
        {   // handle packages in buffer one by one
            int32_t msgType, msgLength; 
            
            Net::readAs(readBuffer_, 0, msgType, true);
            Net::readAs(readBuffer_, sizeof(msgType), msgLength, true);
            // std::cout << "msgType: " << (msgType == NS::REQUEST ? "Request" : "Response") << std::endl;
            // std::cout << "msgLength: " << msgLength << std::endl;
            if (readBuffer_.size() - PACKAGE_HDR_LEN < msgLength)
            {   // not a complete package, continue to next read
                break;
            }
            readBuffer_.free(PACKAGE_HDR_LEN);
            std::string rawMsg;
            circularBufferToString(readBuffer_, msgLength, rawMsg);
            readBuffer_.free(msgLength);
            if (msgType == NS::REQUEST)
            {
                Request request;
                request.ParseFromString(rawMsg);
                requests.push_back(request);
            }
            else if (msgType == NS::RESPONSE)
            {
                Response response;
                response.ParseFromString(rawMsg);
                responses.push_back(response);
            }
            pkgProcessed++;
        }
    }

    if (inputCallBack_)
    {
        inputCallBack_(requests, responses);
    }
    return pkgProcessed;;
}

int TcpConnection::OnOutput()
{
    // check whether we can get some data from upper layer
    if (outputCallBack_ 
        && writeBuffer_.size() + 2 * PACKAGE_HDR_LEN < writeBuffer_.capacity())
    {
        outputCallBack_();
    }
    if (writeBuffer_.empty())
    {   // std::cout << "TcpConnection::OnOutput: no data to send, unregister EV_IN event." << std::endl;
        eventsSource_.Update(Net::EV_IN | Net::EV_ET | Net::EV_ERR);
        return 0;
    }
    int ret = Net::write(eventsSource_.fd(), writeBuffer_);
    // std::cout << "TcpConnection::OnOutput: write " << (ret > 0 ? ret : 0) << " byte(s) to client." << std::endl;
    // std::cout << "                        " << writeBuffer_.size() << " bytes left in buffer." << std::endl;
    return ret;
}

int TcpConnection::OnError()
{
    if (errorCallBack_)
        errorCallBack_();
    return -1;
}

int TcpConnection::Send(const std::string &pkgsData)
{
    put(writeBuffer_, pkgsData.c_str(), pkgsData.size());
    // try to write it immediately
    int bytesWritten = write(eventsSource_.fd(), writeBuffer_);
    if (bytesWritten < 0 && errno != EWOULDBLOCK && errno != EAGAIN)
    {
        std::cout << "TcpConnection::Send: fail." << std::endl;
        return -1;
    }
    std::cout << "TcpConnection::Send: send." << bytesWritten << "bytes." << std::endl;
    // if there are data left
    if (!writeBuffer_.empty())
    {
        std::cout << "TcpConnection::Send: wait for the next round." << std::endl;
        eventsSource_.Update(Net::EV_IN | Net::EV_OUT | Net::EV_ET | Net::EV_ERR);
    }
    
    return 0;
}

int TcpConnection::Connect()
{
    int ret = connect(eventsSource_.fd(), (struct sockaddr*)&addr_, sizeof(addr_));
    if (ret != -1)
    {
        eventsSource_.Update(Net::EV_IN | Net::EV_OUT | Net::EV_ET | Net::EV_ERR);
        return 0;
    }
    return -1;
}

int TcpConnection::DisConnect()
{
    ::close(eventsSource_.fd());
    eventsSource_.Close(); 
    return 0;
}
