#pragma once

#include <stdio.h>
#include <unordered_set>
#include <list>
#include <vector>
#include <cassert>
#include "Players.h"
#include "Room.h"

constexpr int maxRooms = 20000;

namespace ua_black_jack_server { 
namespace lobby {

class Lobby{
public:
    using UID = int64_t;
    using RoomID = int32_t;
    using NICKNAME = std::string;
    using PASSWORD = std::string;
public:

    Lobby(){};
    UID Login(NICKNAME nickname, PASSWORD password);

    void Logout(UID uid);

    bool JoinRoom(UID uid, RoomID rid);

    RoomID CreateRoom(UID uid);

    bool LeaveRoom(UID uid);

    bool PlayerReady(UID uid);

    RoomID QuickMatch(UID uid);

    void MatchEnd(RoomID roomid);//由room调用

private:
    //bool check(UID uid);
    
    
    //RPC call 调用room的接口！
    //todo 
    void MatchStart(RoomID rid, std::vector<UID> playersID);

private:
    //每个player的状态由Players管理
    Players AllPlayers_;
    //保存几种不同状态的room
    std::unordered_set<RoomID> emptyRooms_;
    std::unordered_set<RoomID> availableRooms_;//还可以加入的room
    std::unordered_set<RoomID> occupiedRooms_;//对局已经开始的room，它可以从full转换而来，也可以从availabe转换而来
    std::unordered_set<RoomID> fullRooms_;

    std::vector<Room> AllRooms_;

};




}}
