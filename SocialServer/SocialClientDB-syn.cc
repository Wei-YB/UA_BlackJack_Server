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

#include "SocialClientDB-syn.h"

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
    std::string friend_name = request.args()[0];
    int friend_id = Name2Uid(context, friend_name);

    Request request_fri;
    request_fri.set_uid(friend_id);
    request_fri.set_stamp(request.stamp());

    // The actual RPC.
    switch (request.requesttype()) {
        case Request::ADD_FRIEND:
            request_fri.set_requesttype(Request::ADD_WAIT_FRIEND);
            request_fri.add_args(std::to_string(uid));
            CheckStatus(stub_->Notify(&context, request_fri, &reply));
            reply.set_uid(uid);
            break;
        case Request::ACCEPT_FRIEND:
            request_fri.set_requesttype(Request::ADD_FRIEND);
            request_fri.add_args(std::to_string(uid));
            CheckStatus(stub_->Notify(&context, request_fri, &reply));
            request.set_requesttype(Request::ADD_FRIEND);
            CheckStatus(stub_->Notify(&context, request, &reply));
            break;
        case Request::LIST_FRIEND:
        case Request::LIST_WAITTING:
            CheckStatus(stub_->Notify(&context, request, &reply));
            break;
        case Request::DELETE_FRIEND:
            request_fri.set_requesttype(Request::DELETE_FRIEND);
            request_fri.add_args(std::to_string(uid));
            CheckStatus(stub_->Notify(&context, request_fri, &reply));
            CheckStatus(stub_->Notify(&context, request, &reply));
        case Request::DELETE_WAIT_FRIEND:
            CheckStatus(stub_->Notify(&context, request, &reply));
        default:
            break;
    }

    int sz = reply.args().size();
    for (int i = 0; i < sz; ++i) {
        int uid = atoi(reply.args()[i].c_str());
        std::string name = Uid2Name(context, uid);
        reply.set_args(i, name);
    }

    return reply;
}