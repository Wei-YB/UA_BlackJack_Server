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
#include "../net/EventLoop.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using proxy::Proxy;
using common::Response;
using common::Request;


// this pipe should operate on package and non-blocking modes 
class AsyncCall
{
public:
    // There is a flaw in the constructor; we explicitly label the uid type 
    // as int64_t for the template parameter of Net::HandlerManager, which 
    // is fragile since the uid type would probably changed in the future.
    AsyncCall(Proxy::AsyncService *service, ServerCompletionQueue *cq,
                Net::HandlerManager<int64_t> *clientManager, int asyncNotifyPipe)
            : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE), 
            asyncNotifyPipe_(asyncNotifyPipe), clientManager_(clientManager)
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
            new AsyncCall(service_, cq_, clientManager_, asyncNotifyPipe_);
            // check the validity of the request
            if (request_.requesttype() != common::Request_RequestType_NOTIFY_USER)
            {
                // drop the request, and return fail to caller
                returnFailureResponse("Request type not allowed.");
                return;
            }
            int64_t uid = request_.uid();
            // send the request message to corresponding client
            Net::EventsHandler *client = clientManager_->find(uid);
            if (!client)
            {
                returnFailureResponse("User do not exist or offline.");
                return;
            }
            if (0 > client->notifyInAdvance(asyncNotifyPipe_, EPOLLIN, (void*)this))
            {
                returnFailureResponse("Fail to forward to client.");
                return;
            }
            // forward this request to epoll of the main thread
            int ret = write(asyncNotifyPipe_, (void*)&uid, sizeof(uid));
            if (ret < sizeof(uid))
            {
                returnFailureResponse("Fail to forward to client.");
                return;
            }
            status_ = FORWARD;
        }
        // this case is dealed in main thread eventloop
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

    inline Request &getRequest()
    {
        return request_;
    }

    void setReply(const Response &response)
    {
        reply_ = response;
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
    int asyncNotifyPipe_;
    Net::HandlerManager<int64_t> *clientManager_;
};


class ProxyServerImpl final {
public:
    ProxyServerImpl(const std::string &serverAddress, int asyncNotifyPipe, 
                    Net::HandlerManager<int64_t> *clientManager)
        : serverAddress_(serverAddress), asyncNotifyPipe_(asyncNotifyPipe),
            clientManager_(clientManager) {}

    ~ProxyServerImpl()
    {
        server_->Shutdown();
        cq_->Shutdown();
        close(asyncNotifyPipe_);
    }

    void Run() 
    {   
        ServerBuilder builder;
        builder.AddListeningPort(serverAddress_, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);

        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << serverAddress_ << std::endl;
    
        HandleRpcs();
    }

private:
    void HandleRpcs()
    {
        new AsyncCall(&service_, cq_.get(), clientManager_, asyncNotifyPipe_);
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
    int asyncNotifyPipe_;
    Net::HandlerManager<int64_t> *clientManager_;
};
