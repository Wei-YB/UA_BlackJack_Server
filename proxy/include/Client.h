#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <memory>
#include "common.h"
#include "UA_BlackJack.pb.h"
#include "EventLoop.h"
#include "TcpConnection.h"

using ua_blackjack::Response;
using ua_blackjack::Request;

class Client
{
public:
    Client(std::shared_ptr<TcpConnection> conn,
           const std::function<void(FileDesc, Request &)> &requestCallBack,
           const std::function<void(Response &)> &responseCallBack,
           const std::function<void(FileDesc)> &errorCallBack);
public:
    //
    int SendRequest(const Request &request);

    int SendResponse(const Response &response);
    
    UserId uid() const {return uid_;}
    void SetUid(UserId id) {uid_ = id;}
    void SetUnloginStamp(int64_t stamp) {unloginStamp_ = stamp;}
    // callbacks
    void OnMessages(std::vector<Request> &requests, std::vector<Response> &responses);
    void OnSendReady() {/*TODO: write requests/responses buffer to tcp connection*/;}
    void OnError() {errorCallBack_(conn_->SockFd());}
private:
    UserId uid_ = -1;
    int64_t unloginStamp_;
    std::mutex connLock_;
    std::shared_ptr<TcpConnection> conn_;
    std::function<void(FileDesc, Request &)> requestCallBack_;
    std::function<void(Response &)> responseCallBack_;
    std::function<void(FileDesc)> errorCallBack_;
};

#endif