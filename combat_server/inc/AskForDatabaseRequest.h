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
using ua_blackjack::DatabaseService;
using ua_blackjack::Request;
using ua_blackjack::Response;
/*************************/
#include <memory>
#include "Player.h"
class ClientForDatebase
{
public:
    typedef std::shared_ptr<ClientForDatebase> ptr;
    ClientForDatebase(std::shared_ptr<Channel> channel)
        : stub_(DatabaseService::NewStub(channel)) {}
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

    void matchEnd(const std::list<Player::ptr> playerList);

private:
    // struct for keeping state and data information
    struct AsyncClientCall
    {
        Response reply;

        ClientContext context;

        Status status;

        std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
    };

    std::unique_ptr<DatabaseService::Stub> stub_;
    CompletionQueue cq_;
    uint64_t stamp = 0;
};