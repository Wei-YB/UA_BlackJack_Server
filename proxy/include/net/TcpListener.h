#ifndef _TCP_H_
#define _TCP_H_

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

#include <exception>
#include <unordered_map>

#include "circ_buf.h"
#include "EventLoop.h"

#define DEFAULT_WAIT_QUEUE_LEN  10
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

template <typename ClientHandler>
class TcpListener final : public EventsHandler
{
public:
    TcpListener(const char *ip, unsigned short port,
                int wait_queue_len = DEFAULT_WAIT_QUEUE_LEN) : EventsHandler()
    {
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip, &m_addr.sin_addr) == 0)
        {
            throw "Socket: Invalid IP addr.\n";
        }

        if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            throw "Socket: fail to create socket.\n";
        }

        if (bind(m_sockfd, (struct sockaddr *)&m_addr, sizeof(m_addr)) < 0)
        {
            throw "TcpListener: fail to bind socket with address.\n";
        }

        if (listen(m_sockfd, wait_queue_len) < 0)
        {
            close(m_sockfd);
            throw "TcpListener: fail to listen.\n";
        }
    }

    ~TcpListener() {close(m_sockfd);}

    TcpListener(const TcpListener &) = delete;

    TcpListener &operator=(const TcpListener &) = delete;

public:
    int addToEventLoop(int sockfd, int events, EventLoop *loop)
    {
        m_eventLoop = loop;
        return m_eventLoop->add(m_sockfd, events, this);
    }

    int removeFromEventLoop(int sockfd, EventLoop *loop)
    {
        if (loop == m_eventLoop && m_sockfd == sockfd)
        {
            m_eventLoop->del(m_sockfd);
            return 0;
        }
        return -1;
    }

    int handleEvents(int sockfd, int events)
    {
        if (sockfd == m_sockfd && (events & EPOLLIN))
        {
            struct sockaddr_in addr;
            socklen_t len = sizeof(addr);
            int connfd = ::accept(m_sockfd, (struct sockaddr *)&addr, &len);
            if (connfd > 0)
            {
                EventsHandler *clientHandler = new ClientHandler(connfd, addr);
                if (clientHandler->addToPoller(connfd, EPOLLIN | EPOLLET | EPOLLERR, m_poller) < 0)
                {
                    delete clientHandler;
                }
            }
            return 0;
        }
        else if (sockfd == m_sockfd && (events & EPOLLERR))
        {
            return -1;
        }
        else
        {
            // unknown socket/events
        }
        
        return 0;
    }

private:
    int m_sockfd = -1;
    struct sockaddr_in m_addr;
    EventLoop *m_eventLoop = NULL;
};

class TcpClient : public EventsHandler
{
public:
    TcpClient(size_t bufferSize) : EventsHandler(), m_readbuffer(bufferSize), m_writebuffer(bufferSize)
    {
        if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            throw "Socket: fail to create socket.\n";
        }
    }

    ~TcpClient() {close(m_sockfd);}

public:
    int connect(const char *ipAddr, unsigned short port)
    {
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ipAddr, &m_addr.sin_addr) == 0)
        {
            return -1;
        }
        if (::connect(m_sockfd, (struct sockaddr *)&m_addr, sizeof(m_addr)) == 0)
        {
            fcntl(m_sockfd, F_SETFL, fcntl(m_sockfd, F_GETFL) | O_NONBLOCK);
            return 0;
        }
        return -1;
    }

    int addToEventLoop(EventLoop *loop)
    {
        m_eventLoop = loop;
        // return m_eventLoop->add(m_sockfd, EV_OUT | EV_ET | EV_ERR, this);
    }

    int handleEvents(int sockfd, Event events)
    {
        return 0;
    }

    int writeMsg(const char *msg, size_t length) 
    {
        int ret = put(buffer, msg, length);
        m_eventLoop->mod(m_sockfd, EV_OUT | EV_ET | EV_ERR, this);
        return ret;
    }

    int

private:
    int m_sockfd = -1;
    struct sockaddr_in m_addr;
    EventLoop *m_eventLoop = NULL;
    CircularBuffer m_readbuffer;
    CircularBuffer m_writebuffer;
};

};  // end of tcp namespace
#endif 