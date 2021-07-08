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
    all_rooms_(std::vector<Room>(kMaxRooms + 1)), cur_max_roomid_(0), client_(client) {}

Lobby::UID Lobby::Login(std::string nickname, std::string password, int64_t uid, std::string real_pass) {
    if (real_pass == password) {
        //don't judge the status of player whenever login.
        bool ret = all_players_.GetStatus(uid) == Players::Status::OFFLINE;
        ret = true;
        if(ret){
            all_players_.NewPlayer(uid);
            SPDLOG_TRACE("Player:{0:d} has logged in the lobby", uid);
            //****for debug
            RoomID rid = all_players_.getRoom(uid);
            if(rid != 0){
                all_rooms_[rid].Leave_room(uid);
            }
            
            //****
            return uid;
        }else{
            SPDLOG_TRACE("Login failed. Player:{0:d} Login repeatedly", uid);
            return -1;
        }


    }
    else {
        SPDLOG_TRACE("password or nickname wrong. invalid login");
        return -1;
    }
}

void Lobby::Logout(UID uid) {
    if (all_players_.GetStatus(uid) == Players::Status::OFFLINE) {
        return;
    }
    //if(all_players_.GetStatus(uid) == Players::Status::PLAYING) {
    //    client_.LeaveRoom(uid);
    //}
    if (all_players_.GetStatus(uid) == Players::Status::IN_ROOM_NOT_READY ||
        all_players_.GetStatus(uid) == Players::Status::IN_ROOM_READY ||
        all_players_.GetStatus(uid) == Players::Status::PLAYING) {
        LeaveRoom(uid);
    }

    all_players_.LogOut(uid);
    SPDLOG_INFO("Player:{0:d} has logged out of the lobby", uid);
}

//return -1 means createroom failure
Lobby::RoomID Lobby::CreateRoom(UID uid) {
    if (all_players_.GetStatus(uid) != Players::Status::IN_LOBBY) {
        SPDLOG_WARN("Player:{0:d} is not in lobby, can not create the new room", uid);
        return -1;
    }

    if (cur_max_roomid_ > kMaxRooms) {
        SPDLOG_WARN("the size of room is already 20000, can not create the new room.");
        return -1;
    }
    ++cur_max_roomid_;
    empty_rooms_.insert(cur_max_roomid_);

    SPDLOG_INFO("Player:{0:d} has created the new room:{1:d}", uid, cur_max_roomid_);
    return cur_max_roomid_;
}

bool Lobby::JoinRoom(UID uid, RoomID rid) {

    //the players status is not in lobby
    if (all_players_.GetStatus(uid) != Players::Status::IN_LOBBY) {
        SPDLOG_WARN("Player:{0:d} is not in lobby, can not join any room.", uid); 
        return false;
    }

    // the roomid is not available    
    if (!available_rooms_.count(rid) && !empty_rooms_.count(rid)) {
        SPDLOG_WARN("Room:{0:d} is not available, Player:{1:d} cannot join it.", rid, uid);
        return false;
    }

    //update the players status
    all_players_.JoinRoom(uid, rid);
    //update the room status
    all_rooms_[rid].Join_room(uid);

    //just join in, not ready!
    if (empty_rooms_.count(rid)) {
        empty_rooms_.erase(rid);
        available_rooms_.insert(rid);
    }

    if (all_rooms_[rid].isFull()) {
        full_rooms_.insert(rid);
        available_rooms_.erase(rid);
    }

    SPDLOG_INFO("Player:{0:d} has joined in the room:{1:d}", uid, rid);
    return true;
}

bool Lobby::LeaveRoom(UID uid) {
    RoomID rid = all_players_.getRoom(uid);
    if (empty_rooms_.count(rid)) {
        SPDLOG_WARN("Room:{0:d} is empty, can not leave.", rid);
        return false;
    }
    
    const auto status = all_players_.GetStatus(uid);
    //the players status is not in room. logic error fixed
    if (!(status == Players::Status::IN_ROOM_NOT_READY || status == Players::Status::IN_ROOM_READY || status == Players::Status::PLAYING)) {
        SPDLOG_WARN("Player:{0:d} is not in room, can not leave any room.", uid);
        return false;
    }
    //the match is started
    if(status == Players::Status::PLAYING) {
        SPDLOG_TRACE("Game in room:{0:d} is starting, player:{1:d} wants to leave the room and will be hang up.", rid, uid);
        auto stamp = client_.LeaveRoom(uid);//uid error fixed
        //return true;
    }
    
    all_players_.LeaveRoom(uid);
    all_rooms_[rid].Leave_room(uid);
    SPDLOG_INFO("Player:{0:d} has left out of the room:{1:d}", uid, rid);

    //to be tested. uid leave the room maybe result in the begin of match. 
    bool ret = CheckRoomDone(rid);
    if (ret == false) {
        if (full_rooms_.count(rid)) {
            full_rooms_.erase(rid);
            available_rooms_.insert(rid);
        }
        else if (all_rooms_[rid].isEmpty()) {
            available_rooms_.erase(rid);
            empty_rooms_.insert(rid);
        }
    }
    return true;
}

bool Lobby::PlayerReady(UID uid) {
    if (all_players_.GetStatus(uid) == Players::Status::IN_ROOM_READY) {
        SPDLOG_TRACE("player {0} is ready, no need to read agagin", uid);
        return true;
    }
    if (all_players_.GetStatus(uid) != Players::Status::IN_ROOM_NOT_READY) {
        SPDLOG_TRACE("Player:{0:d} is not in room with the status of not ready, can not choose the PlayerReady.", uid);
        return false;
    }

    all_players_.Ready(uid);
    //get roomid
    RoomID rid = all_players_.getRoom(uid);
    //assert(rid>0);
    all_rooms_[rid].Ready(uid);

    SPDLOG_TRACE("Player:{0:d} has been ready in the room:{1:d}", uid, rid);
    //every time playerReady needs 
    CheckRoomDone(rid);

    return true;
}

//return -1 means quickmatch failure
Lobby::RoomID Lobby::QuickMatch(UID uid) {
    if (all_players_.GetStatus(uid) != Players::Status::IN_LOBBY) {
        SPDLOG_WARN("Player:{0:d} is not in the lobby, can not choose the quickmatch.", uid);
        return -1;
    }

    SPDLOG_INFO("Quickmatch for player:{0:d} is starting.", uid);
    int    minUnreadySize = 10;
    RoomID rid            = 0; //in lobby
    for (auto roomid : available_rooms_) {
        int unreadySize = all_rooms_[roomid].unReadySize();
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
        /*
        if (empty_rooms_.size()) {
            rid = *empty_rooms_.begin(); //get roomid of an existed empty room
            //empty_rooms_.erase(rid); // can not erase now
        }
        else {
            rid = CreateRoom(uid); //get roomid of an new empty room
        }
        */
       //当房间全满且没有空房间时quickmatch失败，返回-1
       SPDLOG_TRACE("Quickmatch for player:{0:d} failed.", uid);
       return -1;
    }

    JoinRoom(uid, rid);
    PlayerReady(uid);
    SPDLOG_TRACE("Quickmatch for player:{0:d} is finished.", uid);
    return rid;
}

void Lobby::MatchEnd(RoomID rid) {
    all_rooms_[rid].Match_end();

    auto playersID = all_rooms_[rid].getAllPlayersID();
    for (auto playerid : playersID) {
        if(all_players_.GetStatus(playerid) == Players::Status::PLAYING)
            all_players_.MatchEnd(playerid);
    }

    occupied_rooms_.erase(rid);

    if (all_rooms_[rid].isFull()) {
        full_rooms_.insert(rid);
    }
    else {
        available_rooms_.insert(rid);
    }
    SPDLOG_INFO("The match in the room:{0:d} has been finished", rid);
}

bool Lobby::MatchStart(RoomID rid, std::vector<UID>& args) {

    //add random rotating
    std::uniform_int_distribution<unsigned> u(1, args.size()-1);
    std::default_random_engine e;

    auto randomIndex =u(e);
    //assert(randomIndex >= 1 && randomIndex < args.size());
    if(randomIndex<1 || randomIndex >= args.size()){
        SPDLOG_ERROR("swap of players in the same room failed!");
    }

    std::swap(args[1], args[randomIndex]); 
    
    client_.GameStart(args);
    return true;
}

int Lobby::GetPlayersSizeInRoom(RoomID rid) {
    return all_rooms_[rid].getAllPlayersID().size();
}

Lobby::RoomStatus Lobby::GetRoomStatus(RoomID rid) {
    if (empty_rooms_.count(rid))
        return RoomStatus::EMPTY;
    if (occupied_rooms_.count(rid))
        return RoomStatus::OCCUPIED;
    if (available_rooms_.count(rid))
        return RoomStatus::AVAILABLE;
    if (full_rooms_.count(rid))
        return RoomStatus::FULL;
    return RoomStatus::NOT_EXIST;
}

bool Lobby::CheckRoomDone(RoomID rid) {
    if (all_rooms_[rid].isDone()) {
        //update room status
        if(occupied_rooms_.count(rid)){
            return true;
        }
        if (available_rooms_.count(rid)) {
            available_rooms_.erase(rid);
            occupied_rooms_.insert(rid);
        }

        if (full_rooms_.count(rid)) {
            full_rooms_.erase(rid);
            occupied_rooms_.insert(rid);
        }

        std::vector<UID> player_list;
        //match start, every player in this room needs to be changed status.
        player_list.push_back(rid);
        auto players = all_rooms_[rid].getAllPlayersID();
        for (UID pid : players) {
            all_players_.MatchStart(pid);
            player_list.push_back(pid);
        }

        MatchStart(rid, player_list);
        SPDLOG_INFO("All the Players in the room:{0:d} has been ready, the match begins at once.", rid);
        return true;
    }
    return false;
}

std::vector<Lobby::RoomID> Lobby::GetAvailableRoomList() {
    std::vector<RoomID> ret;
    int CurAvailableRooms = 0;
    for (const auto& rid : available_rooms_) {
        if(CurAvailableRooms < kMaxAvailableRooms){
            ret.push_back(rid);
            ++CurAvailableRooms;
        }
        else{
            break;
        }
        
    }
    for (const auto& rid : empty_rooms_) {
        if(CurAvailableRooms < kMaxAvailableRooms){
            ret.push_back(rid);
            ++CurAvailableRooms;
        }
        else{
            break;
        }
    }
    return ret;
}
