#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <mutex>
#include <exception>
#include <unordered_map>

#include "CircularBuffer.h"
#include "EventLoop.h"
#include "asyncServiceClient.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "log.h"

using namespace Net;

static int setNonBlocking(int fd)
{
    int opt = fcntl(fd, F_GETFL);
    if (fcntl(fd, F_SETFL, opt | O_NONBLOCK) < 0)
    {
        return -1;
    }
    return 0;
}


TcpServer::TcpServer(const char *ip, 
        unsigned short port, 
        EventLoop *loop,
        const std::function<void(std::shared_ptr<TcpConnection>)> &connCb,
        const std::function<void(FileDesc)> &errCb) 
        : loop_(loop), connectionCallBack_(connCb), errorCallBack_(errCb)
{
    eventsSource_ = std::make_shared<EventsSource>(socket(AF_INET, SOCK_STREAM, 0), loop, 
                                                std::bind(&TcpServer::OnConnection, this),
                                                nullptr,
                                                std::bind(&TcpServer::OnError, this));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr_.sin_addr) == 0)
    {
        throw "Socket: Invalid IP addr.\n";
    }
    if (eventsSource_->fd() < 0)
    {
        throw "Socket: fail to create socket.\n";
    }

    int reuse = 1;
    setsockopt(eventsSource_->fd(), SOL_SOCKET, SO_REUSEPORT, (const void *)&reuse , sizeof(int));
    
    if (bind(eventsSource_->fd(), (struct sockaddr *)&addr_, sizeof(addr_)) < 0)
    {
        throw "TcpListener: fail to bind socket with address.\n";
    }
    if (listen(eventsSource_->fd(), DEFAULT_WAIT_QUEUE_LEN) < 0)
    {
        close(eventsSource_->fd());
        throw "TcpListener: fail to listen.\n";
    }
    setNonBlocking(eventsSource_->fd());
    eventsSource_->Update(Net::EV_IN | Net::EV_ET | Net::EV_ERR);
}

TcpServer::~TcpServer() 
{
    close(eventsSource_->fd());
}


int TcpServer::OnConnection()
{  
    logger_ptr->info("In main thread: Tcp server detect new connection requests.");
    int connCnt = 0;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    // accept until no connections
    FileDesc connfd;
    while (-1 < (connfd = ::accept(eventsSource_->fd(), (struct sockaddr *)&addr, &len)))
    {
        connCnt++;
        std::shared_ptr<TcpConnection> conn = std::make_shared<TcpConnection>(connfd, addr, loop_);
        if (connectionCallBack_)
        {
            connectionCallBack_(conn);
        }
    }
    logger_ptr->info("In main thread: Tcp server accept {} new connections.", connCnt);
    return 0;
}

int TcpServer::OnError()
{
    logger_ptr->info("In main thread: Tcp server has error.");
    if (errorCallBack_)
        errorCallBack_(eventsSource_->fd());
    return -1;
}
