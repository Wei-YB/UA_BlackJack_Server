#include <memory>
#include "Client.h"
#include "TcpConnection.h"
#include "ClientProxyProtocol.h"
#include "common.h"
#include "EventLoop.h"

#include "UA_BlackJack.pb.h"


using ua_blackjack::Request;
using ua_blackjack::Response;
using namespace Net;


Client::Client(std::shared_ptr<TcpConnection> conn,
           const std::function<void(FileDesc, Request &)> &requestCallBack,
           const std::function<void(Response &)> &responseCallBack,
           const std::function<void(FileDesc)> &errorCallBack)
       : conn_(conn), 
        requestCallBack_(requestCallBack),
        responseCallBack_(responseCallBack),
        errorCallBack_(errorCallBack) 
{
    conn_->SetInputCallBack(std::bind(&Client::OnMessages, this, std::placeholders::_1, std::placeholders::_2));
    conn_->SetOutPutCallBack(std::bind(&Client::OnSendReady, this));
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
            std::cout << "no room to write response to client!" << std::endl;
            return -1;
        }
        ret = conn_->Send(pkgData);
    }
    return ret;
}

void Client::OnMessages(std::vector<Request> &requests, std::vector<Response> &responses)
{
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
