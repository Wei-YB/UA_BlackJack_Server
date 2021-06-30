#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif
#include <sstream>
using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
// Service dependent
/*************************/
using ua_blackjack::GameService;
using ua_blackjack::ProxyService;
using ua_blackjack::Request;
using ua_blackjack::Response;
/*************************/
using namespace std;

enum
{
    STATUS_OK = 0,
    STATUS_ERROR = 1
};
class ServerImpl final
{
public:
    ~ServerImpl()
    {
        server_->Shutdown();
        // Always shutdown the completion queue after the server.
        cq_->Shutdown();
    }

    // There is no shutdown handling in this code.
    void Run()
    {
        std::string server_address("0.0.0.0:50052");

        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);

        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();
    }

public:
    // Class encompasing the state and logic needed to serve a request.
    class CallData
    {
    public:
        CallData(ProxyService::AsyncService *service, ServerCompletionQueue *cq)
            : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE)
        {
            Proceed();
        }

        void Proceed()
        {

            if (status_ == CREATE)
            {
                status_ = PROCESS;
                service_->RequestNotify(&ctx_, &request_, &responder_, cq_, cq_, this);
            }
            else if (status_ == PROCESS)
            {
                new CallData(service_, cq_);
                auto type = request_.requesttype();
                auto uid = request_.uid();
                auto stamp = request_.stamp();
                auto args = request_.args();

                for (auto arg : args)
                {
                    std::cout << arg << " ";
                }
                if (args[0] == "start")
                {
                    reply_.add_args("Bet");
                    reply_.add_args("12345");
                    std::cout << uid << " reply start successs" << std::endl;
                }
                else if (args[0] == "hit")
                {
                    reply_.add_args("Hit");
                    std::cout << uid << " reply hit successs" << std::endl;
                }
                std::cout << std::endl;
                reply_.set_status(0);
                reply_.set_uid(uid);
                reply_.set_stamp(stamp);

                status_ = FINISH;
                responder_.Finish(reply_, Status::OK, this); //运行这行，调用方就收到response
            }
            else
            {

                GPR_ASSERT(status_ == FINISH);

                delete this;
            }
        }

    private:
        ProxyService::AsyncService *service_;
        ServerCompletionQueue *cq_;
        ServerContext ctx_;

        Request request_;
        Response reply_;

        // The means to get back to the client.
        ServerAsyncResponseWriter<Response> responder_;

        enum CallStatus
        {
            CREATE,
            PROCESS,
            FINISH
        };
        CallStatus status_; // The current serving state.
    };

    // This can be run in multiple threads if needed.

public:
    std::unique_ptr<ServerCompletionQueue> cq_;

    ProxyService::AsyncService service_;
    std::unique_ptr<Server> server_;
};

class Client
{
public:
    Client(std::shared_ptr<Channel> channel)
        : stub_(GameService::NewStub(channel)) {}
    void printResponce(Response &responce)
    {
        std::cout << "status = " << responce.status() << " uid = " << responce.uid()
                  << " stamp = " << responce.stamp() << " args = " << std::endl;
        for (auto &s : responce.args())
        {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    int GetName(const int id)
    {
        Request request;
        request.set_uid(id);

        Response reply;

        ClientContext context;

        // The actual RPC.
        Status status = stub_->Notify(&context, request, &reply);

        // Act upon its status.
        if (status.ok())
        {
            return reply.uid();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return -1;
        }
    }
    void StartGame(const int rid)
    {
        Request request;
        request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_GAME_START);
        request.set_uid(rid);
        request.set_stamp(stamp++);
        for (int i = rid; i < rid + 5; i++)
        {
            std::stringstream ss;
            ss << i;
            request.add_args(ss.str());
        }
        Response reply;
        ClientContext context;
        // The actual RPC.
        Status status = stub_->Notify(&context, request, &reply);
        // Act upon its status.
        if (status.ok())
        {
            //printResponce(reply);
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
        }
    }
    uint64_t stamp = 0;

private:
    std::unique_ptr<GameService::Stub> stub_;
};

int main()
{
    ServerImpl server;
    server.Run(); //开启服务器

    std::string target_str = "localhost:50051";
    Client client(
        grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

    for (int64_t rid = 3000; rid < 1; rid += 5)
    {
        client.StartGame(rid);
    }
    new ServerImpl::CallData(&server.service_, server.cq_.get());
    void *tag; // uniquely identifies a request.
    bool ok;
    gpr_timespec deadline;
    deadline.clock_type = GPR_TIMESPAN;
    deadline.tv_sec = 0;
    deadline.tv_nsec = 100;
    while (true)
    {
        while (server.cq_->AsyncNext<gpr_timespec>(&tag, &ok, deadline) != grpc::CompletionQueue::NextStatus::GOT_EVENT)
        {
        }

        GPR_ASSERT(ok);
        static_cast<ServerImpl::CallData *>(tag)->Proceed();
    }
}