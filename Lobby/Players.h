#pragma once

#include <unordered_map>
#include <string>
#include <cassert>




namespace UABlackJackServer { namespace Lobby {

class Players {
public:
    using UID = int64_t;
    using RoomID = int32_t;
    // using Status = int32_t;

    enum class Status {
        IN_LOBBY,
        IN_ROOM_NOT_READY,
        IN_ROOM_READY,
        PLAYING,
        OFFLINE,
    };

public:
    Status getStatus(UID uid) const;

    /**
     * \brief get room id by player id
     * \param uid player id
     * \return roomId, except -1 means offline, 0 means in lobby
     */
    RoomID getRoom(UID uid) const;

    void newPlayer(UID uid);

    void joinRoom(UID uid, RoomID rid);

    void leaveRoom(UID uid);

    void logOut(UID uid);

    void matchStart(UID uid);

    void matchEnd(UID uid);

    void ready(UID uid);

    // for debug
    static std::string stateToString(Status status);

private:
    std::unordered_map<UID, int32_t> players_;
};

}}
