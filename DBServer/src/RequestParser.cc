#include "RequestParser.h"
// #define STRESS_BENCH

namespace ua_blackjack::data_base_server {


template <typename T>
T typecast(const std::string& str) {
    return T();
}

template <>
int typecast<int>(const std::string& str) {
    return std::stoi(str);
}

bool RequestParser::ParseSignUp(const RepeatedString& args, Response& response) {
    if (args.size() != 2)
        return false;
    auto uid = database_.SignUp(args[0], args[1]);
    if (uid < 0)
        return false;
    response.set_uid(uid);
    response.set_status(1);
    return true;
}

bool RequestParser::ParseGetUid(const RepeatedString& args, Response& response) {
#ifdef STRESS_BENCH
    if(args.size() != 1)
        return false;
    const auto uid = 1000;
    response.set_uid(uid > 0 ? uid : -1);
    response.set_status(1);
    return true;
#else
    if (args.size() != 1)
        return false;
    SPDLOG_TRACE("Parse GET UID, call database.getuid");
    const auto uid = database_.GetUid(args[0]);
    if(uid < 0){
        response.set_status(-1);
        return false;
    }
    response.set_uid(uid > 0 ? uid : -1);
    response.set_status(1);
    return true;
#endif
}

bool RequestParser::ParseGetPassword(int64_t uid, Response& response) {
    const auto pass = database_.GetPassword(uid);
    response.add_args(pass);
    return true;
}

bool RequestParser::ParseGetScore(int64_t uid, Response& response) {
    response.add_args( std::to_string(database_.GetScore(uid)));
    return true;
}

bool RequestParser::ParseGetNickname(int64_t uid, Response& response) {
    response.add_args(database_.GetNickname(uid));
    return true;
}

bool RequestParser::ParseGetFriendList(int64_t uid, Response& response) {
    auto friendList = database_.GetFriendList(uid);
    for (auto& str : friendList) {
        response.add_args(std::move(str));
    }
    return true;
}

bool RequestParser::ParseGetWaitingFriendList(int64_t uid, Response& response) {
    auto waitFriendList = database_.GetWaitingFriendList(uid);
    for (auto& str : waitFriendList) {
        response.add_args(std::move(str));
    }
    return true;
}

bool RequestParser::ParseGetMatchList(int64_t uid, Response& response) {
    auto matchList = database_.getMatchList(uid);
    SPDLOG_TRACE("get match list for {0}, size is {1}", uid, matchList.size());
    for (auto& str : matchList) {
        response.add_args(std::move(str));
    }
    return true;
}

bool RequestParser::ParseGetMatchInfo(const RepeatedString& args, Response& response) {
    if (args.size() != 1)
        return false;
    auto matchId = std::stoi(args[0]);
    auto info    = database_.GetMatchInfo(matchId);
    if (info.empty())
        return true;
    response.add_args("time");
    response.add_args(info["time"]);
    info.erase("time");
    for (auto& p : info) {
        response.add_args(p.first);
        response.add_args(std::move(p.second));
    }
    return true;
}

bool RequestParser::ParseRankMe(int64_t uid, Response& response) {
    auto rank = database_.RankMe(uid);
    response.add_args(std::to_string(rank));
    return true;
}

bool RequestParser::ParseRankTop(const RepeatedString& args, Response& response) {
    if (args.size() != 1)
        return false;
    auto index = std::stoi(args[0]);
    auto ret   = database_.RankTop(index);
    for (auto& str : ret) {
        response.add_args(std::move(str));
    }
    return true;
}

bool RequestParser::ParseAddFriend(int64_t uid, const RepeatedString& args, Response& response) {
    SPDLOG_TRACE("parse add friend");
    if (args.size() != 1)
        return false;
    auto friendId = std::stoi(args[0]);
    return database_.AddFriend(uid, friendId);
}

bool RequestParser::ParseDeleteFriend(int64_t uid, const RepeatedString& args, Response& response) {
    if (args.size() != 1)
        return false;
    auto friendId = std::stoi(args[0]);
    return database_.DeleteFriend(uid, friendId);
}

bool RequestParser::ParseAddWaitFriend(int64_t uid, const RepeatedString& args, Response& response) {
    if (args.size() != 1)
        return false;
    auto friendId = std::stoi(args[0]);
    return database_.AddWaitFriend(uid, friendId);
}

bool RequestParser::ParseDeleteWaitFriend(int64_t uid, const RepeatedString& args, Response& response) {
    if (args.size() != 1)
        return false;
    auto friendId = std::stoi(args[0]);
    return database_.DeleteWaitingFriend(uid, friendId);
}

bool RequestParser::ParseIsFriend(int64_t uid, const RepeatedString& args, Response& response){
    if (args.size() != 1)
        return false;
    auto friendId = std::stoi(args[0]);
    return database_.IsFriend(uid, friendId);
}

bool RequestParser::ParseIsWaitingFriend(int64_t uid, const RepeatedString& args, Response& response){
    if (args.size() != 1)
        return false;
    auto friendId = std::stoi(args[0]);
    return database_.IsWaitingFriend(uid, friendId);
}

bool RequestParser::ParseMatchEnd(const RepeatedString& args, Response& response) {
    if (args.size() % 2 != 0)
        return false;
    std::map<std::string, std::string> input;
    for (int i = 0; i < args.size() / 2; ++i) {
        input[args[2 * i]] = args[2 * i + 1];
    }

    return database_.MatchFinish(input);
}

}
