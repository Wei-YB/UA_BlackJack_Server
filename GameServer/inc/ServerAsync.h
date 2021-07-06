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
#include "GameProcess.h"
#include "MycondintonCo.h"
#include "combat_typedef.h"
#include <memory>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "UA_BlackJack.grpc.pb.h"
#endif
#include "UA_BlackJack.pb.h"
#include "spdlog/spdlog.h"
#include "ServiceCallback.h"
void handelGrpc(ua_blackjack::Request &request);
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
    class CallData
    {
    public:
      typedef std::function<void(Request &request, Response &responce)> CallbackFunc;
      CallData(GameService::AsyncService *service, ServerCompletionQueue *cq)
          : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE)
      {
        Proceed();
      }

      void Proceed();

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
  }
}
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
      void Run();

    public:
      // Class encompasing the state and logic needed to serve a request.

      // This can be run in multiple threads if needed.
      void HandleRpcs();

    public:
      std::unique_ptr<ServerCompletionQueue> cq_;

      GameService::AsyncService service_;
      std::unique_ptr<Server> server_;
    };
  }
}