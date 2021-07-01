#include <functional>
#include <netinet/in.h>
#include <fcntl.h>

#include "TcpConnection.h"
#include "ClientProxyProtocol.h"
#include "EventLoop.h"
#include "CircularBuffer.h"
#include "common.h"
#include "log.h"

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
                            : writeBuffer_(bufferSize_), readBuffer_(bufferSize_) 
{
    eventsSource_ = std::make_shared<EventsSource>(socket(AF_INET, SOCK_STREAM, 0), loop,
                                            std::bind(&TcpConnection::OnInput, this), 
                                            std::bind(&TcpConnection::OnOutput, this), 
                                            std::bind(&TcpConnection::OnError, this));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr_.sin_addr);
}

TcpConnection::TcpConnection(FileDesc connfd, 
                            const struct sockaddr_in &addr, 
                            EventLoop *loop)
                            : writeBuffer_(bufferSize_), 
                            readBuffer_(bufferSize_), 
                            addr_(addr)
{
    eventsSource_ = std::make_shared<EventsSource>(connfd, loop,
                                            std::bind(&TcpConnection::OnInput, this), 
                                            std::bind(&TcpConnection::OnOutput, this), 
                                            std::bind(&TcpConnection::OnError, this));
    setNonBlocking(connfd);
    logger_ptr->info("In main thread: in creating tcp connection, now modify events to EV_IN");
    eventsSource_->Update(Net::EV_IN | Net::EV_ET | Net::EV_ERR);
    logger_ptr->info("In main thread: events: {} after modifying", (int)eventsSource_->events_);
}

TcpConnection::~TcpConnection() 
{
    close(eventsSource_->fd());
}

int TcpConnection::OnInput()
{
    FileDesc clientfd = eventsSource_->fd();
    int byteRead, pkgProcessed = 0;
    bool needToRead = true;
    std::vector<Request> requests;
    std::vector<Response> responses;
    while (needToRead)
    {
        if ((byteRead = read(clientfd, readBuffer_)) <= 0)
        {   logger_ptr->info("In main thread: connection (sockfd: {}) shutdown by peer.", SockFd());
            if (byteRead < 0 && hupCallBack_) hupCallBack_();
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

    logger_ptr->info("In main thread: connection (sockfd: {0}) receives {1} packages.", SockFd(), requests.size() + responses.size());
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
        && (writeBuffer_.size() + 2 * PACKAGE_HDR_LEN < writeBuffer_.capacity()))
    {
        outputCallBack_();
    }
    if (writeBuffer_.empty())
    {   
        logger_ptr->info("In main thread: connection (sockfd: {0}) has no data to send, unregister EV_IN event.", SockFd());
        eventsSource_->Update(Net::EV_IN | Net::EV_ET | Net::EV_ERR);
        return 0;
    }
    int ret = Net::write(eventsSource_->fd(), writeBuffer_);
    logger_ptr->info("In main thread: connection (sockfd: {0}) write {1} bytes.", SockFd(), ret);
    return ret;
}

int TcpConnection::OnError()
{
    logger_ptr->info("In main thread: connection (sockfd: {0}) has error!", SockFd());
    if (errorCallBack_)
        errorCallBack_();
    return -1;
}

int TcpConnection::Send(const std::string &pkgsData)
{
    put(writeBuffer_, pkgsData.c_str(), pkgsData.size());
    // try to write it immediately
    int bytesWritten = write(eventsSource_->fd(), writeBuffer_);
    if (bytesWritten < 0)
    {   logger_ptr->info("In main thread: connection (sockfd: {0}) send with fatal error!", SockFd());
        return -1;
    }
    // if there are data left
    if (!writeBuffer_.empty())
    {
        eventsSource_->Update(Net::EV_IN | Net::EV_OUT | Net::EV_ET | Net::EV_ERR);
    }

    logger_ptr->info("In main thread: connection (sockfd: {0}) send {1} bytes data.", SockFd(), bytesWritten);

    return 0;
}

int TcpConnection::Connect()
{
    int ret = connect(eventsSource_->fd(), (struct sockaddr*)&addr_, sizeof(addr_));
    if (ret != -1)
    {
        logger_ptr->info("In main thread: (sockfd: {0}) successfully connect to host", SockFd());
        eventsSource_->Update(Net::EV_IN | Net::EV_OUT | Net::EV_ET | Net::EV_ERR);
        return 0;
    }
    logger_ptr->info("In main thread: (sockfd: {0}) fail to connect to host", SockFd());
    return -1;
}

int TcpConnection::DisConnect()
{
    logger_ptr->info("In main thread: (sockfd: {0}) disconnect from host", SockFd());
    ::close(eventsSource_->fd());
    eventsSource_->Close(); 
    return 0;
}

int TcpConnection::SockFd() const {return eventsSource_->fd();}
int TcpConnection::GetWriteBufferRoom() const {return writeBuffer_.capacity() - writeBuffer_.size();}