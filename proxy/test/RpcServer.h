#ifndef _RPCSERVER_H_
#define _RPCSERVER_H_

#include <memory>
#include <string>
#include <thread>
#include <functional>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "UA_BlackJack.grpc.pb.h"
#include "UA_BlackJack.pb.h"
#include "log.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using namespace ua_blackjack;

template<typename AsyncRpcService>
class AsyncCall
{
    typedef std::function<void(const Request&, Response&)> RequestCallBack;
public:
    AsyncCall(AsyncRpcService *service, 
            ServerCompletionQueue *cq,
            const RequestCallBack &cb)
            : service_(service), cq_(cq), 
            responder_(&ctx_), status_(CREATE), cb_(cb)
            {Proceed();}
    
    void Proceed()
    {
        if (status_ == CREATE)
        {
            status_ = PROCESS;
            service_->RequestNotify(&ctx_, &request_, &responder_, cq_, cq_, this);
        }
        else if (status_ == PROCESS)
        {
            new AsyncCall<AsyncRpcService>(service_, cq_, cb_);
            cb_(request_, response_);
            status_ = FINISH;
            responder_.Finish(response_, Status::OK, this);
        }
        else
        {
            delete this;
        }
    }

private:
    AsyncRpcService *service_;
    ServerCompletionQueue *cq_;
    ServerContext ctx_;
    Request request_;
    Response response_;
    ServerAsyncResponseWriter<Response> responder_;
    enum CallStatus {CREATE, PROCESS, FINISH};
    CallStatus status_;
    RequestCallBack cb_;
};

template<typename AsyncRpcService>
class AsyncRpcServer final {
    typedef std::function<void(const Request&, Response&)> RequestCallBack;
public:
    // the callback is used to tell the server how to deal with the 
    // request when it comes
    AsyncRpcServer( const std::string &serverAddr, 
                    const RequestCallBack &cb) 
                    : serverAddr_(serverAddr),
                    cb_(cb) {}
    ~AsyncRpcServer() {server_->Shutdown(); cq_->Shutdown();}

    void Run()
    {
        ServerBuilder builder;
        builder.AddListeningPort(serverAddr_, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);
        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();
        logger_ptr->info("Server listening on {}", serverAddr_);
        HandleRpcs();
    }

    void Stop() {flag_ = true;}

private:
    void HandleRpcs()
    {
        new AsyncCall<AsyncRpcService>(&service_, cq_.get(), cb_);
        void *tag;
        bool ok;
        while (!flag_)
        {
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<AsyncCall<AsyncRpcService>*>(tag)->Proceed();
        }
    }

private:
    std::string serverAddr_;
    std::unique_ptr<ServerCompletionQueue> cq_;
    AsyncRpcService service_;
    std::unique_ptr<Server> server_;
    RequestCallBack cb_;
    bool flag_ = false;
};

// AsyncServer<lobby::Lobby::AsyncService>;
// AsyncServer<room::Room::AsyncService>;
// AsyncServer<social::Social::AsyncService>;
// AsyncCall<lobby::Lobby::AsyncService>;
// AsyncCall<room::Room::AsyncService>;
// AsyncCall<social::Social::AsyncService>;


#endif