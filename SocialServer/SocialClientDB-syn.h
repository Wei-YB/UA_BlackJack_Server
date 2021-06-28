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

#ifndef SOCIAL_CLIENT_DB_SYN_H
#define SOCIAL_CLIENT_DB_SYN_H

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "UA_BlackJack.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// Service dependent
/*************************/
using ua_blackjack::DatabaseService;
using ua_blackjack::Request;
using ua_blackjack::Response;
/*************************/

class Client {
public:
    Client(std::shared_ptr<Channel> channel) : stub_(DatabaseService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    Response RequestDB(Request& request);

    void CheckStatus(const Status& status);
    int Name2Uid(ClientContext& context, const std::string& name);
    std::string Uid2Name(ClientContext& context, int uid);

private:
    std::unique_ptr<DatabaseService::Stub> stub_;
};

#endif