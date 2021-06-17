#include "Players.h"

constexpr int PlayingFlag = 100000007;

UABlackJackServer::Lobby::Players::Status UABlackJackServer::Lobby::Players::getStatus(UID uid) const {
    if (!players_.count(uid))
        return Status::OFFLINE;
    const auto status = players_.at(uid);
    if (status < 0) {
        return Status::IN_ROOM_NOT_READY;
    }
    if (status > PlayingFlag) {
        return Status::PLAYING;
    }
    if (status == 0) {
        return Status::IN_LOBBY;
    }
    return Status::IN_ROOM_READY;
}

UABlackJackServer::Lobby::Players::RoomID UABlackJackServer::Lobby::Players::getRoom(UID uid) const {
    const auto status = getStatus(uid);
    if (status == Status::OFFLINE)
        return -1;
    const auto rid = players_.at(uid);
    if (rid < 0) {
        return -rid;
    }
    if (rid == 0)
        return 0;
    if (rid > PlayingFlag) {
        return rid - PlayingFlag;
    }
    return rid;
}

void UABlackJackServer::Lobby::Players::newPlayer(UID uid) {
    // return false
    assert(players_.find(uid) == players_.end());
    players_[uid] = 0;
    // return true;
}

void UABlackJackServer::Lobby::Players::joinRoom(UID uid, RoomID rid) {
    assert(players_[uid] == 0);
    players_[uid] = -rid;
}

void UABlackJackServer::Lobby::Players::leaveRoom(UID uid) {
    assert(players_.count(uid));
    players_[uid] = 0;
}

void UABlackJackServer::Lobby::Players::logOut(UID uid) {
    assert(players_.count(uid));
    players_.erase(uid);
}

void UABlackJackServer::Lobby::Players::matchStart(UID uid) {
    assert(players_[uid] > 0 && players_[uid] < PlayingFlag);
    players_[uid] += PlayingFlag;
}

void UABlackJackServer::Lobby::Players::matchEnd(UID uid) {
    assert(players_[uid] > PlayingFlag);
    const auto roomId = players_[uid] - PlayingFlag;
    players_[uid]     = -roomId;
}

void UABlackJackServer::Lobby::Players::ready(UID uid) {
    assert(players_[uid] < 0);
    players_[uid] = -players_[uid];
}

std::string UABlackJackServer::Lobby::Players::stateToString(Status status) {
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
    default:
        return "UNKNOWN";
    }
}
