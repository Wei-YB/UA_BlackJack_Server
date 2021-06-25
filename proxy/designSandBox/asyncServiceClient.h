#ifndef _ASYNCSERVICECALIENT_H_
#define _ASYNCSERVICECALIENT_H_

#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <mutex>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <thread>

#include <chrono>

#include "common.h"
#include "EventLoop.h"
#include "common.pb.h"
#include "lobby.grpc.pb.h"
#include "room.grpc.pb.h"
#include "social.grpc.pb.h"

typedef std::chrono::time_point<std::chrono::system_clock> time_point;
using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;

using common::Request;
using common::Response;


struct AsyncClientCall
{
    Request request;
    Response reply;
    ClientContext context;
    Status status;
    std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
};

class ServiceClient
{
public:
    ServiceClient(const std::string &serviceAddr) : serviceAddr_(serviceAddr) {}
    ~ServiceClient() {}
public:
    virtual int Call(const Request &) = 0;
    // 
    void AsyncCompleteRpc()
    {
        void *got_tag;
        bool ok = false;

        time_point deadline = std::chrono::system_clock::now() +
                                std::chrono::milliseconds(500);
        while (!stop_ && cq_.AsyncNext(&got_tag, &ok, deadline))
        {
            deadline = std::chrono::system_clock::now() +
                std::chrono::milliseconds(500);
            if (!ok)
            {
                continue;
            }
            //std::cout << "get response" << std::endl;
            // get a response, forward to the corresponding client
            AsyncClientCall *call = static_cast<AsyncClientCall*>(got_tag);
            // call back the Proxy
            if (responseCallBack_)
            {
                responseCallBack_(call->reply);
            }
            delete call;
        }
    }
    void SetResponseCallBack(const std::function<void(const Response&)> &cb) {responseCallBack_ = cb;}
    void StopClient() {stop_ = true;}
    
protected:
    std::string serviceAddr_;
    CompletionQueue cq_;
    std::function<void(const Response&)> responseCallBack_;
    bool stop_;
};

template<typename ConcreteService>
class ConcreteServiceClient : public ServiceClient
{
public:
    ConcreteServiceClient(const std::string &serviceAddr) 
            : ServiceClient(serviceAddr),
            stub_(ConcreteService::NewStub(CreateChannel(serviceAddr, InsecureChannelCredentials()))) 
    {}

public:
    int Call(const Request &request)
    {
        AsyncClientCall *call = new AsyncClientCall;
        call->request = request;
        call->response_reader =
            stub_->PrepareAsyncNotify(&call->context, call->request, &cq_);

        call->response_reader->StartCall();
        call->response_reader->Finish(&call->reply, &call->status, (void*)call);
        return 0;
    }
private:
    std::unique_ptr<typename ConcreteService::Stub> stub_;
};

#endif

// int main()
// {
//     // create three pipes for three async clients
//     int pipes[3][2];
//     for (int i = 0; i < 3; ++i)
//     {
//         pipe2(pipes[i], O_DIRECT | O_NONBLOCK);
//     }

//     Client<Lobby> lobbyClient(grpc::CreateChannel(lobbyAddress, grpc::InsecureChannelCredentials()), pipes[0][1]);
//     Client<Room> roomClient(grpc::CreateChannel(roomAddress, grpc::InsecureChannelCredentials()), pipes[1][1]);
//     Client<Social> socialClient(grpc::CreateChannel(socialAddress, grpc::InsecureChannelCredentials()), pipes[2][1]);

//     std::thread lobbyListener = std::thread(&Client<Lobby>::AsyncCompleteRpc, &lobbyClient);
//     std::thread roomListener = std::thread(&Client<Room>::AsyncCompleteRpc, &roomClient);
//     std::thread socialListener = std::thread(&Client<Social>::AsyncCompleteRpc, &socialClient);

//     lobbyListener.join();
//     roomListener.join();
//     socialListener.join();

//     return 0;
// }