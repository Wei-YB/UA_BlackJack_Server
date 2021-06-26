#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <time.h>
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <thread>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif
#include "combat_typedef.h"
using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
// Service dependent
/*************************/
using ua_blackjack::Request;
using ua_blackjack::Response;
using ua_blackjack::UserService;
/*************************/

#include <memory>
class ClientForTestUser
{
public:
    typedef std::shared_ptr<ClientForTestUser> ptr;
    ClientForTestUser(std::shared_ptr<Channel> channel, BlackJackUID _uid)
        : stub_(UserService::NewStub(channel)), uid(_uid) {}
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
    void AsyncCompleteRpc();

    void askBettingMoney(const BlackJackUID uid);
    void askHitOrStand(const BlackJackUID uid);
    void askUpdate(const BlackJackUID uid, const BlackJackUID notifyUser);
    void askEnd(const BlackJackUID uid, FinalResultOfGame isWin);

private:
    // struct for keeping state and data information
    struct AsyncClientCall
    {
        Response reply;

        ClientContext context;

        Status status;

        std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
    };

    std::unique_ptr<UserService::Stub> stub_;
    CompletionQueue cq_;
    uint64_t stamp = 0;
    BlackJackUID uid;
};