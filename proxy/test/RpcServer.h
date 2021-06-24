#ifndef _RPCSERVER_H_
#define _RPCSERVER_H_

#include <memory>
#include <string>
#include <thread>
#include <functional>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "lobby.grpc.pb.h"
#include "room.grpc.pb.h"
#include "social.grpc.pb.h"
#include "common.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using common::Request;
using common::Response;



template<typename AsyncService>
class AsyncCall
{
    typedef std::function<void(const Request&, Response&)> RequestCallBack;
public:
    AsyncCall(AsyncService *service, 
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
            new AsyncCall<AsyncService>(service_, cq_, cb_);
            //new AsyncCall(service_, cq_, cb_);
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
    AsyncService *service_;
    //lobby::Lobby::AsyncService *service_;
    ServerCompletionQueue *cq_;
    ServerContext ctx_;
    Request request_;
    Response response_;
    ServerAsyncResponseWriter<Response> responder_;
    enum CallStatus {CREATE, PROCESS, FORWARD, FINISH};
    CallStatus status_;
    RequestCallBack cb_;
};

template<typename AsyncService>
class AsyncServer final {
    typedef std::function<void(const Request&, Response&)> RequestCallBack;
public:
    AsyncServer(const std::string &serverAddr, 
                const RequestCallBack &cb) 
                : serverAddr_(serverAddr),
                cb_(cb) {}
    ~AsyncServer() {server_->Shutdown(); cq_->Shutdown();}

    void Run()
    {
        ServerBuilder builder;
        builder.AddListeningPort(serverAddr_, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);
        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << serverAddr_ << std::endl;
        HandleRpcs();
    }

private:
    void HandleRpcs()
    {
        new AsyncCall<AsyncService>(&service_, cq_.get(), cb_);
        //new AsyncCall(&service_, cq_.get(), cb_);
        void *tag;
        bool ok;
        while (true)
        {
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            //static_cast<AsyncCall<AsyncService>*>(tag)->Proceed();
            static_cast<AsyncCall<AsyncService>*>(tag)->Proceed();
        }
    }

private:
    std::string serverAddr_;
    std::unique_ptr<ServerCompletionQueue> cq_;
    AsyncService service_;
    //lobby::Lobby::AsyncService service_;
    std::unique_ptr<Server> server_;
    RequestCallBack cb_;
};

// AsyncServer<lobby::Lobby::AsyncService>;
// AsyncServer<room::Room::AsyncService>;
// AsyncServer<social::Social::AsyncService>;
// AsyncCall<lobby::Lobby::AsyncService>;
// AsyncCall<room::Room::AsyncService>;
// AsyncCall<social::Social::AsyncService>;


#endif