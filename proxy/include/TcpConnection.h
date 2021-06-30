#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_
#include <functional>
#include <netinet/in.h>
#include "common.h"
#include "CircularBuffer.h"
#include "EventLoop.h"

namespace ua_blackjack {
    class Response;
    class Request;
}

using namespace ua_blackjack;

namespace Net {

class EventLoop;

class TcpConnection
{
    static const int bufferSize_ = 4096;
public:
    // for unconnected connection
    TcpConnection(const char *ip, unsigned short port, Net::EventLoop *loop);
    TcpConnection(FileDesc connfd, const struct sockaddr_in &addr, Net::EventLoop *loop);
    ~TcpConnection();

    // callbacks
    int OnInput();
    
    int OnOutput();
    
    int OnError();
    //
    int Send(const std::string &pkgsData);

    int Connect();

    int DisConnect();

    int SockFd() const;
    int GetWriteBufferRoom() const;
    // 
    void SetInputCallBack(const std::function<void(std::vector<Request>&, std::vector<Response>&)> &cb) {inputCallBack_ = cb;}
    void SetOutPutCallBack(const std::function<void()> &cb) {outputCallBack_ = cb;}
    void SetHupCallBack(const std::function<void()> &cb) {hupCallBack_ = cb;}
    void SetErrorCallBack(const std::function<void()> &cb) {errorCallBack_ = cb;}

private:
    struct sockaddr_in addr_;
    std::shared_ptr<EventsSource> eventsSource_;
    std::function<void(std::vector<Request>&, std::vector<Response>&)> inputCallBack_;
    std::function<void()> outputCallBack_;
    std::function<void()> errorCallBack_;
    std::function<void()> hupCallBack_;
    CircularBuffer writeBuffer_;
    CircularBuffer readBuffer_;
};

}   // namespace Net
#endif
