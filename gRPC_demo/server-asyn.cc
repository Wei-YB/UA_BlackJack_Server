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
using demo::GetNameService;
<<<<<<< HEAD
using demo::IDReply;
using demo::IDRequest;
/*************************/

class ServerImpl final
{
public:
  ~ServerImpl()
  {
=======
using demo::NameReply;
using demo::NameRequest;
/*************************/

class ServerImpl final {
 public:
  ~ServerImpl() {
>>>>>>> de988bdddb4acfb3a9a210da2348bcff788ee20e
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
  }

  // There is no shutdown handling in this code.
<<<<<<< HEAD
  void Run()
  {
=======
  void Run() {
>>>>>>> de988bdddb4acfb3a9a210da2348bcff788ee20e
    std::string server_address("0.0.0.0:50051");

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    HandleRpcs();
  }

<<<<<<< HEAD
private:
  // Class encompasing the state and logic needed to serve a request.
  class CallData
  {
  public:
    CallData(GetNameService::AsyncService *service, ServerCompletionQueue *cq)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE)
    {
      Proceed();
    }

    void Proceed()
    {
      if (status_ == CREATE)
      {
        status_ = PROCESS;
        service_->RequestGetID(&ctx_, &request_, &responder_, cq_, cq_, this); //异步监听请求
      }
      else if (status_ == PROCESS)
      {
=======
 private:
  // Class encompasing the state and logic needed to serve a request.
  class CallData {
   public:
    CallData(GetNameService::AsyncService* service, ServerCompletionQueue* cq)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
      Proceed();
    }

    void Proceed() {
      if (status_ == CREATE) {
        status_ = PROCESS;
        service_->RequestGetName(&ctx_, &request_, &responder_, cq_, cq_, this);
      } else if (status_ == PROCESS) {
>>>>>>> de988bdddb4acfb3a9a210da2348bcff788ee20e
        new CallData(service_, cq_);

        std::string name = "Aaron ";
        // The actual processing.
        reply_.set_name(name);

        // And we are done! Let the gRPC runtime know we've finished
        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this);
<<<<<<< HEAD
      }
      else
      {
=======
      } else {
>>>>>>> de988bdddb4acfb3a9a210da2348bcff788ee20e
        GPR_ASSERT(status_ == FINISH);

        delete this;
      }
    }

<<<<<<< HEAD
  private:
    GetNameService::AsyncService *service_;
    ServerCompletionQueue *cq_;
    ServerContext ctx_;

    IDRequest request_;
    IDReply reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<IDReply> responder_;

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
=======
   private:
    GetNameService::AsyncService* service_;
    ServerCompletionQueue* cq_;
    ServerContext ctx_;

    NameRequest request_;
    NameReply reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<NameReply> responder_;

    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.
  };

  // This can be run in multiple threads if needed.
  void HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service_, cq_.get());
    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
      GPR_ASSERT(cq_->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<CallData*>(tag)->Proceed();
>>>>>>> de988bdddb4acfb3a9a210da2348bcff788ee20e
    }
  }

  std::unique_ptr<ServerCompletionQueue> cq_;
  GetNameService::AsyncService service_;
  std::unique_ptr<Server> server_;
};

<<<<<<< HEAD
int main(int argc, char **argv)
{
=======
int main(int argc, char** argv) {
>>>>>>> de988bdddb4acfb3a9a210da2348bcff788ee20e
  ServerImpl server;
  server.Run();

  return 0;
}
