#pragma once

#include "Player.pb.h"
#include "Player.grpc.pb.h"

#include "Database.h"

namespace ua_black_jack_server::data_base_server {

using ::player::Request;
using ::player::Response;
using RepeatedString = ::google::protobuf::RepeatedPtrField<std::string>;


class RequestParser {
public:
    RequestParser(const std::string& redis_host = "127.0.0.1:6379", int init_score = 2000) : database_(redis_host, init_score) {
    }

public:
    bool Parse(const Request& req, Response& response) {
        switch (req.requesttype()) {
        case player::Request_RequestType_SIGNUP:
            return ParseSignUp(req.args(), response);
        case player::Request_RequestType_RANK_ME:
            return ParseRankMe(req.uid(), response);
        case player::Request_RequestType_RANK_TOP:
            return ParseRankTop(req.args(), response);
        case player::Request_RequestType_ADD_FRIEND:
            return ParseAddFriend(req.uid(), req.args(), response);
        case player::Request_RequestType_DELETE_FRIEND:
            return ParseDeleteFriend(req.uid(), req.args(), response);
        case player::Request_RequestType_LIST_FRIEND:
            return ParseGetFriendList(req.uid(), response);
        case player::Request_RequestType_LIST_MATCH:
            return ParseGetMatchList(req.uid(), response);
        case player::Request_RequestType_LIST_WAITTING:
            return ParseGetWaitingFriendList(req.uid(), response);
        case player::Request_RequestType_GET_NAME:
            return ParseGetNickname(req.uid(), response);
        case player::Request_RequestType_GET_UID:
            return ParseGetUid(req.args(), response);
        case player::Request_RequestType_GET_PASSWORD:
            return ParseGetPassword(req.uid(), response);
        case player::Request_RequestType_GET_SCORE:
            return ParseGetScore(req.uid(), response);
        case player::Request_RequestType_GET_MATCH_INFO:
            return ParseGetMatchInfo(req.args(), response);
        case player::Request_RequestType_MATCH_END:
            return ParseMatchEnd(req.args(), response);
        default:
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
    bool ParseAddWaitFriend(int64_t uid, const RepeatedString& args, Response& response);
    bool ParseDeleteWaitFriend(int64_t uid, const RepeatedString& args, Response& response);

    bool ParseMatchEnd(const RepeatedString& args, Response& response);

    Database database_;

};
}
