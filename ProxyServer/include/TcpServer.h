#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <functional>
#include "Timer.h"

#define DEFAULT_WAIT_QUEUE_LEN  64
#define DEFAULT_HEALTH_REPORT_PERIOD    10

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
            const std::function<void(FileDesc)> &errCb,
            int healthReportPeriod = DEFAULT_HEALTH_REPORT_PERIOD);

    ~TcpServer();

    TcpServer(const TcpServer &) = delete;

    TcpServer &operator=(const TcpServer &) = delete;

    FileDesc listenFd() const {return eventsSource_->fd();}

private:
    int OnConnection();

    int OnError();

    void OnHealthReport();

private:
    std::shared_ptr<EventsSource> eventsSource_;
    struct sockaddr_in addr_;
    EventLoop *loop_;
    std::function<void(std::shared_ptr<TcpConnection>)> connectionCallBack_;
    std::function<void(FileDesc)> errorCallBack_;
    Timer timer_;
    int64_t connAccepted_ = 0;
};

}  // end of tcp namespace
#endif 
