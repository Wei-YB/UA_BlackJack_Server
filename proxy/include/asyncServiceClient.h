#ifndef _ASYNCSERVICECALIENT_H_
#define _ASYNCSERVICECALIENT_H_

#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <mutex>
#include <queue>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <thread>

#include <chrono>

#include "UA_BlackJack.pb.h"
#include "UA_BlackJack.grpc.pb.h"
#include "EventLoop.h"
#include "common.h"

typedef std::chrono::time_point<std::chrono::system_clock> time_point;

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;

using ua_blackjack::Response;
using ua_blackjack::Request;
using ua_blackjack::LobbyService;
using ua_blackjack::GameService;
using ua_blackjack::SocialService;

struct AsyncClientCall
{
    ClientContext context;
    std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
    Request request;
    Response reply;
    time_point callTime;
    time_point expiredTime;
};

class ServiceClient
{
public:
    ServiceClient(const std::string &serviceName, 
                  const std::string &serviceAddr) 
                  : serviceName_(serviceName),
                    serviceAddr_(serviceAddr) {}
    ~ServiceClient() {}
public:
    virtual int Call(const Request &) = 0;
    // 
    void AsyncCompleteRpc()
    {
#if (DEBUG_MODE == 1)
        AsyncCompleteRpc_stand_alone_();
#else
        AsyncCompleteRpc_();
#endif

    }
    void SetResponseCallBack(const std::function<void(const Response&)> &cb) {responseCallBack_ = cb;}
    void StopClient() {stop_ = true;}
    
private:
    void AsyncCompleteRpc_()
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

#if (DEBUG_MODE == 1)
    void AsyncCompleteRpc_stand_alone_()
    {
        while (!stop_)
        {
            // check the call queue every 1 ms
            usleep(1000);
            time_point now = std::chrono::system_clock::now();
            {   // start of critical section
            std::lock_guard<std::mutex> guard(callQueueLock_);
            while (!callQueue_.empty())
            {
                std::shared_ptr<AsyncClientCall> call = callQueue_.front();
                if (call->expiredTime <= now)
                {
                    std::cout << "Get response for call (uid = " 
                              << call->request.uid() << "):" << std::endl;
                    std::cout << "status: " << call->reply.status() << std::endl;
                    bool isSignupLogn = call->request.requesttype() == Request::LOGIN || 
                                        call->request.requesttype() == Request::SIGNUP;
                    call->reply.set_uid(isSignupLogn ? ++requestCnt_ : call->request.uid());
                    call->reply.set_stamp(call->request.stamp());
                    call->reply.set_status(0);
                    call->reply.add_args(serviceName_ + " successfully handle your requets.");
                    if (responseCallBack_)
                    {
                        std::cout << "call proxy to handle this response." << std::endl;
                        responseCallBack_(call->reply);
                    }
                    callQueue_.pop();
                }
                else
                {
                    break;
                }
            }   // end of critical section
            }
        }   
    }
#endif

protected:
    std::string serviceName_;
    std::string serviceAddr_;
    CompletionQueue cq_;
    std::function<void(const Response&)> responseCallBack_;
    bool stop_;
#if (DEBUG_MODE == 1)
    std::queue<std::shared_ptr<AsyncClientCall>> callQueue_;
    std::mutex callQueueLock_;
    int64_t requestCnt_;
#else

#endif
};

template<typename ConcreteService>
class ConcreteServiceClient : public ServiceClient
{
public:
#if (DEBUG_MODE == 0)
    ConcreteServiceClient(const std::string &serviceName, 
                          const std::string &serviceAddr) 
                        : ServiceClient(serviceName, serviceAddr),
                        stub_(ConcreteService::NewStub(CreateChannel(serviceAddr, InsecureChannelCredentials()))) 
    {}
#else
    ConcreteServiceClient(const std::string &serviceName, 
                          const std::string &serviceAddr) 
                        : ServiceClient(serviceName, serviceAddr) 
    {}
#endif

public:
    int Call(const Request &request)
    {  
#if (DEBUG_MODE == 0)
        std::cout << "remote mode call." << std::endl;
        AsyncClientCall *call = new AsyncClientCall;
        call->request = request;    
        call->response_reader =
            stub_->PrepareAsyncNotify(&call->context, call->request, &cq_);

        call->response_reader->StartCall();
        call->response_reader->Finish(&call->reply, &call->status, (void*)call);
#else
        std::cout << "stand alone mode call." << std::endl;
        std::shared_ptr<AsyncClientCall> call = std::make_shared<AsyncClientCall>();
        call->request = request;
        call->callTime = std::chrono::system_clock::now();
        call->expiredTime = call->callTime + std::chrono::milliseconds(10);
        {
            std::lock_guard<std::mutex> guard(callQueueLock_);
            callQueue_.push(call);
        }
#endif
        return 0;
    }
private:
#if (DEBUG_MODE == 0)
    std::unique_ptr<typename ConcreteService::Stub> stub_;
#endif
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