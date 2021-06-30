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
#include "log.h"

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



class ServiceClient
{
public:
    ServiceClient(const std::string &serviceName, 
                  const std::string &serviceAddr) 
                  : serviceName_(serviceName),
                    serviceAddr_(serviceAddr),
                    stop_(false) {}
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
#if (DEBUG_MODE == 0)
    void AsyncCompleteRpc_()
    {
        void *got_tag;
        bool ok = false;
        // std::cout << "enter " << serviceName_ << " thread" << std::endl;
        logger_ptr->info("In {0} thread: Thread started.", serviceName_);
        time_point deadline = std::chrono::system_clock::now() +
                                std::chrono::milliseconds(500);
        grpc::CompletionQueue::NextStatus sta;
        while (!stop_ && (sta = cq_.AsyncNext(&got_tag, &ok, deadline)))
        {
            AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
            if (!ok || sta == grpc::CompletionQueue::NextStatus::TIMEOUT)
            {
                continue;
            }
            // get a response, forward to the corresponding client
            // int64_t callKey = (int64_t)got_tag;
            // std::shared_ptr<AsyncClientCall> call;
            // {
            // std::lock_guard<std::mutex> guard(asyncCallsLock_);
            // call = asyncCalls_[callKey];
            // asyncCalls_.erase(callKey);
            // }
            
            // call back the Proxy
            if (responseCallBack_ && call->status.ok())
            {
                responseCallBack_(call->reply);
            }
            else 
            {
                logger_ptr->info("In {} thread: response not ok", serviceName_);
                Response res;
                res.set_status(-1);
                res.set_uid(call->request.uid());
                res.set_stamp(call->request.stamp());
                responseCallBack_(call->reply);
            }
            delete call;
            deadline = std::chrono::system_clock::now() +
                        std::chrono::milliseconds(500);
        }
        logger_ptr->info("In {} thread: leaving the thread", serviceName_);
    }

#else
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
                    if (call->request.requesttype() == Request::LOGIN 
                        || call->request.requesttype() == Request::SIGNUP)
                    {
                        call->reply.set_uid(++requestCnt_);
                        logger_ptr->info("In gRPC client {0} thread: this is a login request, set its uid to {1}", serviceName_, requestCnt_);
                    }
                    else
                    {
                        call->reply.set_uid(call->request.uid());
                        logger_ptr->info("In gRPC client {0} thread: this is a normal request, simply echo back", serviceName_);
                    }
                    call->reply.set_stamp(call->request.stamp());
                    call->reply.set_status(-1);
                    call->reply.add_args(serviceName_ + " successfully handle your requets.");
                    if (responseCallBack_)
                    {
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
        logger_ptr->info("In gRPC client {0} thread: leaving the thread", serviceName_);
    }
#endif

protected:
    struct AsyncClientCall
    {
        Response reply;
        ClientContext context;
        Status status;
        std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
        Request request;
        time_point callTime;
        time_point expiredTime;
    };

    std::string serviceName_;
    std::string serviceAddr_;
    CompletionQueue cq_;
    std::mutex asyncCallsLock_;
    std::unordered_map<int64_t, std::shared_ptr<AsyncClientCall>> asyncCalls_;
    std::function<void(const Response&)> responseCallBack_;
    bool stop_;
#if (DEBUG_MODE == 1)
    std::queue<std::shared_ptr<AsyncClientCall>> callQueue_;
    std::mutex callQueueLock_;
    int64_t requestCnt_ = 0;
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
        logger_ptr->info("In main thread: client (uid: {0}) call {1}", request.uid(), serviceName_);
        //std::shared_ptr<AsyncClientCall> call = std::make_shared<AsyncClientCall>();
        AsyncClientCall *call =  new AsyncClientCall;
        //asyncCalls_.emplace((int64_t)call.get(), call);
        call->request = request;    
        call->response_reader =
            stub_->PrepareAsyncNotify(&call->context, request, &cq_);

        call->response_reader->StartCall();
        call->response_reader->Finish(&call->reply, &call->status, (void*)call);
#else
        // Response res;
        // if (request.requesttype() == Request::LOGIN 
        //     || request.requesttype() == Request::SIGNUP)
        // {
        //     std::cout << "this is a login response." << std::endl;
        //     res.set_uid(++requestCnt_);
        //     std::cout << "setting its uid to " << requestCnt_ << std::endl;
        // }
        // else
        // {
        //     res.set_uid(request.uid());
        //     std::cout << "setting the response uid to " << request.uid();
        // }
        // res.set_stamp(request.stamp());
        // res.set_status(0);
        // res.add_args(serviceName_ + " successfully handle your requets.");
        // if (responseCallBack_)
        // {
        //     std::cout << "call proxy to handle this response." << std::endl;
        //     responseCallBack_(res);
        // }
        std::shared_ptr<AsyncClientCall> call = std::make_shared<AsyncClientCall>();
        call->request = request;
        call->callTime = std::chrono::system_clock::now();
        call->expiredTime = call->callTime;// + std::chrono::milliseconds(10);
        {
            std::lock_guard<std::mutex> guard(callQueueLock_);
            logger_ptr->info("push request (type: {0}, from: uid = {1}) into call queue.", request.requesttype(), request.uid());
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
