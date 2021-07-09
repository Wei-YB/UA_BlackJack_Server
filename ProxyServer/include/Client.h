#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <memory>
#include "common.h"
#include <functional>
#include <vector>

namespace Net {
    class TcpConnection;
    class StringPiece;
    class Timer;
}

namespace ua_blackjack {
    class Response;
    class Request;
}

using namespace Net;
using ua_blackjack::Response;
using ua_blackjack::Request;



class Client
{
public:
    Client(std::shared_ptr<TcpConnection> conn,
           const std::function<void(FileDesc, Request)> &requestCallBack,
           const std::function<void(Response)> &responseCallBack,
           const std::function<void(FileDesc)> &disconnectCallBack,
           const std::function<void(FileDesc)> &errorCallBack);
public:
    //
    int SendRequest(const Request &request);

    int SendResponse(const Response &response);
    
    int EnableTimeout(int sec);

    int DisableTimeout();

    FileDesc fd() const;
    UserId uid() const {return uid_;}
    void SetUid(UserId id) {uid_ = id;}
    int64_t unloginStamp() const {return unloginStamp_;}
    void SetUnloginStamp(int64_t stamp) {unloginStamp_ = stamp;}
    int64_t signupStamp() const {return signupStamp_;}
    void SetSignupStamp(int64_t stamp) {signupStamp_ = stamp;}
    int64_t logoutStamp() const {return logoutStamp_;}
    void SetLogoutStamp(int64_t stamp) {logoutStamp_ = stamp;}
    
private:
    // callbacks
    //void OnMessages(std::vector<std::pair<int32_t, std::string>> msgs);
    void OnMessages(std::vector<std::pair<int32_t, StringPiece>> msgs);
    void OnLeave();
    void OnSendReady() {/*TODO: write requests/responses buffer to tcp connection*/;}
    void OnError();

private:
    UserId uid_ = -1;
    int64_t unloginStamp_;
    int64_t signupStamp_;
    int64_t logoutStamp_;
    std::mutex connLock_;
    std::shared_ptr<TcpConnection> conn_;
    std::shared_ptr<Timer> timer_;
    std::function<void(FileDesc, Request)> requestCallBack_;
    std::function<void(Response)> responseCallBack_;
    std::function<void(FileDesc)> disconnectCallBack_;
    std::function<void(FileDesc)> errorCallBack_;
};

#endif