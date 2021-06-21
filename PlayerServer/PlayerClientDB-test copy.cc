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

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "player.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// Service dependent
/*************************/
using player::DatabaseService;
using player::Request;
using player::Response;
/*************************/

class Client {
public:
    Client(std::shared_ptr<Channel> channel) : stub_(DatabaseService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    Response RequestDB(const std::string name, const std::string passwd) {
        Request request;
        request.set_requesttype(Request::SIGNUP);
        request.set_uid(-1);
        request.set_stamp(12345);
        request.add_args(name);
        request.add_args(passwd);

        Response reply;

        ClientContext context;

        // The actual RPC.
        Status status = stub_->RequestDB(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            std::cout << "Got Reply" << std::endl;
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
        }

        return reply;
    }

private:
    std::unique_ptr<DatabaseService::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string addr = "9.134.69.87:50051";
    Client client(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials()));

    auto reply = client.RequestDB("Aaron", "Aaron_pass");
    std::cout << reply.uid() << " " << reply.status() << std::endl;

    reply = client.RequestDB("Owen", "Owen_pass");
    std::cout << reply.uid() << " " << reply.status() << std::endl;

    reply = client.RequestDB("Aaron", "Aaron_pass");
    std::cout << reply.uid() << " " << reply.status() << std::endl;

    return 0;
}
