#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_
#include <functional>
#include <netinet/in.h>
#include "common.h"
#include "UA_BlackJack.pb.h"
#include "EventLoop.h"
#include "CircularBuffer.h"

using ua_blackjack::Request;
using ua_blackjack::Response;

namespace Net {

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

    int SockFd() const {return eventsSource_.fd();}
    int GetWriteBufferRoom() const {return writeBuffer_.capacity() - writeBuffer_.size();}
    // 
    void SetInputCallBack(const std::function<void(std::vector<Request>&, std::vector<Response>&)> &cb) {inputCallBack_ = cb;}
    void SetOutPutCallBack(const std::function<void()> &cb) {outputCallBack_ = cb;}
    void SetErrorCallBack(const std::function<void()> &cb) {errorCallBack_ = cb;}

private:
    struct sockaddr_in addr_;
    Net::EventsSource eventsSource_;
    std::function<void(std::vector<Request>&, std::vector<Response>&)> inputCallBack_;
    std::function<void()> outputCallBack_;
    std::function<void()> errorCallBack_;
    Net::CircularBuffer writeBuffer_;
    Net::CircularBuffer readBuffer_;
};

}   // namespace Net
#endif
