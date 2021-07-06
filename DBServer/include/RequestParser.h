#pragma once

#include "UA_BlackJack.pb.h"
#include "UA_BlackJack.grpc.pb.h"

#include "Database.h"

namespace ua_blackjack::data_base_server {

using ::ua_blackjack::Request;
using ::ua_blackjack::Response;
using RepeatedString = ::google::protobuf::RepeatedPtrField<std::string>;


class RequestParser {
public:
    RequestParser(const std::string& redis_host = "127.0.0.1:6379", int init_score = 2000) : database_(
        redis_host, init_score) {
    }

public:
    bool Parse(const Request& req, Response& response) {
        SPDLOG_DEBUG("try to parse current rquest");
        switch (req.requesttype()) {
        case ua_blackjack::Request_RequestType_SIGNUP:
            return ParseSignUp(req.args(), response);
        case ua_blackjack::Request_RequestType_RANK_ME:
            return ParseRankMe(req.uid(), response);
        case ua_blackjack::Request_RequestType_RANK_TOP:
            return ParseRankTop(req.args(), response);
        case ua_blackjack::Request_RequestType_ADD_FRIEND:
            return ParseAddFriend(req.uid(), req.args(), response);
        case ua_blackjack::Request_RequestType_DELETE_FRIEND:
            return ParseDeleteFriend(req.uid(), req.args(), response);
        case ua_blackjack::Request_RequestType_LIST_FRIEND:
            return ParseGetFriendList(req.uid(), response);
        case ua_blackjack::Request_RequestType_LIST_MATCH:
            return ParseGetMatchList(req.uid(), response);
        case ua_blackjack::Request_RequestType_LIST_WAITTING:
            return ParseGetWaitingFriendList(req.uid(), response);
        case ua_blackjack::Request_RequestType_GET_NAME:
            return ParseGetNickname(req.uid(), response);
        case ua_blackjack::Request_RequestType_GET_UID:
            return ParseGetUid(req.args(), response);
        case ua_blackjack::Request_RequestType_GET_PASSWORD:
            return ParseGetPassword(req.uid(), response);
        case ua_blackjack::Request_RequestType_GET_SCORE:
            return ParseGetScore(req.uid(), response);
        case ua_blackjack::Request_RequestType_GET_MATCH_INFO:
            return ParseGetMatchInfo(req.args(), response);
        case ua_blackjack::Request_RequestType_MATCH_END:
            return ParseMatchEnd(req.args(), response);
        case ua_blackjack::Request_RequestType_ADD_WAIT_FRIEND:
            return ParseAddWaitFriend(req.uid(), req.args(), response);
        case ua_blackjack::Request_RequestType_DELETE_WAIT_FRIEND:
            return ParseDeleteWaitFriend(req.uid(), req.args(), response);
        case ua_blackjack::Request_RequestType_IS_FRIEND:
            return ParseIsFriend(req.uid(), req.args(), response);
        case ua_blackjack::Request_RequestType_IS_WAITING_FRIEND:
            return ParseIsWaitingFriend(req.uid(), req.args(), response);
        default:
            SPDLOG_ERROR("bad request type");
            return false;
        }
    }

private:
    bool ParseSignUp(const RepeatedString& args, Response& response);

    bool ParseGetUid(const RepeatedString& args, Response& response);
    bool ParseGetPassword(int64_t uid, Response& response);
    bool ParseGetScore(int64_t uid, Response& response);
    bool ParseGetNickname(int64_t uid, Response& response);
    bool ParseGetFriendList(int64_t uid, Response& response);
    bool ParseGetWaitingFriendList(int64_t uid, Response& response);
    bool ParseGetMatchList(int64_t uid, Response& response);
    bool ParseGetMatchInfo(const RepeatedString& args, Response& response);

    bool ParseRankMe(int64_t uid, Response& response);
    bool ParseRankTop(const RepeatedString& args, Response& response);

    bool ParseAddFriend(int64_t uid, const RepeatedString& args, Response& response);
    bool ParseDeleteFriend(int64_t uid, const RepeatedString& args, Response& response);
    bool ParseIsFriend(int64_t uid, const RepeatedString& args, Response& response);
    
    bool ParseAddWaitFriend(int64_t uid, const RepeatedString& args, Response& response);
    bool ParseDeleteWaitFriend(int64_t uid, const RepeatedString& args, Response& response);
    bool ParseIsWaitingFriend(int64_t uid, const RepeatedString& args, Response& response);


    bool ParseMatchEnd(const RepeatedString& args, Response& response);

    Database database_;

};
}
