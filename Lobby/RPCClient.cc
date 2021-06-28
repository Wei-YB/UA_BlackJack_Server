#include "RPCClient.h"

#include <spdlog/spdlog.h>

namespace spd = spdlog;

using namespace ua_blackjack;

RPCClient::RPCClient(std::shared_ptr<Channel>   database_channel,
                     std::shared_ptr<Channel>   game_channel,
                     BlockList& response_list):
    database_stub_(DatabaseService::NewStub(database_channel)),
    game_stub_(GameService::NewStub(game_channel)),
    result_list_(response_list) {
}

int RPCClient::GetPassword(int64_t uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(ua_blackjack::Request_RequestType_GET_PASSWORD);
    auto stamp = SetStamp(request);
    AsyncCall(database_stub_, std::move(request));
    return stamp;
}

int RPCClient::GetUid(const std::string& nickname) {
    Request request;
    request.add_args(nickname);
    auto stamp = SetStamp(request);
    request.set_requesttype(ua_blackjack::Request_RequestType_GET_UID);
    AsyncCall(database_stub_, std::move(request));
    return stamp;
}

void RPCClient::GameStart(const std::vector<int64_t>& playerList) {
    Request request;
    for (auto uid : playerList) {
        request.add_args(std::to_string(uid));
    }
    SetStamp(request);
    request.set_requesttype(ua_blackjack::Request_RequestType_GAME_START);
    AsyncCall(game_stub_, std::move(request));
}

int RPCClient::LeaveRoom(int64_t uid) {
    Request    request;
    const auto stamp = SetStamp(request);
    request.set_requesttype(Request::LEAVE_ROOM);
    request.set_uid(uid);
    AsyncCall(game_stub_, std::move(request));
    return stamp;
}

void RPCClient::AsyncCompleteRpc() {
    void* got_tag;
    bool  ok = false;

    while (cq_.Next(&got_tag, &ok)) {
        std::string result;
        auto*       call = static_cast<AsyncClientCall*>(got_tag);
        GPR_ASSERT(ok);
        if (call->status.ok()) {
            spd::trace("success response with stamp {0}", call->reply.stamp());
            result_list_.PushBack(std::move(call->reply));
        }
        else {
            spd::trace("got bad response");
            Response bad_response;
            bad_response.set_stamp(call->stamp);
            bad_response.set_status(-1);
            result_list_.PushBack(std::move(bad_response));
        }
        delete call;
    }
}

int RPCClient::NextStamp() {
    if (stamp_ == INT32_MAX) {
        return stamp_ = 0;
    }
    return static_cast<int>(++stamp_);
}
