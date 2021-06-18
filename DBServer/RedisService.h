#pragma once

#include <lib_acl.hpp>
#include <unordered_map>

#include "nameFormat.h"

namespace ua_black_jack_server {namespace DataBaseServer {

using nameFormat::GetKey;
using nameFormat::FormatType;

class RedisService {
public:
    using UID = int64_t;
    using String = acl::string;
    using RepeatedString = std::vector<acl::string>;
    using HashString = std::unordered_map<String, String>;

public:
    RedisService(acl::redis_client& client) : conn_(&client) {
    }

    // nickname -> UID
    bool   NameExists(const char* nickname);
    String GetUid(const char* nickname);
    bool   SetUid(const char* nickname, UID uid);

    // UID -> password
    String GetPassword(UID uid);
    bool   setPassword(UID uid, const char* password);

    // UID -> nickname
    String GetNickname(UID uid);
    bool   SetNickname(UID uid, const char* nickname);

    // UID -> score
    int  GetScore(UID uid);
    bool SetScore(UID uid, int score);
    bool AddScore(UID uid, int diff);

    // UID -> friendList
    RepeatedString GetFriendList(UID uid);
    bool           InsertFriendList(UID uid, UID friendId);
    bool           RemoveFriendList(UID uid, UID friendId);

    // TODO
    // UID -> waitingFriendList
    // RepeatedString GetWaitingFriendList(UID uid);
    // bool InsertWaitingFriendList(UID uid, UID friendId);
    // bool RemoveWaitingFriendList(UID uid, UID friendId);

    // UID -> matchList
    RepeatedString GetMatchList(UID uid);
    bool           InsertMatchList(UID uid, int32_t mid);

    // UID -> rank
    int  GetRank(UID uid);
    bool UpdateRank(UID uid, double score);
    bool AddRankScore(UID uid, int diff);

    // rankSet
    RepeatedString GetTopPlayer(UID index);

    // TODO
    // HashString getMatchInfo(UID matchId);
    // bool insertMatchInfo(UID matchId, const HashString& matchInfo);

    UID     NextUid();
    int32_t NextMatchId();

    bool Exists(const char* key);


private:
    std::vector<acl::string> listBuffer_;
    acl::string              buffer_;
    // acl::redis_string conn_string_;
    acl::redis conn_;
    // acl::redis_client client_;
};
}}
