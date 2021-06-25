#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <memory>
#include "common.h"
#include "common.pb.h"
#include "TcpConnection.h"

using common::Request;
using common::Response;

class Client
{
public:
    Client(std::shared_ptr<TcpConnection> conn, 
           const std::function<void(FileDesc, const Request &)> &requestCallBack,
           const std::function<void(const Response &)> &responseCallBack,
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
public:
    //
    int SendRequest(const Request &request)
    {
        std::string rawRequest = request.SerializeAsString();
        std::string pkgData(8 + rawRequest.size(), '\0');
        NS::pack(NS::REQUEST, rawRequest, &pkgData[0], pkgData.size());
        {
            std::lock_guard<std::mutex> guard(connLock_);
            if (pkgData.size() > conn_->GetWriteBufferRoom())
            {   // TODO: maybe we can buffer this request
                return -1;
            }
            conn_->Send(pkgData);
        }
        return 0;
    }

    int SendResponse(const Response &response)
    {
        std::string rawResponse = response.SerializeAsString();
        std::string pkgData(8 + rawResponse.size(), '\0');
        NS::pack(NS::RESPONSE, rawResponse, &pkgData[0], pkgData.size());
        {
            std::lock_guard<std::mutex> guard(connLock_);
            if (pkgData.size() > conn_->GetWriteBufferRoom())
            {   // TODO: maybe we can buffer this request
                return -1;
            }
            conn_->Send(pkgData);
        }
        return 0;
    }
    
    UserId uid() const {return uid_;}
    void SetUid(UserId id) {uid_ = id;}
    void SetUnloginStamp(int64_t stamp) {unloginStamp_ = stamp;}
    // callbacks
    void OnMessages(const std::vector<Request> &requests, const std::vector<Response> &responses)
    {
        for (int i = 0; i < requests.size(); ++i)
        {
            requestCallBack_(conn_->SockFd(), requests[i]);
        }
        for (int i = 0; i < responses.size(); ++i)
        {
            responseCallBack_(responses[i]);
        }
    }
    void OnSendReady() {/*TODO: write requests/responses buffer to tcp connection*/;}
    void OnError() {errorCallBack_(conn_->SockFd());}
private:
    UserId uid_ = -1;
    int64_t unloginStamp_;
    std::mutex connLock_;
    std::shared_ptr<TcpConnection> conn_;
    std::function<void(FileDesc, const Request &)> requestCallBack_;
    std::function<void(const Response &)> responseCallBack_;
    std::function<void(FileDesc)> errorCallBack_;
};

#endif