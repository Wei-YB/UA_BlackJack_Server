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

void Client::CheckStatus(const Status& status) {
    // Act upon its status.
    if (status.ok()) {
        std::cout << "Got Reply" << std::endl;
    } else {
        std::cout << "Request Error " << status.error_code() << ": " << status.error_message() << std::endl;
    }
}

int Client::Name2Uid(ClientContext& context, const std::string& name) {
    Response reply;
    Request request;

    request.set_requesttype(Request::GET_UID);
    request.set_uid(-1);
    request.set_stamp(std::time(nullptr));
    request.add_args(name);

    CheckStatus(stub_->Notify(&context, request, &reply));
    return reply.uid();
}

std::string Client::GetScore(ClientContext& context, int uid) {
    Response reply;
    Request request;

    request.set_requesttype(Request::GET_SCORE);
    request.set_uid(uid);
    request.set_stamp(std::time(nullptr));

    CheckStatus(stub_->Notify(&context, request, &reply));
    return reply.args()[0];
}

std::string Client::Uid2Name(ClientContext& context, int uid) {
    Response reply;
    Request request;

    request.set_requesttype(Request::GET_NAME);
    request.set_uid(uid);
    request.set_stamp(std::time(nullptr));

    CheckStatus(stub_->Notify(&context, request, &reply));
    return reply.args()[0];
}

Response Client::RequestDB(Request& request) {
    Response reply;

    ClientContext context;

    int uid = request.uid();

    // The actual RPC.
    CheckStatus(stub_->Notify(&context, request, &reply));

    if (request.requesttype() == Request::RANK_ME) {
        std::string score = GetScore(context, uid);
        reply.add_args(score);
    } else if (request.requesttype() == Request::RANK_TOP) {
        int sz = reply.args().size();
        for (int i = 0; i < sz; ++i) {
            int uid = atoi(reply.args()[i].c_str());
            std::string name = Uid2Name(context, uid);
            std::string score = GetScore(context, uid);
            reply.set_args(i, name + ": " + score);
        }
    }

    return reply;
}