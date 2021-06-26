#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include <sys/types.h>
#include <netinet/in.h>

#include "CircularBuffer.h"
#include "EventLoop.h"
#include "TcpConnection.h"

#define DEFAULT_WAIT_QUEUE_LEN  64
#define DEFAULT_MAX_EVENTS  1024

namespace Net {

class TcpServer
{
public:
    TcpServer(const char *ip, 
            unsigned short port, 
            EventLoop *loop,
            const std::function<void(std::shared_ptr<TcpConnection>)> &connCb,
            const std::function<void(FileDesc)> &errCb);

    ~TcpServer();

    TcpServer(const TcpServer &) = delete;

    TcpServer &operator=(const TcpServer &) = delete;

public:
    int OnConnection();

    int OnError();

private:
    EventsSource eventsSource_;
    struct sockaddr_in addr_;
    EventLoop *loop_;
    std::function<void(std::shared_ptr<TcpConnection>)> connectionCallBack_;
    std::function<void(FileDesc)> errorCallBack_;
};

}  // end of tcp namespace
#endif 