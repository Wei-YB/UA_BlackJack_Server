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
           const std::function<void(FileDesc, Request &)> &requestCallBack,
           const std::function<void(Response &)> &responseCallBack,
           const std::function<void(FileDesc)> &disconnectCallBack,
           const std::function<void(FileDesc)> &errorCallBack)
       : conn_(conn), 
        requestCallBack_(requestCallBack),
        responseCallBack_(responseCallBack),
        errorCallBack_(errorCallBack),
        disconnectCallBack_(disconnectCallBack) 
{
    conn_->SetInputCallBack(std::bind(&Client::OnMessages, this, std::placeholders::_1, std::placeholders::_2));
    conn_->SetOutPutCallBack(std::bind(&Client::OnSendReady, this));
    conn_->SetHupCallBack(std::bind(&Client::OnLeave, this));
    conn_->SetErrorCallBack(std::bind(&Client::OnError, this));
}

int Client::SendRequest(const Request &request)
{
    int ret = 0;
    std::string rawRequest = request.SerializeAsString();
    std::string pkgData(8 + rawRequest.size(), '\0');
    NS::pack(NS::REQUEST, rawRequest, &pkgData[0], pkgData.size());
    {   // when we try to write data to the lower tcp socket, we need to hold a lock
        std::lock_guard<std::mutex> guard(connLock_);
        if (pkgData.size() > conn_->GetWriteBufferRoom())
        {   // TODO: maybe we can buffer this request
            logger_ptr->info("In gRPC server thread: Try to send request to client (uid: {}), but tcp buffer has no free room.", request.uid());
            return -1;
        }
        ret = conn_->Send(pkgData);
    }
    return ret;
}

int Client::SendResponse(const Response &response)
{
    int ret = 0;
    std::string rawResponse = response.SerializeAsString();
    std::string pkgData(8 + rawResponse.size(), '\0');
    NS::pack(NS::RESPONSE, rawResponse, &pkgData[0], pkgData.size());
    {
        std::lock_guard<std::mutex> guard(connLock_);
        if (pkgData.size() > conn_->GetWriteBufferRoom())
        {   // TODO: maybe we can buffer this request
            logger_ptr->info("In gRPC server thread: Try to send request to response (uid: {}), but tcp buffer has no free room.", response.uid());
            return -1;
        }
        ret = conn_->Send(pkgData);
    }
    return ret;
}

void Client::OnMessages(std::vector<Request> &requests, std::vector<Response> &responses)
{
    logger_ptr->info("In main thread: tcp (sockfd: {0}) get {1} requests and {2} response from client (uid: {3})", 
                                            conn_->SockFd(), requests.size(), responses.size(), uid_);
    if (requestCallBack_)
    {
        for (int i = 0; i < requests.size(); ++i)
        {
            requestCallBack_(conn_->SockFd(), requests[i]);
        }
    }
    if (responseCallBack_)
    {
        for (int i = 0; i < responses.size(); ++i)
        {
            responseCallBack_(responses[i]);
        }
    }
}

void Client::OnLeave() {disconnectCallBack_(conn_->SockFd());}

void Client::OnError() {errorCallBack_(conn_->SockFd());}
