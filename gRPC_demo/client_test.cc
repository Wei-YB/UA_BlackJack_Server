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
#include <sys/wait.h>

#include <iostream>
#include <memory>
#include <string>

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
using demo::NameReply;
using demo::NameRequest;
/*************************/

class Client {
public:
    Client(std::shared_ptr<Channel> channel) : stub_(GetNameService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string GetName(const int id) {
        NameRequest request;
        request.set_id(id);

        NameReply reply;

        ClientContext context;

        // The actual RPC.
        Status status = stub_->GetName(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.name();
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<GetNameService::Stub> stub_;
};

void* handle(void* arg) {
    pthread_detach(pthread_self());
    std::string target_str = "localhost:50051";
    Client client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    // Client* client = (Client*)arg;
    int id = 1234;
    for (int i = 0; i < 200; ++i) {
        std::string reply = client.GetName(id);
    }
}

int main(int argc, char** argv) {
    for (int i = 0; i < 8; ++i) {
        pthread_t tid;
        pthread_create(&tid, nullptr, handle, nullptr);
    }

    while (true)
        ;

    return 0;
}
