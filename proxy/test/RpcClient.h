#ifndef _RPCCLIENT_H_
#define _RPCCLIENT_H_

#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <mutex>
#include <queue>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include <chrono>

#include "UA_BlackJack.pb.h"
#include "UA_BlackJack.grpc.pb.h"
#include "EventLoop.h"

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
    Request request;
    Response reply;
    time_point callTime_;
    time_point expiredTime_;
};

class ServiceClient
{
public:
    ServiceClient(const std::string &serviceName, const std::string &serviceAddr) 
    : serviceAddr_(serviceAddr), serviceName_(serviceName), requestCnt_(0) {}
    ~ServiceClient() {}
public:
    virtual int Call(const Request &) = 0;
    // 
    void AsyncCompleteRpc()
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
                if (call->expiredTime_ <= now)
                {
                    // std::cout << "Get response for call (uid = " 
                    //           << call->request_.uid() << "):" << std::endl;
                    // std::cout << "status: " << call->reply_.status() << std::endl;
                    bool isSignupLogn = call->request.requesttype() == Request::LOGIN || 
                                        call->request.requesttype() == Request::SIGNUP;
                    call->reply.set_uid(isSignupLogn ? ++requestCnt_ : call->request.uid());
                    call->reply.set_stamp(call->request.stamp());
                    call->reply.set_status(0);
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
    }
    void SetResponseCallBack(const std::function<void(const Response&)> &cb) {responseCallBack_ = cb;}
    void StopClient() {stop_ = true;}
    
protected:
    std::string serviceAddr_;
    std::string serviceName_;
    std::queue<std::shared_ptr<AsyncClientCall>> callQueue_;
    std::mutex callQueueLock_;
    std::function<void(const Response&)> responseCallBack_;
    int64_t requestCnt_;
    bool stop_;
};

template<typename ConcreteService>
class ConcreteServiceClient : public ServiceClient
{
public:
    ConcreteServiceClient(const std::string &serviceAddr) : ServiceClient(serviceAddr)
    {}

public:
    int Call(const Request &request)
    {
        std::shared_ptr<AsyncClientCall> call = std::make_shared<AsyncClientCall>();
        call->request = request;
        call->callTime_ = std::chrono::system_clock::now();
        call->expiredTime_ = call->callTime_ + std::chrono::milliseconds(10);
        {
            std::lock_guard<std::mutex> guard(callQueueLock_);
            callQueue_.push(call);
        }
        return 0;
    }
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