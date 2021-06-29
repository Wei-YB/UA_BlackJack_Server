#include "Lobby.h"
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
/*
    initialize Allrooms_, the index of allrooms is roomid, index 0 is not used!
*/

using namespace ua_blackjack::lobby;

Lobby::Lobby(RPCClient& client):
    AllRooms_(std::vector<Room>(maxRooms + 1)), curMaxRoomID(0), client_(client) {

    //logger = spdlog::basic_logger_mt("Lobby_log", "Logs/Lobby_log.txt");
    logger = spdlog::stdout_color_mt("Lobby log");
    logger->set_level(spdlog::level::trace);
}

Lobby::UID Lobby::Login(std::string nickname, std::string password, int64_t uid, std::string real_pass) {
    if (real_pass == password) {
        if (AllPlayers_.GetStatus(uid) == Players::Status::OFFLINE)
        {
            AllPlayers_.NewPlayer(uid);
            logger->trace("Player:{0:d} has logged in the lobby", uid);
        }else {
            logger->trace("Player {0} already in lobby", nickname);
        }
        return uid;
    }
    else {
        logger->trace("invalid login");
        return -1;
    }
}

void Lobby::Logout(UID uid) {
    if (AllPlayers_.GetStatus(uid) == Players::Status::OFFLINE) {
        return;
    }
    if(AllPlayers_.GetStatus(uid) == Players::Status::PLAYING) {
        client_.LeaveRoom(uid);
    }
    if (AllPlayers_.GetStatus(uid) == Players::Status::IN_ROOM_NOT_READY ||
        AllPlayers_.GetStatus(uid) == Players::Status::IN_ROOM_READY ||
        AllPlayers_.GetStatus(uid) == Players::Status::PLAYING) {
        LeaveRoom(uid);
    }

    AllPlayers_.LogOut(uid);
    logger->info("Player:{0:d} has logged out of the lobby", uid);
}

//return -1 means createroom failure
Lobby::RoomID Lobby::CreateRoom(UID uid) {
    if (AllPlayers_.GetStatus(uid) != Players::Status::IN_LOBBY) {
        logger->warn("Player:{0:d} is not in lobby, can not create the new room", uid);
        return -1;
    }

    if (curMaxRoomID > maxRooms) {
        logger->warn("the size of room is already 20000, can not create the new room.");
        return -1;
    }
    ++curMaxRoomID;
    emptyRooms_.insert(curMaxRoomID);

    logger->info("Player:{0:d} has created the new room:{1:d}", uid, curMaxRoomID);
    return curMaxRoomID;
}

bool Lobby::JoinRoom(UID uid, RoomID rid) {

    //the players status is not in lobby
    if (AllPlayers_.GetStatus(uid) != Players::Status::IN_LOBBY) {
        logger->warn("Player:{0:d} is not in lobby, can not join any room.", uid); //如何设置日志级别
        return false;
    }

    // the roomid is not available    
    if (!availableRooms_.count(rid) && !emptyRooms_.count(rid)) {
        logger->warn("Room:{0:d} is not available, Player:{1:d} cannot join it.", rid, uid);
        return false;
    }

    //update the players status
    AllPlayers_.JoinRoom(uid, rid);
    //update the room status
    AllRooms_[rid].Join_room(uid);

    //just join in, not ready!
    if (emptyRooms_.count(rid)) {
        emptyRooms_.erase(rid);
        availableRooms_.insert(rid);
    }

    if (AllRooms_[rid].isFull()) {
        fullRooms_.insert(rid);
        availableRooms_.erase(rid);
    }

    logger->info("Player:{0:d} has joined in the room:{1:d}", uid, rid);
    return true;
}

bool Lobby::LeaveRoom(UID uid) {
    //the players status is not in room
    const auto status = AllPlayers_.GetStatus(uid);
    if (!(status == Players::Status::IN_ROOM_NOT_READY || status== Players::Status::IN_ROOM_READY)) {
        logger->warn("Player:{0:d} is not in room, can not leave any room.", uid);
        return false;
    }

    // the roomid is not available: the match is started, cannot leave    
    RoomID rid = AllPlayers_.getRoom(uid);
    if(occupiedRooms_.count(rid)) {
        logger->trace("Game {0} is start, forward request to game service", rid);
        auto stamp = client_.LeaveRoom(rid);
        return true;
        
    }
    if (emptyRooms_.count(rid)) {
        logger->warn("Room:{0:d} is empty, can not leave.", rid);
        return false;
    }

    AllPlayers_.LeaveRoom(uid);
    AllRooms_[rid].Leave_room(uid);
    logger->info("Player:{0:d} has left out of the room:{1:d}", uid, rid);

    //to be tested. uid leave the room maybe result in the begin of match. 
    bool ret = CheckRoomDone(rid);

    if (ret == false) {
        if (fullRooms_.count(rid)) {
            fullRooms_.erase(rid);
            availableRooms_.insert(rid);
        }

        else if (AllRooms_[rid].isEmpty()) {
            availableRooms_.erase(rid);
            emptyRooms_.insert(rid);
        }
    }

    return true;
}

bool Lobby::PlayerReady(UID uid) {
    if (AllPlayers_.GetStatus(uid) == Players::Status::IN_ROOM_READY) {
        SPDLOG_TRACE("player {0} is ready, no need to read agagin", uid);
        return true;
    }
    if (AllPlayers_.GetStatus(uid) != Players::Status::IN_ROOM_NOT_READY) {
        SPDLOG_TRACE("Player:{0:d} is not in room with the status of not ready, can not choose the PlayerReady.", uid);
        return false;
    }

    AllPlayers_.Ready(uid);
    //get roomid
    RoomID rid = AllPlayers_.getRoom(uid);
    assert(rid>0);
    AllRooms_[rid].Ready(uid);

    SPDLOG_TRACE("Player:{0:d} has been ready in the room:{1:d}", uid, rid);
    //every time playerReady needs 
    CheckRoomDone(rid);

    return true;
}

//return -1 means quickmatch failure
Lobby::RoomID Lobby::QuickMatch(UID uid) {
    if (AllPlayers_.GetStatus(uid) != Players::Status::IN_LOBBY) {
        logger->warn("Player:{0:d} is not in the lobby, can not choose the quickmatch.", uid);
        return -1;
    }

    logger->info("Quickmatch for player:{0:d} is starting.", uid);
    int    minUnreadySize = 10;
    RoomID rid            = 0; //in lobby
    for (auto roomid : availableRooms_) {
        int unreadySize = AllRooms_[roomid].unReadySize();
        if (unreadySize == 0) {
            rid = roomid;
            break;
        }
        if (unreadySize < minUnreadySize) {
            minUnreadySize = unreadySize;
            rid            = roomid;
        }

    }
    if (rid == 0) {
        if (emptyRooms_.size()) {
            rid = *emptyRooms_.begin(); //get roomid of an existed empty room
            //emptyRooms_.erase(rid); // can not erase now
        }
        else {
            rid = CreateRoom(uid); //get roomid of an new empty room
        }
    }

    JoinRoom(uid, rid);
    PlayerReady(uid);
    logger->trace("Quickmatch for player:{0:d} is finished.", uid);
    return rid;
}

void Lobby::MatchEnd(RoomID rid) {
    AllRooms_[rid].Match_end();

    auto playersID = AllRooms_[rid].getAllPlayersID();
    for (auto playerid : playersID) {
        AllPlayers_.MatchEnd(playerid);
    }

    occupiedRooms_.erase(rid);

    if (AllRooms_[rid].isFull()) {
        fullRooms_.insert(rid);
    }
    else {
        availableRooms_.insert(rid);
    }
    logger->info("The match in the room:{0:d} has been finished", rid);
}

bool Lobby::MatchStart(RoomID rid, const std::vector<UID>& args) {
    client_.GameStart(args);
    return true;
}

int Lobby::GetPlayersSizeInRoom(RoomID rid) {
    return AllRooms_[rid].getAllPlayersID().size();
}

Lobby::RoomStatus Lobby::GetRoomStatus(RoomID rid) {
    if (emptyRooms_.count(rid))
        return RoomStatus::EMPTY;
    if (occupiedRooms_.count(rid))
        return RoomStatus::OCCUPIED;
    if (availableRooms_.count(rid))
        return RoomStatus::AVAILABLE;
    if (fullRooms_.count(rid))
        return RoomStatus::FULL;
    return RoomStatus::NOT_EXIST;
}

bool Lobby::CheckRoomDone(RoomID rid) {
    if (AllRooms_[rid].isDone()) {
        //update room status
        if (availableRooms_.count(rid)) {
            availableRooms_.erase(rid);
            occupiedRooms_.insert(rid);
        }

        if (fullRooms_.count(rid)) {
            fullRooms_.erase(rid);
            occupiedRooms_.insert(rid);
        }

        std::vector<UID> player_list;
        //match start, every player in this room needs to be changed status.
        player_list.push_back(rid);
        auto players = AllRooms_[rid].getAllPlayersID();
        for (UID pid : players) {
            AllPlayers_.MatchStart(pid);
            player_list.push_back(pid);
        }

        MatchStart(rid, player_list);
        logger->info("All the Players in the room:{0:d} has been ready, the match begins at once.", rid);
        return true;
    }
    return false;
}

std::vector<Lobby::RoomID> Lobby::GetAvailableRoomList() {

    std::vector<RoomID> ret;
    for (auto& rid : availableRooms_) {
        ret.push_back(rid);
    }
    ret.push_back(curMaxRoomID);
    return ret;
}
