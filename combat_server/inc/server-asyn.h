/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#pragma once
#include "combat_typedef.h"
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif
#include "demo.pb.h"
#include "spdlog/spdlog.h"
int createstEnv_t(BlackJackRoomID roomID, UidList &uids);
void *createOneGame(void *arg);
void *waitingSignalFromOtherModule(void *arg);
void *recoveryUnusedCo(void *arg); //回收协程的协程
using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

// Service dependent
/*************************/
using ua_blackjack::GameService;
using ua_blackjack::Request;
using ua_blackjack::Response;
/*************************/

namespace ua_blackjack
{
  namespace Game
  {
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
        std::string server_address("0.0.0.0:50051");

        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);

        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();
        spdlog::info("Server listening on {0:d}", server_address);
      }

    public:
      // Class encompasing the state and logic needed to serve a request.
      class CallData
      {
      public:
        CallData(GameService::AsyncService *service, ServerCompletionQueue *cq)
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
            reply_.set_status(0);
            reply_.set_uid(uid);
            reply_.set_stamp(stamp);
            spdlog::info("type = {0:d} uid = {1:d} stamp = {2:d}", type, uid, stamp);
            if (type == ua_blackjack::Request_RequestType::Request_RequestType_GAME_START) //真正的创建房间code
            {
              UidList uids;
              BlackJackRoomID roomid = atoi(args[0].c_str());

              spdlog::info("Create Room Ask come");
              for (int i = 1; i < args.size(); i++)
              {
                uids.push_back(atoi(args[i].c_str()));
              }

              createstEnv_t(roomid, uids);
              uids.clear();
            }
            /*
        if (type == ua_blackjack::Request_RequestType::Request_RequestType_GAME_START) //创建房间
        {
          static uint64_t roomMemberSize = 0;
          static ua_blackjack::GameServiceNamespace::UidList uids;
          static ua_blackjack::GameServiceNamespace::BlackJackRoomID roomid;
          spdlog::info("ONE PEROSON COME IN");
          roomMemberSize++;
          roomMemberSize %= 4;
          uids.push_back(uid);
          if (roomMemberSize == 0) //凑够2个人 demo，后面不需要这些，而是由lobby分配人过来
          {
            spdlog::info("CREATE ROOM");
            createstEnv_t(roomid++, uids);
            uids.clear();
          }
        }
        */
            else if (type == ua_blackjack::Request_RequestType::Request_RequestType_LEAVE_ROOM) //退出房间
            {
              if (auto playerPtr = playerHashMap[uid].lock())
              {
                spdlog::info("{0:d} quit by itself", uid);
                playerPtr->quit(); //托管
              }
              else
              {
                spdlog::error("Quit error uid {0:d}   not existed in any room", uid);
              }
            }
            else if (type == ua_blackjack::Request_RequestType::Request_RequestType_DOUBLE) //双倍
            {
              if (auto playerPtr = playerHashMap[uid].lock())
              {
                spdlog::info("{0:d} double by itself", uid);

                playerPtr->bettingMoney *= 2;
              }
              else
              {
                spdlog::error("Double error uid {0:d}   not existed in any room", uid);
              }
            }
            else if (type == ua_blackjack::Request_RequestType::Request_RequestType_SURRENDER) //投降
            {
              if (auto playerPtr = playerHashMap[uid].lock())
              {
                spdlog::info("{0:d} surrond by itself", uid);
                playerPtr->bettingMoney *= 2;
              }
              else
              {
                spdlog::error("Surrond error uid {0:d}   not existed in any room", uid);
              }
            }

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
        GameService::AsyncService *service_;
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
      void HandleRpcs()
      {
        // Spawn a new CallData instance to serve new clients.
        new CallData(&service_, cq_.get());
        void *tag; // uniquely identifies a request.
        bool ok;
        while (true)
        {

          GPR_ASSERT(cq_->Next(&tag, &ok));
          GPR_ASSERT(ok);

          static_cast<CallData *>(tag)->Proceed();
        }
      }

    public:
      std::unique_ptr<ServerCompletionQueue> cq_;

      GameService::AsyncService service_;
      std::unique_ptr<Server> server_;
    };
  }
}