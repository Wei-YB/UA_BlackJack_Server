#include <functional>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "TcpConnection.h"
#include "ClientProxyProtocol.h"
#include "EventsSource.h"
#include "CircularBuffer.h"
#include "common.h"
#include "log.h"

using namespace Net;

static int setNonBlocking(int fd)
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

TcpConnection::TcpConnection(const char *ip,
                             unsigned short port,
                             EventLoop *loop)
                            : writeBuffer_(bufferSize_), 
                            readBuffer_(bufferSize_)
{
    eventsSource_ = std::make_shared<EventsSource>(socket(AF_INET, SOCK_STREAM, 0), loop,
                                                   std::bind(&TcpConnection::OnInput, this),
                                                   std::bind(&TcpConnection::OnOutput, this),
                                                   std::bind(&TcpConnection::OnError, this),
                                                   std::bind(&TcpConnection::OnDisconnect, this));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr_.sin_addr);
    eventLoop_ = loop;
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
                                                   std::bind(&TcpConnection::OnError, this),
                                                   std::bind(&TcpConnection::OnDisconnect, this));
    setNonBlocking(connfd);
    eventsSource_->EnableRead();
    // eventsSource_->EnableWrite();
    eventsSource_->EnableET();
    eventLoop_ = loop;
    valid_ = true;
}

TcpConnection::~TcpConnection()
{
    valid_ = false;
    ::close(eventsSource_->fd());
    eventsSource_->RemoveFromLoop();
}

// this callback decode the user-defined packages and return
// raw protobuf messages without deserializing them
int TcpConnection::OnInput()
{
    FileDesc clientfd = eventsSource_->fd();
    int byteRead, pkgProcessed = 0, offset = 0;
    bool needToRead = true;
    std::vector<std::pair<int32_t, StringPiece>> pkgs;
    while (needToRead)
    {
        if ((byteRead = read(clientfd, readBuffer_)) < 0)
        {
            valid_ = false;
            logger_ptr->trace("In TcpConnection::OnInput(): connection (sockfd: {}) shutdown by peer.", SockFd());
            if (hupCallBack_) 
            {
                ::close(SockFd());
                hupCallBack_();
            }
            return byteRead;
        }
        if (readBuffer_.size() != readBuffer_.capacity())
        {   // the tcp recv buffer has been cleared
            needToRead = false;
        }
        // if specify a decoder, decode it
        if (decoder_)
        {   // logger_ptr->info("In main thread: using decoder");
            // handle packages in buffer one by one
            while (true)
            {
                int32_t pkgType;
                StringPiece strPiece = decoder_(readBuffer_, &offset, &pkgType);
                //std::string pkg(std::move(decoder_(readBuffer_, &pkgType)));
                //if (pkg.size() == 0)
                if (strPiece.length() == 0)
                {
                    break;
                }
                // readBuffer_.free(pkg.size());
                pkgProcessed++;
                //pkgs.emplace_back(std::move(pkgType), std::move(pkg));
                pkgs.push_back(std::make_pair(pkgType, strPiece));
            }   
        }
        else
        {
            // logger_ptr->info("no decoder available.");
            StringPiece strPiece(&readBuffer_, (readBuffer_.m_head + offset) % readBuffer_.capacity(), readBuffer_.size() - offset);
            pkgs.push_back(std::make_pair(0, strPiece));
            offset += readBuffer_.size();
            // pkgs.emplace_back(0, std::move(circularBufferToString(readBuffer_, readBuffer_.size())));
            //readBuffer_.free(readBuffer_.size());
            continue;
        }
    }

    logger_ptr->trace("In TcpConnection::OnInput(): connection (sockfd: {0}) receives {1} packages.", SockFd(), pkgs.size());
    if (inputCallBack_)
    {
        inputCallBack_(std::move(pkgs));
    }

    return pkgProcessed;
}

int TcpConnection::OnOutput()
{
    if (!valid_)
        return -1;
    // check whether we can get some data from upper layer
    if (outputCallBack_ && writeBuffer_.size() < writeBuffer_.capacity())
    {
        outputCallBack_();
    }
    if (writeBuffer_.empty())
    {
        logger_ptr->trace("In TcpConnection::OnOutput(): connection (sockfd: {0}) has no data to send, unregister EV_OUT event.", SockFd());
        eventsSource_->DisableWrite();
        return 0;
    }
    int ret = write(eventsSource_->fd(), writeBuffer_);
    logger_ptr->trace("In TcpConnection::OnOutput(): connection (sockfd: {0}) write {1} bytes.", SockFd(), ret);
    return 0;
}

int TcpConnection::OnError()
{
    valid_ = false;
    logger_ptr->warn("In TcpConnection::OnError(): connection (sockfd: {0}) has error!", SockFd());
    if (errorCallBack_)
        errorCallBack_();
    ::close(SockFd());
    return -1;
}

int TcpConnection::OnDisconnect()
{
    valid_ = false;
    logger_ptr->info("In TcpConnection::OnDisconnect(): connection (sockfd: {0}) shutdown by peer!", SockFd());
    if (hupCallBack_)
        hupCallBack_();
    close(SockFd());
    return -1;
}

int TcpConnection::Send(int32_t type, const std::string &pkgsData)
{
    if (!valid_)
    {
        logger_ptr->error("In TcpConnection::Send: try to send data in a invalid socket {}.", SockFd());
        return -2;
    }
    if (encoder_)
    {
        // if (encoder_(type, pkgsData, writeBuffer_) < 0)
        //     return -1;
    }
    else
    {
        if (writeBuffer_.capacity() - writeBuffer_.size() < pkgsData.size())
            return -1;
        put(writeBuffer_, pkgsData.c_str(), pkgsData.size());
    }
    // try to write it immediately
    int bytesWritten = write(SockFd(), writeBuffer_);
    if (bytesWritten < 0)
    {
        valid_ = false;
        logger_ptr->warn("In TcpConnection::Send(): connection (sockfd: {0}) send with fatal error!", SockFd());
        return -2;
    }
    // if there are data left
    if (!writeBuffer_.empty())
    {
        eventsSource_->EnableWrite();
    }

    logger_ptr->trace("In TcpConnection::Send(): connection (sockfd: {0}) send {1} bytes data.", SockFd(), bytesWritten);

    return 0;
}

int TcpConnection::Connect()
{
    int ret = connect(SockFd(), (struct sockaddr *)&addr_, sizeof(addr_));
    if (ret > -1)
    {
        logger_ptr->info("TcpConnection::Connect(): (sockfd: {0}) successfully connect to host", SockFd());
        eventsSource_->EnableWrite();
        eventsSource_->EnableRead();
        eventsSource_->EnableET();
        setNonBlocking(SockFd());
        valid_ = true;
        return 0;
    }
    logger_ptr->warn("In TcpConnection::Connect(): (sockfd: {0}) fail to connect to host", SockFd());
    return -1;
}

int TcpConnection::DisConnect()
{
    valid_ = false;
    logger_ptr->info("In TcpConnection::DisConnect(): (sockfd: {0}) disconnect from host", SockFd());
    ::close(SockFd());
    eventsSource_->RemoveFromLoop();
    return 0;
}

int TcpConnection::SockFd() const { return eventsSource_->fd(); }