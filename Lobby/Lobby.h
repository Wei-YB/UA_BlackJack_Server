#pragma once

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif


#include <unordered_set>
#include <vector>
#include "RPCClient.h"
#include "Players.h"
#include "Room.h"

constexpr int maxRooms = 20000;

namespace ua_blackjack { 
namespace lobby {

class Lobby{
public:
    using UID = int64_t;
    using RoomID = int32_t;

public:

    Lobby(RPCClient& client);

    UID Login(std::string nickname, std::string password, int64_t uid, std::string real_pass);

    void Logout(UID uid);

    bool JoinRoom(UID uid, RoomID rid);

    RoomID CreateRoom(UID uid);

    bool LeaveRoom(UID uid);

    bool PlayerReady(UID uid);

    RoomID QuickMatch(UID uid);

    std::vector<RoomID> GetAvailableRoomList();

    void MatchEnd(RoomID rid);

    //for debug
    int GetPlayersSizeInRoom(RoomID rid);
    enum class RoomStatus{
        EMPTY,
        AVAILABLE,
        OCCUPIED,
        FULL,
        NOT_EXIST
    };
    RoomStatus GetRoomStatus(RoomID rid);

private:
    bool CheckRoomDone(RoomID rid);
    
    //RPC call, send data to match service
    bool MatchStart(RoomID rid, const std::vector<UID>& args);

private:
    //the status of player is managed by Players.
    Players AllPlayers_;

    //the status of room is managed by these four set in order to exceed the search.
    std::unordered_set<RoomID> emptyRooms_;
    std::unordered_set<RoomID> availableRooms_;
    std::unordered_set<RoomID> occupiedRooms_;
    std::unordered_set<RoomID> fullRooms_;

    std::vector<Room> AllRooms_;
    RoomID curMaxRoomID;
    std::shared_ptr<spdlog::logger> logger;

    //需要加上RPCClient的引用
    RPCClient& client_;

};
}}
