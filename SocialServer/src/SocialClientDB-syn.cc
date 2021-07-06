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

using namespace ua_blackjack::social_client;

void SocialClient::CheckStatus(const Status& status) {
    // Act upon its status.
    if (status.ok()) {
        logger->info("gRpc Got Reply");
    } else {
        logger->error("gRpc Request Error {0} : {1}", status.error_code(), status.error_message());
    }
}

bool SocialClient::IsFriend(int uid, int fri_uid) {
    ClientContext context;
    Response reply;
    Request request;

    request.set_requesttype(Request::IS_FRIEND);
    request.set_uid(uid);
    request.set_stamp(std::time(nullptr));
    request.add_args(std::to_string(fri_uid));

    logger->info("Send IS_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}, args_size: {3}", reply.status(), reply.uid(),
                 reply.stamp(), reply.args().size());

    if (reply.status() == -1) {
        logger->info("is not friend");
        return false;
    } else {
        logger->info("is friend");
        return true;
    }
}
bool SocialClient::IsWaitingFriend(int uid, int fri_uid) {
    ClientContext context;
    Response reply;
    Request request;

    request.set_requesttype(Request::IS_WAITING_FRIEND);
    request.set_uid(uid);
    request.set_stamp(std::time(nullptr));
    request.add_args(std::to_string(fri_uid));

    logger->info("Send IS_WAITING_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}, args_size: {3}", reply.status(), reply.uid(),
                 reply.stamp(), reply.args().size());

    if (reply.status() == -1) {
        logger->info("is not waiting friend");
        return false;
    } else {
        logger->info("is waiting friend");
        return true;
    }
}

int SocialClient::Name2Uid(const std::string& name) {
    ClientContext context;
    Response reply;
    Request request;

    request.set_requesttype(Request::GET_UID);
    request.set_uid(-1);
    request.set_stamp(std::time(nullptr));
    request.add_args(name);

    logger->info("Send GET_UID to Database Server");
    CheckStatus(stub_->Notify(&context, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}, args_size: {3}", reply.status(), reply.uid(),
                 reply.stamp(), reply.args().size());

    return reply.uid();
}

std::string SocialClient::Uid2Name(int uid) {
    ClientContext context;
    Response reply;
    Request request;

    request.set_requesttype(Request::GET_NAME);
    request.set_uid(uid);
    request.set_stamp(std::time(nullptr));

    logger->info("Send GET_NAME to Database Server");
    CheckStatus(stub_->Notify(&context, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());

    return reply.args()[0];
}

void SocialClient::AddFriend(Request& request_fri, Response& reply, Request& request, int uid) {
    if (IsFriend(uid, request_fri.uid())) {
        reply.set_status(-1);
        reply.set_stamp(request_fri.stamp());
        reply.set_uid(uid);
        reply.add_args("friend");
        return;
    }

    if (IsWaitingFriend(uid, request_fri.uid())) {
        AcceptFriend(request_fri, reply, request, uid);
        return;
    }

    ClientContext context;

    request_fri.set_requesttype(Request::ADD_WAIT_FRIEND);
    request_fri.add_args(std::to_string(uid));

    logger->info("Send ADD_WAIT_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context, request_fri, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());

    reply.set_uid(uid);
}

void SocialClient::AcceptFriend(Request& request_fri, Response& reply, Request& request, int uid) {
    if (!IsWaitingFriend(uid, request_fri.uid())) {
        reply.set_status(-1);
        reply.set_stamp(request_fri.stamp());
        reply.set_uid(uid);
        reply.add_args("bad");
        return;
    }

    // friend operation: AddFriend
    ClientContext context1;
    request_fri.set_requesttype(Request::ADD_FRIEND);
    request_fri.add_args(std::to_string(uid));

    logger->info("Send ADD_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context1, request_fri, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());

    // self operation: 1. DeleteWaitFriend 2. AddFriend
    ClientContext context2;
    request.set_requesttype(Request::DELETE_WAIT_FRIEND);
    request.set_args(0, std::to_string(request_fri.uid()));
    logger->info("Send DELETE_WAIT_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context2, request, &reply));

    ClientContext context3;
    request.set_requesttype(Request::ADD_FRIEND);
    logger->info("Send ADD_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context3, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());
}
void SocialClient::ListFriend(Request& request, Response& reply) {
    ClientContext context;

    logger->info("Send LIST_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());

    int sz = reply.args().size();
    for (int i = 0; i < sz; ++i) {
        int uid = atoi(reply.args()[i].c_str());
        std::string name = Uid2Name(uid);
        reply.set_args(i, name);
    }
}

void SocialClient::ListWaitingFriend(Request& request, Response& reply) {
    ClientContext context;

    logger->info("Send LIST_WAITING_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());

    int sz = reply.args().size();
    for (int i = 0; i < sz; ++i) {
        int uid = atoi(reply.args()[i].c_str());
        std::string name = Uid2Name(uid);
        reply.set_args(i, name);
    }
}
void SocialClient::DeleteFriend(Request& request_fri, Response& reply, Request& request, int uid) {
    if (!IsFriend(uid, request_fri.uid())) {
        reply.set_status(-1);
        reply.set_stamp(request_fri.stamp());
        reply.set_uid(uid);
        reply.add_args("not");
        return;
    }

    // friend operation
    ClientContext context1;
    request_fri.set_requesttype(Request::DELETE_FRIEND);
    request_fri.add_args(std::to_string(uid));
    logger->info("Send DELETE_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context1, request_fri, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());

    // self operation
    ClientContext context2;
    request.set_args(0, std::to_string(request_fri.uid()));
    logger->info("Send DELETE_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context2, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());
}
void SocialClient::DeleteWaitFriend(Request& request, Response& reply, int friend_id) {
    if (!IsWaitingFriend(request.uid(), friend_id)) {
        reply.set_status(-1);
        reply.set_stamp(request.stamp());
        reply.set_uid(request.uid());
        reply.add_args("not");
        return;
    }

    ClientContext context;
    request.set_args(0, std::to_string(friend_id));
    logger->info("Send DELETE_WAITING_FRIEND to Database Server");
    CheckStatus(stub_->Notify(&context, request, &reply));
    logger->info("Got reply, status: {0}, uid: {1}, stamp: {2}", reply.status(), reply.uid(), reply.stamp());
}

Response SocialClient::RequestDB(Request& request) {
    logger->info("Received request, type: {0}, uid: {1}, stamp: {2}", request.requesttype(), request.uid(),
                 request.stamp());

    Response reply;

    int uid = request.uid();

    std::string friend_name;
    int friend_id;
    Request request_fri;

    if (request.args().size() > 0) {
        friend_name = request.args()[0];
        friend_id = Name2Uid(friend_name);
        if (friend_id <= 0 || friend_id == uid) {
            reply.set_status(-1);
            reply.set_uid(uid);
            reply.set_stamp(request.stamp());

            if (friend_id <= 0)
                reply.add_args("none");
            else
                reply.add_args("self");

            return reply;
        }

        request_fri.set_uid(friend_id);
        request_fri.set_stamp(request.stamp());
    }

    // The actual RPC.
    switch (request.requesttype()) {
        case Request::ADD_FRIEND:
            AddFriend(request_fri, reply, request, uid);
            break;
        case Request::ACCEPT_FRIEND:
            AcceptFriend(request_fri, reply, request, uid);
            break;
        case Request::LIST_FRIEND:
            ListFriend(request, reply);
            break;
        case Request::LIST_WAITTING:
            ListWaitingFriend(request, reply);
            break;
        case Request::DELETE_FRIEND:
            DeleteFriend(request_fri, reply, request, uid);
            break;
        case Request::DELETE_WAIT_FRIEND:
            DeleteWaitFriend(request, reply, friend_id);
            break;
        default:
            break;
    }

    return reply;
}