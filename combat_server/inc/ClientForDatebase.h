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
#include "spdlog/spdlog.h"
#include <sstream>
namespace ua_blackjack
{
    namespace Game
    {
        class ClientForDatebase
        {
        public:
            typedef std::shared_ptr<ClientForDatebase> ptr;

            void printResponce(Response &responce)
            {
                std::stringstream ss;
                ss << "ClientForDatebase status = " << responce.status() << " uid = " << responce.uid()
                   << " stamp = " << responce.stamp() << " args = ";
                for (auto &s : responce.args())
                {
                    ss << s << " ";
                }

                spdlog::info(ss.str());
            }
            void AsyncCompleteRpc();

            void matchEnd(const std::list<Player::ptr> playerList);
            void askPlayerNickName(const BlackJackUID uid);
            static ClientForDatebase &getInstance()
            {
                static ClientForDatebase instance(grpc::CreateChannel(
                    DatabaseServiceAddr, grpc::InsecureChannelCredentials()));
                return instance;
            }

        private:
            ClientForDatebase(std::shared_ptr<Channel> channel)
                : stub_(DatabaseService::NewStub(channel)) {}
            ~ClientForDatebase(){};
            ClientForDatebase(const ClientForDatebase &);
            ClientForDatebase &operator=(const ClientForDatebase &);

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
    }
}
