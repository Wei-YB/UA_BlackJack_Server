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

#include "PlayerClientDB-syn.h"

using namespace ua_blackjack::player_client;

void PlayerClient::CheckStatus(const Status& status) {
    // Act upon its status.
    if (status.ok()) {
        logger->info("gRpc Got Reply");
    } else {
        logger->error("gRpc Request Error {0} : {1}", status.error_code(), status.error_message());
    }
}

int PlayerClient::Name2Uid(const std::string& name) {
    ClientContext context;
    Response reply;
    Request request;

    request.set_requesttype(Request::GET_UID);
    request.set_uid(-1);
    request.set_stamp(std::time(nullptr));
    request.add_args(name);

    logger->info("Send GET_UID to Database Server");
    CheckStatus(stub_->Notify(&context, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());

    return reply.uid();
}

std::string PlayerClient::GetScore(int uid) {
    ClientContext context;
    Response reply;
    Request request;

    request.set_requesttype(Request::GET_SCORE);
    request.set_uid(uid);
    request.set_stamp(std::time(nullptr));

    logger->info("Send GET_SCORE to Database Server");
    CheckStatus(stub_->Notify(&context, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());

    return reply.args()[0];
}

std::string PlayerClient::Uid2Name(int uid) {
    ClientContext context;
    Response reply;
    Request request;

    request.set_requesttype(Request::GET_NAME);
    request.set_uid(uid);
    request.set_stamp(std::time(nullptr));

    CheckStatus(stub_->Notify(&context, request, &reply));
    return reply.args()[0];
}

Response PlayerClient::RequestDB(Request& request) {
    logger->info("Received request, type: {0}, uid: {1}, stamp: {2}", request.requesttype(), request.uid(),
                 request.stamp());

    Response reply;

    ClientContext context;

    int uid = request.uid();

    logger->info("Send request to Database Server");
    // The actual RPC.
    CheckStatus(stub_->Notify(&context, request, &reply));

    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());

    if (request.requesttype() == Request::RANK_ME) {
        std::string score = GetScore(uid);
        reply.add_args(score);
    } else if (request.requesttype() == Request::RANK_TOP) {
        int sz = reply.args().size();
        for (int i = 0; i < sz; ++i) {
            int uid = atoi(reply.args()[i].c_str());
            std::string name = Uid2Name(uid);
            std::string score = GetScore(uid);
            reply.set_args(i, name + ": " + score);
        }
    } else if (request.requesttype() == Request::GET_MATCH_INFO) {
        int sz = reply.args().size();
        for (int i = 2; i < sz; i += 2) {
            int uid = atoi(reply.args()[i].c_str());
            std::string name = Uid2Name(uid);
            reply.set_args(i, name);
        }
    }

    logger->info("Returned reply to proxy");
    return reply;
}