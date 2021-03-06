#pragma once

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <unordered_set>
#include <vector>
#include "RPCClient.h"
#include "Players.h"
#include "Room.h"
#include <random>



namespace ua_blackjack { 
namespace lobby {

constexpr int kMaxRooms = 20000;
constexpr int kMaxAvailableRooms = 500; 

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
    bool MatchStart(RoomID rid, std::vector<UID>& args);

private:
    //the status of player is managed by Players.
    Players all_players_;

    //the status of room is managed by these four set in order to exceed the search.
    std::unordered_set<RoomID> empty_rooms_;
    std::unordered_set<RoomID> available_rooms_;
    std::unordered_set<RoomID> occupied_rooms_;
    std::unordered_set<RoomID> full_rooms_;

    std::vector<Room> all_rooms_;
    RoomID cur_max_roomid_;

    //add a reference to RPCclient 
    RPCClient& client_;

};
}}
