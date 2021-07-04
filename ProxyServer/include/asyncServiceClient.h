#ifndef _ASYNCSERVICECALIENT_H_
#define _ASYNCSERVICECALIENT_H_

#include <memory>
#include <string>
#include <mutex>
#include <chrono>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "UA_BlackJack.pb.h"
#include "UA_BlackJack.grpc.pb.h"
#include "EventLoop.h"
#include "common.h"
#include "BlockingQueue.h"
#include "log.h"

typedef std::chrono::system_clock SystemClock;
typedef std::chrono::time_point<SystemClock> TimePoint;
typedef std::chrono::milliseconds MilliSeconds;

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;

using ua_blackjack::Response;
using ua_blackjack::Request;

class ServiceClient
{
public:
    ServiceClient(const std::string &serviceName, const std::string &serviceAddr) 
                  : serviceName_(serviceName), serviceAddr_(serviceAddr), stop_(false) {}
    ~ServiceClient() {}
public:
    virtual int Call(const Request &) = 0;
    // 
    void AsyncCompleteRpc()
    {   
        if (serviceAddr_ == "0.0.0.0:0")
            AsyncCompleteRpc_stand_alone_();
        else
            AsyncCompleteRpc_();
    }

    void SetResponseCallBack(const std::function<void(Response&)> &cb) {responseCallBack_ = cb;}
    
    void StopClient() {stop_ = true;}
    
// private:
    void AsyncCompleteRpc_()
    {
        void *got_tag;
        bool ok = false;

        // logger_ptr->info("In {0} thread: Thread started.", serviceName_);
        TimePoint deadline = SystemClock::now() + MilliSeconds(500);
        grpc::CompletionQueue::NextStatus sta;
        while (!stop_ && (sta = cq_.AsyncNext(&got_tag, &ok, deadline)))
        {
            AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
            if (!ok || sta == grpc::CompletionQueue::NextStatus::TIMEOUT)
            {
                continue;
            }
            // call back the Proxy
            if (responseCallBack_ && call->status.ok())
            {
                responseCallBack_(call->reply);
            }
            else 
            {
                // logger_ptr->info("In {} thread: response not ok", serviceName_);
                Response res;
                res.set_status(-1);
                res.set_uid(call->request.uid());
                res.set_stamp(call->request.stamp());
                if (responseCallBack_)
                    responseCallBack_(call->reply);
            }
            delete call;
            deadline = SystemClock::now() + MilliSeconds(500);
        }

        // logger_ptr->info("In {} thread: leaving the thread", serviceName_);
    }

    void AsyncCompleteRpc_stand_alone_()
    {
        AsyncClientCall *call;
        while (!stop_)
        {
            if (callQueue_.Pop(call, 1000))
            {
                if (call->request.requesttype() == Request::LOGIN 
                    || call->request.requesttype() == Request::SIGNUP)
                {
                    call->reply.set_uid(++requestCnt_);
                    // logger_ptr->info("In gRPC client {0} thread: this is a login request, set its uid to {1}", serviceName_, requestCnt_);
                }
                else
                {
                    call->reply.set_uid(call->request.uid());
                    // logger_ptr->info("In gRPC client {0} thread: this is a normal request, simply echo back", serviceName_);
                }
                call->reply.set_stamp(call->request.stamp());
                call->reply.set_status(0);
                call->reply.add_args(serviceName_ + " successfully handle your requets.");
                if (responseCallBack_)
                {
                    responseCallBack_(call->reply);
                }
                delete call;
            }   
        }

        // logger_ptr->info("In gRPC client {0} thread: leaving the thread", serviceName_);
    }

protected:
    struct AsyncClientCall
    {
        Response reply;
        ClientContext context;
        Status status;
        std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
        Request request;
    };

    std::string serviceName_;
    std::string serviceAddr_;
    CompletionQueue cq_;
    std::function<void(Response&)> responseCallBack_;
    BlockingQueue<AsyncClientCall*> callQueue_;
    int64_t requestCnt_ = 0;
    bool stop_;
};

template<typename ConcreteService>
class ConcreteServiceClient : public ServiceClient
{
public:
    ConcreteServiceClient(const std::string &serviceName, 
                          const std::string &serviceAddr) 
                        : ServiceClient(serviceName, serviceAddr) 
    {
        if (serviceAddr != "0.0.0.0:0")
            stub_ = ConcreteService::NewStub(CreateChannel(serviceAddr, InsecureChannelCredentials()));
    }

public:
    int Call(const Request &request)
    {  
        // logger_ptr->info("In main thread: client (uid: {0}) call {1}", request.uid(), serviceName_);
        AsyncClientCall *call =  new AsyncClientCall;
        call->request = request;
        if (serviceAddr_ != "0.0.0.0:0")
        {
            call->response_reader = stub_->PrepareAsyncNotify(&call->context, request, &cq_);
            call->response_reader->StartCall();
            call->response_reader->Finish(&call->reply, &call->status, (void*)call);
        }    
        else
        {
            // logger_ptr->info("push request (type: {0}, from: uid = {1}) into call queue.", request.requesttype(), request.uid());
            callQueue_.Push(call);
        }
        return 0;
    }

private:
    std::unique_ptr<typename ConcreteService::Stub> stub_;
};

#endif

