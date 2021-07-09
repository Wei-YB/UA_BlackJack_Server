#include "Players.h"
#include <cassert>

constexpr int kPlayingFlag = 100000007;

ua_blackjack::lobby::Players::Status ua_blackjack::lobby::Players::GetStatus(const UID uid) const {
    if (!players_.count(uid))
        return Status::OFFLINE;
    const auto status = players_.at(uid);
    if (status < 0) {
        return Status::IN_ROOM_NOT_READY;
    }
    if (status > kPlayingFlag) {
        return Status::PLAYING;
    }
    if (status == 0) {
        return Status::IN_LOBBY;
    }
    return Status::IN_ROOM_READY;
}

ua_blackjack::lobby::Players::RoomID ua_blackjack::lobby::Players::getRoom(const UID uid) const {
    const auto status = GetStatus(uid);
    if (status == Status::OFFLINE)
        return -1;
    const auto rid = players_.at(uid);
    if (rid < 0) {
        return -rid;
    }
    if (rid == 0)
        return 0;
    if (rid > kPlayingFlag) {
        return rid - kPlayingFlag;
    }
    return rid;
}

void ua_blackjack::lobby::Players::NewPlayer(const UID uid) {
    // return false
    //assert(players_.find(uid) == players_.end()); 
    players_[uid] = 0;// indicates in Lobby now
    // return true;
}

void ua_blackjack::lobby::Players::JoinRoom(const UID uid, const RoomID rid) {
    //assert(players_[uid] == 0);
    if(players_[uid] != 0){
        SPDLOG_ERROR("player:{0:d} is not in lobby, cannot not join the room{1:d}.", uid, rid);
    }
    players_[uid] = -rid;
}

void ua_blackjack::lobby::Players::LeaveRoom(const UID uid) {
    //assert(players_.count(uid));
    if(!players_.count(uid)){
        SPDLOG_ERROR("player:{0:d} is not in room, cannot not leave this room.", uid);
    }
    players_[uid] = 0;
}

void ua_blackjack::lobby::Players::LogOut(const UID uid) {
    //assert(players_.count(uid));
    if(!players_.count(uid)){
        SPDLOG_ERROR("player:{0:d} is not in room, no needs to call logout in the Players.cc.", uid);
    }
    players_.erase(uid);
}

void ua_blackjack::lobby::Players::MatchStart(const UID uid) {
    SPDLOG_TRACE("MatchStart has been called with uid {0:d}, the matchstart room tag is {1:d}. (should larger than 100000007)",uid, players_[uid]);
    //assert(players_[uid] > 0 && players_[uid] < kPlayingFlag);
    if(!(players_[uid] > 0 && players_[uid] < kPlayingFlag)){
        SPDLOG_ERROR("player:{0:d} is not IN_ROOM_READY, cannot call the MatchStart.", uid);
    }
    players_[uid] += kPlayingFlag;
}

void ua_blackjack::lobby::Players::MatchEnd(const UID uid) {
    SPDLOG_TRACE("MatchEnd has been called with uid {0:d}, the matchend room tag {1:d}. (should larger than 100000007)", uid, players_[uid]);
    //assert(players_[uid] > kPlayingFlag);
    if(!(players_[uid] > kPlayingFlag)){
        SPDLOG_ERROR("player:{0:d} 's state is not right in lobby, cannot be called by the MatchEnd.", uid);
    }
    const auto room_id = players_[uid] - kPlayingFlag;
    players_[uid]      = -room_id;
}

void ua_blackjack::lobby::Players::Ready(const UID uid) {
    //assert(players_[uid] < 0);
    if(!(players_[uid] < 0)){
        SPDLOG_ERROR("player:{0:d} 's state is not IN_ROOM_UNREADY, ready failed.", uid);
    }
    players_[uid] = -players_[uid];
}

std::string ua_blackjack::lobby::Players::StateToString(const Status status) {
    switch (status) {
    case Status::IN_LOBBY:
        return "IN_LOBBY";
    case Status::IN_ROOM_READY:
        return "IN_ROOM_READY";
    case Status::IN_ROOM_NOT_READY:
        return "IN_ROOM_NOT_READY";
    case Status::PLAYING:
        return "PLAYING";
    case Status::OFFLINE:
        return "OFFLINE";
    default: // NOLINT(clang-diagnostic-covered-switch-default)
        return "UNKNOWN";
    }
}
