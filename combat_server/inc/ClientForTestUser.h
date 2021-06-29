#pragma once
#include "Player.h"
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
#include "spdlog/spdlog.h"
#include <sstream>
using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
// Service dependent
/*************************/
using ua_blackjack::ProxyService;
using ua_blackjack::Request;
using ua_blackjack::Response;
/*************************/

#include <memory>
namespace ua_blackjack
{
    namespace Game
    {
        class ClientForTestUser
        {
        public:
            typedef std::shared_ptr<ClientForTestUser> ptr;
            static ClientForTestUser &getInstance()
            {
                static ClientForTestUser instance(grpc::CreateChannel(
                    ProxyServiceAddr, grpc::InsecureChannelCredentials()));
                return instance;
            }
            void printResponce(Response &responce)
            {
                std::stringstream ss;
                ss << "ClientForTestUser status = " << responce.status() << " uid = " << responce.uid()
                   << " stamp = " << responce.stamp() << " args = ";
                for (auto &s : responce.args())
                {
                    ss << s << " ";
                }

                spdlog::info(ss.str());
            }
            void AsyncCompleteRpc();

            void askBettingMoney(const BlackJackUID uid);
            void askHitOrStand(const BlackJackUID uid);
            void askUpdate(const BlackJackUID uid, const BlackJackUID notifyUser);
            void askUpdate(const BlackJackUID uid, const BlackJackUID notifyUser, bool showDealerHidePker);
            void askUpdate(const std::list<ua_blackjack::Game::Player::ptr> &playerList, const BlackJackUID notifyUser);
            void askEnd(const BlackJackUID uid, FinalResultOfGame isWin);

        private:
            ClientForTestUser(std::shared_ptr<Channel> channel)
                : stub_(ProxyService::NewStub(channel)) {}
            ~ClientForTestUser(){};
            ClientForTestUser(const ClientForTestUser &);
            ClientForTestUser &operator=(const ClientForTestUser &);
            // struct for keeping state and data information
            struct AsyncClientCall
            {
                Response reply;

                ClientContext context;

                Status status;

                std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
            };

            std::unique_ptr<ProxyService::Stub> stub_;
            CompletionQueue cq_;
            uint64_t stamp = 0;
        };
    }
}