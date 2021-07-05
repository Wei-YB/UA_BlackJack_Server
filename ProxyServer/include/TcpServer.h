#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <functional>

#define DEFAULT_WAIT_QUEUE_LEN  64

namespace Net {
class EventsSource;
class EventLoop;
class TcpConnection;

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

    FileDesc listenFd() const {return eventsSource_->fd();}

// private:
    int OnConnection();

    int OnError();

private:
    std::shared_ptr<EventsSource> eventsSource_;
    struct sockaddr_in addr_;
    EventLoop *loop_;
    std::function<void(std::shared_ptr<TcpConnection>)> connectionCallBack_;
    std::function<void(FileDesc)> errorCallBack_;
};

}  // end of tcp namespace
#endif 
