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
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif
using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

// Service dependent
/*************************/
using ua_blackjack::GameService;
using ua_blackjack::Request;
using ua_blackjack::Response;
using ua_blackjack::UserService;
/*************************/
using namespace std;
class Client
{
public:
  Client(std::shared_ptr<Channel> channel)
      : stub_(GameService::NewStub(channel)) {}
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
  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  int GetName(const int id)
  {
    Request request;
    request.set_uid(id);

    Response reply;

    ClientContext context;

    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
      return reply.uid();
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return -1;
    }
  }
  void StartGame(const int uid)
  {
    Request request;
    request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_GAME_START);
    request.set_uid(uid);
    request.set_stamp(stamp++);
    Response reply;
    ClientContext context;
    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);
    // Act upon its status.
    if (status.ok())
    {
      printResponce(reply);
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
    }
  }
  uint64_t stamp = 0;

private:
  std::unique_ptr<GameService::Stub> stub_;
};
// Logic and data behind the server's behavior.
class GetNameServiceImpl final : public UserService::Service
{
  Status Notify(ServerContext *context, const Request *request,
                Response *reply) override
  {
    auto requestType = request->requesttype();
    auto uid = request->uid();
    auto stamp = request->stamp();
    auto args = request->args();
    if (requestType == ua_blackjack::Request_RequestType::Request_RequestType_NOTIFY_USER)
    {
      if (args[0] == "start")
      {
        cout << "bet money please" << endl;
        string money;
        cin >> money;

        reply->add_args(money);
        reply->set_stamp(stamp);
        reply->set_uid(uid);
        reply->set_status(0);
      }
      else if (args[0] == "hit")
      {
        cout << "Hit Or Stand(Enter Hit Or Stand)" << endl;
        string sss;
        cin >> sss;

        reply->add_args(sss);
        reply->set_stamp(stamp);
        reply->set_uid(uid);
        reply->set_status(0);
      }
      else if (args[0] == "update")
      {
        for (auto &s : args)
        {
          std::cout << s << std::endl;
        }
        reply->add_args("update");
        reply->set_stamp(stamp);
        reply->set_uid(uid);
        reply->set_status(0);
      }
      else if (args[0] == "end")
      {
        for (auto &s : args)
        {
          std::cout << s << std::endl;
        }
        reply->add_args("end");
        reply->set_stamp(stamp);
        reply->set_uid(uid);
        reply->set_status(0);
      }
    }

    return Status::OK;
  }
};
std::shared_ptr<Server> server2;
void RunServer(string port)
{
  string sk = "0.0.0.0:" + port;
  std::string server_address(sk);
  GetNameServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cout << "please enter port" << endl;
    return -1;
  }

  std::string target_str = "localhost:50051";
  Client client(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  int64_t uid = atoi(argv[1]);
  client.StartGame(uid++);
  cout << "join room success" << endl;
  RunServer(argv[1]); //运行服务器
  return 0;
}
