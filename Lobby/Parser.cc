#include "Parser.h"
#include <cassert>
#include <algorithm>
#include <list>
#include <cstdio>
using namespace ua_blackjack::lobby;

Parser::Parser(Lobby& lobby): stamp_(-1), lobby_(lobby) {
}

bool Parser::Parse(const Request& request, Response* rpc_result, Response& response) {
    response.set_uid(request.uid());
    response.set_status(request.stamp());
    response.set_status(0);
    const auto req_type = request.requesttype();
    const auto uid      = request.uid();
    if (req_type == ua_blackjack::Request_RequestType_JOIN_ROOM) {
        const auto room_id = std::stoi(request.args(0));
        if (!lobby_.JoinRoom(uid, room_id)) {
            response.set_status(-1);
        }
        return true;

    }
    if (req_type == ua_blackjack::Request_RequestType_LEAVE_ROOM) {
        response.set_status(lobby_.LeaveRoom(uid) ? 0 : -1);
        return true;
    }
    if (req_type == ua_blackjack::Request_RequestType_CREATE_ROOM) {
        const auto room_id = lobby_.CreateRoom(uid);
        if (room_id < 0) {
            response.set_status(-1);
            response.add_args("-1");
        }
        else
            response.add_args(std::to_string(room_id));
        return true;
    }
    if (req_type == ua_blackjack::Request_RequestType_QUICK_MATCH) {
        const auto room_id = lobby_.QuickMatch(uid);
        if (room_id < 0) {
            response.set_status(-1);
            response.add_args("-1");
        }
        else {
            response.add_args(std::to_string(room_id));
        }
        return true;
    }
    if (req_type == ua_blackjack::Request_RequestType_ROOM_LIST) {
        auto ret = lobby_.GetAvailableRoomList();
        for (auto val : ret) {
            response.add_args(std::to_string(val));
        }
        return true;
    }
    if (req_type == ua_blackjack::Request_RequestType_MATCH_END) {
        const auto room_id = std::stoi(request.args()[0]);
        lobby_.MatchEnd(room_id);
        return true;
    }
    if (req_type == ua_blackjack::Request_RequestType_LOGOUT) {
        lobby_.Logout(uid);
        return true;
    }
    response.set_status(-1);
    return true;
}

int64_t Parser::GetStamp() const {
    return stamp_;
}

Parser::~Parser() = default;

LoginParser::LoginParser(Lobby& lobby, RPCClient& client): Parser(lobby),
                                                           client_(client), state_(RPC_State::START), padding_{} {
}

bool LoginParser::Parse(const Request& request, Response* rpcResult, Response& response) {
    response.set_stamp(request.stamp());
    response.set_status(-1);
    if (state_ == RPC_State::START) {
        if (request.args_size() != 2) {
            return true;
        }
        password = request.args()[1];
        nickname = request.args()[0];
        spdlog::trace("rpc call get uid with nickname {0}", nickname);
        stamp_ = client_.GetUid(nickname);
        
        spdlog::trace("this call stamp is {0}", stamp_);
        state_ = RPC_State::GET_UID;
        return false;
    }
    if (state_ == RPC_State::GET_UID) {
        spdlog::trace("try to get uid from rpc result");
        if(rpcResult->status() == -1) {
            spdlog::error("get bad response from database");
            response.set_uid(-1);
            return true;
        }
        const auto uid = rpcResult->uid();
        spdlog::trace("rpc get_uid get response: {0}, which nickname is {1}", uid, nickname);
        if (uid < 0) {
            return true;
        }
        response.set_uid(uid);
        stamp_ = client_.GetPassword(uid);
        state_ = RPC_State::GET_PASSWORD;
        return false;
    }
    if (state_ == RPC_State::GET_PASSWORD) {
        if (rpcResult->status() == -1) {
            spdlog::error("get bad response from database");
            response.set_uid(-1);
            return true;
        }
        const auto& real_pass = rpcResult->args(0);
        spdlog::trace("rpc get_pass get response: {0}, which uid is {1}", real_pass, response.uid());
        const auto ret = lobby_.Login(nickname, password, response.uid(), real_pass);
        if (ret >= 0) {
            response.set_uid(ret);
            response.set_status(0);
        }
        return true;
    }
    return true;
}

LoginParser::~LoginParser() = default;

ReadyParser::ReadyParser(Lobby& lobby, RPCClient& client):
    Parser(lobby),
    client_(client),
    state_(RPC_State::START),
    padding_{} {
}

bool ReadyParser::Parse(const Request& request, Response* rpcResult, Response& response) {
    if (state_ == RPC_State::START) {
        response.set_uid(request.uid());
        response.set_stamp(request.stamp());
        auto ret = lobby_.PlayerReady(request.uid());
        if(ret) {
            response.set_status(0);
            return true;
        }
    }else {
        return true;
    }
    //     std::vector<int64_t> player_list;
    //     const auto           ret = lobby_.PlayerReady(request.uid());
    //     if (!player_list.empty()) {
    //         // match start
    //         client_.GameStart(player_list);
    //         stamp_ = client_.CurStamp();
    //         state_ = RPC_State::CALL_MATCH_START;
    //         return false;
    //     }
    //     if (ret) {
    //         response.set_status(0);
    //         return true;
    //     }
    //     {
    //         // bad request
    //         response.set_status(-1);
    //         return true;
    //     }
    // }
    // else {
    //     const auto success = rpcResult->status();
    //     if (success != 0) {
    //         // match start fail
    //         // TODO;
    //         return true;
    //     }
    //     else {
    //         response.set_status(0);
    //         return true;
    //     }
    // }
}

ReadyParser::~ReadyParser() = default;

ParserFactory::ParserFactory(Lobby& lobby, RPCClient& client):
    lobby_(lobby), rpc_client_(client) {
}

std::shared_ptr<Parser> ParserFactory::NewParser(ua_blackjack::Request_RequestType type) {
    if (type == ua_blackjack::Request_RequestType_LOGIN)
        return std::make_shared<LoginParser>(lobby_, rpc_client_);

    if (type == ua_blackjack::Request_RequestType_READY)
        return std::make_shared<ReadyParser>(lobby_, rpc_client_);
        
    return std::make_shared<Parser>(lobby_);
}
