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

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

// Service dependent
/*************************/
using demo::GameService;
using demo::Request;
using demo::Response;
/*************************/
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
    std::cout << "Server listening on " << server_address << std::endl;
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
        std::cout << "CREATE" << std::endl;
        status_ = PROCESS;
        service_->RequestNotify(&ctx_, &request_, &responder_, cq_, cq_, this);
      }
      else if (status_ == PROCESS)
      {
        std::cout << "PROCESS" << std::endl;
        new CallData(service_, cq_);
        auto type = request_.requesttype();
        auto uid = request_.uid();
        auto stamp = request_.stamp();
        auto args = request_.args();

        reply_.set_status(STATUS_OK);
        reply_.set_uid(uid + 100);
        reply_.set_stamp(stamp);
        //reply_.set_args(args);

        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this); //运行这行，调用方就收到response
      }
      else
      {
        std::cout << "FINISH" << std::endl;
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
