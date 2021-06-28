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
using ua_blackjack::LobbyService;
using ua_blackjack::Request;
using ua_blackjack::Response;
/*************************/
#include <memory>
#include "Player.h"
#include "spdlog/spdlog.h"
#include <sstream>
class ClientForLobby
{
public:
    typedef std::shared_ptr<ClientForLobby> ptr;

    void printResponce(Response &responce)
    {
        std::stringstream ss;
        ss << "status = " << responce.status() << " uid = " << responce.uid()
           << " stamp = " << responce.stamp() << " args = ";
        for (auto &s : responce.args())
        {
            ss << s << " ";
        }

        spdlog::info(ss.str());
    }
    void AsyncCompleteRpc();

    void matchEnd(const BlackJackRoomID roomid);

    static ClientForLobby &getInstance()
    {
        static ClientForLobby instance(grpc::CreateChannel(
            LobbyServiceAddr, grpc::InsecureChannelCredentials()));
        return instance;
    }

private:
    ClientForLobby(std::shared_ptr<Channel> channel)
        : stub_(LobbyService::NewStub(channel)) {}
    ~ClientForLobby(){};
    ClientForLobby(const ClientForLobby &);
    ClientForLobby &operator=(const ClientForLobby &);
    // struct for keeping state and data information
    struct AsyncClientCall
    {
        Response reply;

        ClientContext context;

        Status status;

        std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
    };

    std::unique_ptr<LobbyService::Stub> stub_;
    CompletionQueue cq_;
    uint64_t stamp = 0;
};