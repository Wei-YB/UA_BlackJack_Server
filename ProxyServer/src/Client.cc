#include <memory>
#include "Client.h"
#include "TcpConnection.h"
#include "ClientProxyProtocol.h"
#include "common.h"
#include "EventLoop.h"
#include "log.h"

#include "UA_BlackJack.pb.h"


using ua_blackjack::Request;
using ua_blackjack::Response;
using namespace Net;



Client::Client(std::shared_ptr<TcpConnection> conn,
           const std::function<void(FileDesc, Request)> &requestCallBack,
           const std::function<void(Response)> &responseCallBack,
           const std::function<void(FileDesc)> &disconnectCallBack,
           const std::function<void(FileDesc)> &errorCallBack)
       : conn_(conn), 
        requestCallBack_(requestCallBack),
        responseCallBack_(responseCallBack),
        errorCallBack_(errorCallBack),
        disconnectCallBack_(disconnectCallBack) 
{
    conn_->SetInputCallBack(std::bind(&Client::OnMessages, this, std::placeholders::_1));
    conn_->SetOutPutCallBack(std::bind(&Client::OnSendReady, this));
    conn_->SetHupCallBack(std::bind(&Client::OnLeave, this));
    conn_->SetErrorCallBack(std::bind(&Client::OnError, this));
    conn_->SetEncoder(std::bind(pack, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    //conn_->SetDecoder(std::bind(unpack, std::placeholders::_1, std::placeholders::_2));
    conn_->SetDecoder(std::bind(unpack_sp, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

int Client::SendRequest(const Request &request)
{
    std::string rawRequest(std::move(request.SerializeAsString()));
    // when we try to write data to the lower tcp socket, we need to hold a lock
    std::lock_guard<std::mutex> guard(connLock_);
    return conn_->Send(REQUEST, rawRequest);
}

int Client::SendResponse(const Response &response)
{
    std::string rawResponse(std::move(response.SerializeAsString()));
    // when we try to write data to the lower tcp socket, we need to hold a lock
    std::lock_guard<std::mutex> guard(connLock_);
    return conn_->Send(RESPONSE, rawResponse);
}

//void Client::OnMessages(std::vector<std::pair<int32_t, std::string>> msgs)
void Client::OnMessages(std::vector<std::pair<int32_t, StringPiece>> msgs)
{
    int requestCnt = 0, responseCnt = 0;
    for (int i = 0; i < msgs.size(); ++i)
    {
        if (msgs[i].first == REQUEST)
        {
            Request request;
            ParseFromStringPiece(request, msgs[i].second);
            //request.ParseFromString(std::get<1>(msgs[i]));
            if (requestCallBack_) requestCallBack_(conn_->SockFd(), std::move(request));
            requestCnt++;
        }
        else if (msgs[i].first == RESPONSE)
        {
            Response response;
            //response.ParseFromString(std::get<1>(msgs[i]));
            ParseFromStringPiece(response, msgs[i].second);
            if (responseCallBack_) responseCallBack_(std::move(response));
            responseCnt++;
        }
    }
    // logger_ptr->info("In main thread: tcp (sockfd: {0}) get {1} requests and {2} response from client (uid: {3})", conn_->SockFd(), requestCnt, responseCnt, uid_);
}

void Client::OnLeave() 
{
    // logger_ptr->info("In main thread: client (uid: {}) is leaving.", uid_);
    if (disconnectCallBack_) 
    {
        disconnectCallBack_(conn_->SockFd());
        return;
    }
    // logger_ptr->info("In main thread: client (uid: {}) has no chance to tell the proxy server.", uid_);
}

void Client::OnError() {errorCallBack_(conn_->SockFd());}
