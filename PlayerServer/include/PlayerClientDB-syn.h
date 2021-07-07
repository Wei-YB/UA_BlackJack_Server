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

#ifndef PLAYER_CLIENT_DB_SYN_H
#define PLAYER_CLIENT_DB_SYN_H

#include <grpcpp/grpcpp.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "global.h"

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

namespace ua_blackjack {
namespace player_client {
class PlayerClient {
public:
    PlayerClient(std::shared_ptr<Channel> channel) : stub_(DatabaseService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    Response RequestDB(Request& request);

    void CheckStatus(const Status& status);
    int Name2Uid(const std::string& name);
    std::string Uid2Name(int uid);
    std::string GetScore(int uid);

private:
    std::unique_ptr<DatabaseService::Stub> stub_;

    std::unordered_map<int, std::string> name_cache_;
    // std::unordered_map<int, std::string> score_cache_;
};
}  // namespace player_client
}  // namespace ua_blackjack

#endif