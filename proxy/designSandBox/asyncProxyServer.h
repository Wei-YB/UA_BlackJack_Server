#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "common.pb.h"
#include "proxy.grpc.pb.h"
#include "EventLoop.h"
#include "common.h"
#include "classesDesign.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using proxy::Proxy;
using common::Response;
using common::Request;


class AsyncCall
{
public:
    AsyncCall(Proxy::AsyncService *service, 
              ServerCompletionQueue *cq)
            : service_(service), cq_(cq), cb_(cb), 
            responder_(&ctx_), status_(CREATE) 
    {Proceed();}
    
    void Proceed()
    {
        if (status_ == CREATE)
        {
            status_ = PROCESS;
            service_->RequestNotifyUser(&ctx_, &request_, &responder_, cq_, cq_, this);
        }
        else if (status_ == PROCESS)
        {
            new AsyncCall(service_, cq_, cb_);
            // check the validity of the request
            if (request_.requesttype() != common::Request::NOTIFY_USER)
            {
                returnFailureResponse("Request type not allowed.");
                return;
            }
            // TODO: check the validity of the functor
            cb_(request_);
        }
        else if (status_ == FORWARD)
        {
            responder_.Finish(reply_, Status::OK, this);
            status_ = FINISH;
        }
        else
        {
            // GPR_ASSERT(status_ == FINISH);
            delete this;
        }
    }

    void setReply(const Response &reply)
    {
        reply_ = reply;
    }

private:
    void returnFailureResponse(const std::string &msg)
    {
        reply_.set_uid(request_.uid());
        reply_.set_status(-1);
        reply_.set_stamp(request_.stamp());
        reply_.add_args(msg);
        responder_.Finish(reply_, Status::OK, this);
        status_ = FINISH;
    }

private:
    Proxy::AsyncService *service_;
    ServerCompletionQueue *cq_;
    ServerContext ctx_;
    Request request_;
    Response reply_;
    ServerAsyncResponseWriter<Response> responder_;
    enum CallStatus {CREATE, PROCESS, FORWARD, FINISH};
    CallStatus status_;
    std::function<void(const Request &)> cb_;
};

class ProxyRpcServer
{
public:
    void Response();
    void SetRequestCallBack(const std::function<void(const Request &)>&);
private:
    std::function<void(const Request &)> requestCallBack_;
};

class ProxyRpcServer
{
public:
    ProxyRpcServer(const std::string &serverAddress) : serverAddress_(serverAddress) {};
    ~ProxyRpcServer() {server_->Shutdown(); cq_->Shutdown();}
public:
    // this function should be run as a thread
    void Run() 
    {   
        ServerBuilder builder;
        builder.AddListeningPort(serverAddress_, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);

        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();
        if (std::shared_ptr<Proxy> sharedProxy = proxy_.lock())
        {
            sharedProxy->
        }
        std::cout << "Server listening on " << serverAddress_ << std::endl;
    
        HandleRpcs();
    }

    void OnResponse(const Response &response)
    {
        int64_t key = response.stamp();
        
        if (asyncCalls_.find(key) != asyncCalls_.end())
        {
            AsyncCall *call = asyncCalls_[key];
            call->setReply(response);
            call->Proceed();
            asyncCalls_.erase(key);
        }
    }

    void SetRequestCallBack(const std::function<void(const Request &)> &cb)
    {
        requestCallBack_ = cb;
    }

private:
    void HandleRpcs()
    {
        // TODO, check the validity of requestCallBack_
        new AsyncCall(&service_, cq_.get());
        void *tag;
        bool ok;
        while (true)
        {
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<AsyncCall*>(tag)->Proceed();
        }
    }

private:
    std::string serverAddress_;
    std::unique_ptr<Server> server_;
    Proxy::AsyncService service_;
    std::unique_ptr<ServerCompletionQueue> cq_;
    std::weak_ptr<Proxy> proxy_;
    std::unordered_map<int64_t, AsyncCall*> asyncCalls_;
    std::mutex asyncCallsLock_;
};

