#ifndef _ASYNCPROXYSERVER_H_
#define _ASYNCPROXYSERVER_H_

#include <iostream>
#include <memory>
#include <string>
#include <mutex>
#include <functional>

#include <chrono>
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "common.pb.h"
#include "proxy.grpc.pb.h"
#include "EventLoop.h"
#include "ProxyServer.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using common::Response;
using common::Request;

namespace {
    enum CallStatus {CREATE, PROCESS, FORWARD, FINISH};
}

struct AsyncCall
{
    AsyncCall() : responder_(&ctx_), status_(CREATE) {}
    
    ServerContext ctx_;
    Request request_;
    Response reply_;
    ServerAsyncResponseWriter<Response> responder_;
    CallStatus status_;
    int64_t stamp_;
};

void returnFailureResponse(AsyncCall *call, const std::string &msg)
{
    call->reply_.set_uid(call->request_.uid());
    call->reply_.set_status(-1);
    call->reply_.set_stamp(call->request_.stamp());
    call->reply_.add_args(msg);
    call->responder_.Finish(call->reply_, Status::OK, call);
    call->status_ = FINISH;
}

class ProxyRpcServer final {
public:
    ProxyRpcServer(const std::string &serverAddress,
                    std::shared_ptr<ProxyServer> proxyPtr)
        : serverAddress_(serverAddress), proxy_(proxyPtr) {}

    ~ProxyRpcServer()
    {
        server_->Shutdown();
        cq_->Shutdown();
    }

    void Run() 
    {   
        ServerBuilder builder;
        builder.AddListeningPort(serverAddress_, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);

        if (std::shared_ptr<ProxyServer> sharedProxy = proxy_.lock())
        {
            sharedProxy->SetClientResponseCallBack(std::bind(&ProxyRpcServer::OnClientResponse, this, std::placeholders::_1));
        }
        else
        {
            std::cout << "Server fail to contact with proxy." << std::endl;
            return;
        }

        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << serverAddress_ << std::endl;
    
        HandleRpcs();
    }

private:
    void HandleRpcs()
    {
        AsyncCall *newCall = new AsyncCall;
        ProcessCall(newCall);
        void *tag;
        bool ok;
        time_point deadline = std::chrono::system_clock::now() +
                                std::chrono::milliseconds(500);
        while (!stop_ && cq_->AsyncNext(&tag, &ok, deadline))
        {
            deadline = std::chrono::system_clock::now() +
                                std::chrono::milliseconds(500);
            if (!ok)
            {
                continue;
            }
            AsyncCall *call = static_cast<AsyncCall*>(tag);
            ProcessCall(call);
        }
    }

    void ProcessCall(AsyncCall *call)
    {
        if (call->status_ == CREATE)
        {
            call->status_ = PROCESS;
            service_.RequestNotifyUser(&call->ctx_, &call->request_, &call->responder_, cq_.get(), cq_.get(), call);
        }
        else if (call->status_ == PROCESS)
        {
            AsyncCall *newCall = new AsyncCall;
            // check the validity of the request
            if (call->request_.requesttype() != common::Request::NOTIFY_USER)
            {
                // drop the request, and return fail to caller
                returnFailureResponse(call, "Request type not allowed.");
                return;
            }
            if (std::shared_ptr<ProxyServer> sharedProxy = proxy_.lock())
            {
                // modify request stamp
                int64_t originStamp = call->request_.stamp();
                int64_t newStamp = (int64_t)call;
                call->request_.set_stamp(newStamp);
                call->stamp_ = originStamp;
                if (0 > sharedProxy->SendRequest(call->request_))
                {
                    returnFailureResponse(call, "User busy or do not exist.");
                    return;
                }
                {
                    std::lock_guard<std::mutex> guard(stampToAsyncCallLock_);
                    stampToAsyncCall_.emplace(newStamp, call);
                }
                return;
            }
            returnFailureResponse(call, "Proxy service unavailable.");
        }
        else
        {
            delete call;
        }
    }

    void OnClientResponse(Response &response)
    {
        int64_t stamp = response.stamp();
        AsyncCall *call = NULL;
        {
            std::lock_guard<std::mutex> guard(stampToAsyncCallLock_);
            if (stampToAsyncCall_.find(stamp) != stampToAsyncCall_.end())
            {
                call = stampToAsyncCall_[stamp];
                stampToAsyncCall_.erase(stamp);
            }
        }
        if (call)
        {
            stamp = call->stamp_;
            response.set_stamp(stamp);
            call->reply_ = response;
            call->responder_.Finish(call->reply_, Status::OK, call);
            call->status_ = FINISH;
        }
    }

    void Stop() {stop_ = true;}

private:
    std::string serverAddress_;
    std::unique_ptr<Server> server_;
    proxy::Proxy::AsyncService service_;
    std::unique_ptr<ServerCompletionQueue> cq_;
    std::weak_ptr<ProxyServer> proxy_;
    std::unordered_map<int64_t, AsyncCall*> stampToAsyncCall_;
    std::mutex stampToAsyncCallLock_;
    bool stop_ = false;
};

#endif