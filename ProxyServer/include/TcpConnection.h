#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_
#include <functional>
#include <netinet/in.h>
#include <vector>
#include <memory>
#include "common.h"
#include "CircularBuffer.h"

using namespace ua_blackjack;

namespace Net {

class EventLoop;
class EventsSource;

class TcpConnection
{
    static const int bufferSize_ = 4096;
public:
    // for unconnected connection
    TcpConnection(const char *ip, unsigned short port, EventLoop *loop);
    TcpConnection(FileDesc connfd, const struct sockaddr_in &addr, EventLoop *loop);
    ~TcpConnection();

    //
    int Send(int32_t type, const std::string &pkgsData);

    int Connect();

    int DisConnect();

    int SockFd() const;

    // 
    //void SetInputCallBack(const std::function<void(std::vector<std::pair<int32_t, std::string>>)> &cb) {inputCallBack_ = cb;}
    void SetInputCallBack(const std::function<void(std::vector<std::pair<int32_t, StringPiece>>)> &cb) {inputCallBack_ = cb;}
    void SetOutPutCallBack(const std::function<void()> &cb) {outputCallBack_ = cb;}
    void SetHupCallBack(const std::function<void()> &cb) {hupCallBack_ = cb;}
    void SetErrorCallBack(const std::function<void()> &cb) {errorCallBack_ = cb;}
    void SetEncoder(const std::function<int(int32_t, const std::string &, CircularBuffer &)> &cb) {encoder_ = cb;}
    void SetDecoder(const std::function<StringPiece(CircularBuffer &, int*, int32_t*)> &cb) {decoder_ = cb;}
    CircularBuffer writeBuffer_;    // temporarily expose it
    EventLoop *eventLoop() const {return eventLoop_;};
private:
    // callbacks
    int OnInput();
    
    int OnOutput();

    int OnDisconnect();
    
    int OnError();

private:
    struct sockaddr_in addr_;
    std::shared_ptr<EventsSource> eventsSource_;
    //std::function<void(std::vector<std::pair<int32_t, std::string>>)> inputCallBack_;
    std::function<void(std::vector<std::pair<int32_t, StringPiece>>)> inputCallBack_;
    std::function<void()> outputCallBack_;
    std::function<void()> errorCallBack_;
    std::function<void()> hupCallBack_;
    std::function<int(int32_t, const std::string &, CircularBuffer &)> encoder_;
    // std::function<std::string(CircularBuffer &, int32_t *)> decoder_;
    std::function<StringPiece(CircularBuffer &, int*, int32_t*)> decoder_;

    CircularBuffer readBuffer_;
    EventLoop *eventLoop_;

    bool valid_ = false;
};

}   // namespace Net
#endif
