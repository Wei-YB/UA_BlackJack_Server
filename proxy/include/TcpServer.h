#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

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

#define DEFAULT_WAIT_QUEUE_LEN  64
#define DEFAULT_MAX_EVENTS  1024

// int setNonBlocking()
//     {
//         int opt = fcntl(m_sockfd, F_GETFL);
//         if (fcntl(m_sockfd, F_SETFL, opt | O_NONBLOCK) < 0)
//         {
//             return -1;
//         }
//         return 0;
//     }
namespace Net {

class TcpServer
{
public:
    TcpServer(const char *ip, unsigned short port, EventLoop *loop,
            const std::function<void(std::shared_ptr<TcpConnection>)> &connCb) 
            : eventsSource_(socket(AF_INET, SOCK_STREAM, 0), loop, 
                            std::bind(&TcpServer::OnConnection, this),
                            nullptr,
                            std::bind(&TcpServer::OnError, this)),
            loop_(loop), connectionCallBack_(connCb), errorCallBack_(nullptr)
    {
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port);
        if (inet_pton(AF_INET, ip, &addr_.sin_addr) == 0)
        {
            throw "Socket: Invalid IP addr.\n";
        }

        if (eventsSource_.fd() < 0)
        {
            throw "Socket: fail to create socket.\n";
        }

        if (bind(eventsSource_.fd(), (struct sockaddr *)&addr_, sizeof(addr_)) < 0)
        {
            throw "TcpListener: fail to bind socket with address.\n";
        }

        if (listen(eventsSource_.fd(), DEFAULT_WAIT_QUEUE_LEN) < 0)
        {
            close(eventsSource_.fd());
            throw "TcpListener: fail to listen.\n";
        }
    }

    ~TcpServer() {close(eventsSource_.fd());}

    TcpServer(const TcpServer &) = delete;

    TcpServer &operator=(const TcpServer &) = delete;

public:
    int OnConnection()
    {  
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        int connfd = ::accept(eventsSource_.fd(), (struct sockaddr *)&addr, &len);
        if (connfd > 0)
        {
            std::shared_ptr<TcpConnection> conn = std::make_shared<TcpConnection>(connfd, addr);
            if (connectionCallBack_)
            {
                connectionCallBack_(conn);
            }
        }
        return 0;
    }

    int OnError()
    {
        if (errorCallBack_)
            errorCallBack_();
        return -1;
    }

private:
    EventsSource eventsSource_;
    struct sockaddr_in addr_;
    EventLoop *loop_;
    std::function<void(std::shared_ptr<TcpConnection>)> connectionCallBack_;
    std::function<void()> errorCallBack_;
};



};  // end of tcp namespace
#endif 