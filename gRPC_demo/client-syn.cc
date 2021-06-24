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

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// Service dependent
/*************************/
using demo::GetNameService;
using demo::IDReply;
using demo::IDRequest;
/*************************/

class Client
{
public:
  Client(std::shared_ptr<Channel> channel)
      : stub_(GetNameService::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string GetName(const int id)
  {
    IDRequest request;
    request.set_id(id);

    IDReply reply;

    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetID(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
      return reply.name();
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<GetNameService::Stub> stub_;
};

int main(int argc, char **argv)
{
  std::string target_str = "localhost:50051";
  Client client(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  int id = 1234;
  std::string reply = client.GetName(id);
  std::cout << "Client received: " << reply << std::endl;

  return 0;
}
