#include "Lobby.h"
#include <iostream>

using std::cout;
using std::endl;

/*
innitialize Allrooms_, the index of allrooms is roomid, index 0 is not used!
*/
ua_black_jack_server::lobby::Lobby::Lobby():
AllRooms_(std::vector<Room> (maxRooms+1)), curMaxRoomID(0) {}

ua_black_jack_server::lobby::Lobby::UID ua_black_jack_server::lobby::Lobby::Login(std::string nickname, std::string password){
    /* return -1 means login failure!
       needs to update player's status, now in lobby 
       todo. rpc call redis service,needs to check password
    */
    UID uid = -1;
    //uid = getUID(nickname);
    //std::string passwordInDatabase = getPassword(uid);

    //***********for test
    if(nickname == "a"){
        uid = 1;
    }
    else if(nickname == "b"){
        uid = 2;
    }
    else if(nickname == "c"){
        uid = 3;
    }
    else if(nickname == "d"){
        uid = 4;
    }
    else if(nickname == "e"){
        uid = 5;
    }
    else if(nickname == "f"){
        uid = 6;
    }
    else if(nickname == "g"){
        uid = 7;
    }
    else if(nickname == "h"){
        uid = 8;
    }
    
    std::string passwordInDatabase = "000000";
    //***********

    if(passwordInDatabase == password){
        AllPlayers_.NewPlayer(uid);
    }
    return uid;
}

void ua_black_jack_server::lobby::Lobby::Logout(UID uid){
    // to do . rpc call redis service
    if(AllPlayers_.GetStatus(uid) == ua_black_jack_server::lobby::Players::Status::IN_ROOM_NOT_READY ||
       AllPlayers_.GetStatus(uid) == ua_black_jack_server::lobby::Players::Status::IN_ROOM_READY ||
       AllPlayers_.GetStatus(uid) == ua_black_jack_server::lobby::Players::Status::PLAYING){
        LeaveRoom(uid);
    }
    //case STATUS::OFFLINE
    AllPlayers_.LogOut(uid);
}

//return -1 means createroom failure
ua_black_jack_server::lobby::Lobby::RoomID ua_black_jack_server::lobby::Lobby::CreateRoom(UID uid){
    if(AllPlayers_.GetStatus(uid) != ua_black_jack_server::lobby::Players::Status::IN_LOBBY){
        printf("uid %d is not in lobby, can not create the new room\n", uid);
        return -1;
    }

    if(curMaxRoomID > maxRooms){
        printf("the size of room is already 20000, can not create the new room\n", uid);
        return -1;
    }
    ++curMaxRoomID;
    emptyRooms_.insert(curMaxRoomID);

    return curMaxRoomID;
}

bool ua_black_jack_server::lobby::Lobby::JoinRoom(UID uid, RoomID rid){

    //the players status is not in lobby
    if(AllPlayers_.GetStatus(uid) != ua_black_jack_server::lobby::Players::Status::IN_LOBBY)
        return false;
    // the roomid is not available    
    if(fullRooms_.count(rid) || occupiedRooms_.count(rid))
        return false;
    
    //update the players status
    AllPlayers_.JoinRoom(uid, rid);
    //update the room status
    AllRooms_[rid].Join_room(uid);

    //just join in, not ready!
    if(emptyRooms_.count(rid)){
        emptyRooms_.erase(rid);
        availableRooms_.insert(rid);
    }

    if(AllRooms_[rid].isFull()){
        fullRooms_.insert(rid);
        availableRooms_.erase(rid);
    }

    return true;
}

bool ua_black_jack_server::lobby::Lobby::LeaveRoom(UID uid){
    //the players status is not in room   
    if(AllPlayers_.GetStatus(uid) != ua_black_jack_server::lobby::Players::Status::IN_ROOM_NOT_READY && 
        AllPlayers_.GetStatus(uid) != ua_black_jack_server::lobby::Players::Status::IN_ROOM_READY){
            return false;
        }
        
    // the roomid is not available: the match is started, cannot leave    
    RoomID rid = AllPlayers_.getRoom(uid);
    if(occupiedRooms_.count(rid) || emptyRooms_.count(rid)){
        return false;
    }
        
    AllPlayers_.LeaveRoom(uid);
    AllRooms_[rid].Leave_room(uid);

    //to be tested. uid leave the room maybe result in the begin of match. 
    bool ret = CheckRoomDone(rid);

    if(ret == false){
        if(fullRooms_.count(rid)){
        fullRooms_.erase(rid);
        availableRooms_.insert(rid);
        }
        
        else if(AllRooms_[rid].isEmpty()){
            availableRooms_.erase(rid);
            emptyRooms_.insert(rid);
        }
    }

    return true;
}

bool ua_black_jack_server::lobby::Lobby::PlayerReady(UID uid){
    if(AllPlayers_.GetStatus(uid) != ua_black_jack_server::lobby::Players::Status::IN_ROOM_NOT_READY){
        return false;
    }

    AllPlayers_.Ready(uid);
    //get roomid
    RoomID rid = AllPlayers_.getRoom(uid);
    assert(rid>0);
    AllRooms_[rid].Ready(uid);

    //every time playerReady needs 
    CheckRoomDone(rid);

    return true;
}

//return -1 means quickmatch failure
 ua_black_jack_server::lobby::Lobby::RoomID ua_black_jack_server::lobby::Lobby::QuickMatch(UID uid){
    /*
        quickmatch is availabal only when uid is in looby. 
        first search in the availabeRooms, if not exists, return a empty room
    */
    if(AllPlayers_.GetStatus(uid) != ua_black_jack_server::lobby::Players::Status::IN_LOBBY)
        return -1;

    int minUnreadySize = 10;
    RoomID rid = 0;//in lobby
    for(auto roomid:availableRooms_){
        int unreadySize = AllRooms_[roomid].unReadySize();
        if(unreadySize == 0){
            rid = roomid;
            break;
        }
        if(unreadySize < minUnreadySize){
            minUnreadySize = unreadySize;
            rid = roomid;
        }
            
    }
    if(rid == 0){
        if(emptyRooms_.size()){
            rid = *emptyRooms_.begin();//get roomid of an existed empty room
            //emptyRooms_.erase(rid); // can not erase now
        }
        else{
            rid = CreateRoom(uid);//get roomid of an new empty room
        }
    }
    
    bool res = JoinRoom(uid, rid);
    assert(res == true);
    res = PlayerReady(uid);//quickmatch means playerReady automatically
    assert(res == true);
    return rid;
}

void ua_black_jack_server::lobby::Lobby::MatchEnd(RoomID rid){
    AllRooms_[rid].Match_end();
    
    auto playersID = AllRooms_[rid].getAllPlayersID();
    for(auto playerid:playersID){
        AllPlayers_.MatchEnd(playerid);
    }

    occupiedRooms_.erase(rid);

    if(AllRooms_[rid].isFull()){
        fullRooms_.insert(rid);
    }
    else{
        availableRooms_.insert(rid);
    }
}

bool ua_black_jack_server::lobby::Lobby::MatchStart(RoomID rid, std::vector<UID> playersID){
    //todo. rpc call
    return true;
}

int ua_black_jack_server::lobby::Lobby::GetPlayersSizeInRoom(RoomID rid){
    return AllRooms_[rid].getAllPlayersID().size();
}

ua_black_jack_server::lobby::Lobby::RoomStatus ua_black_jack_server::lobby::Lobby::GetRoomStatus(RoomID rid){
    if(emptyRooms_.count(rid))
        return RoomStatus::EMPTY;
    else if(occupiedRooms_.count(rid))
        return RoomStatus::OCCUPIED;
    else if(availableRooms_.count(rid))
        return RoomStatus::AVAILABLE;
    else if(fullRooms_.count(rid))
        return RoomStatus::FULL;
    else
        return RoomStatus::NOT_EXIST;
}

bool ua_black_jack_server::lobby::Lobby::CheckRoomDone(RoomID rid){
    if(AllRooms_[rid].isDone()){
        //update room status
        if(availableRooms_.count(rid)){
            availableRooms_.erase(rid);
            occupiedRooms_.insert(rid);
        }

        if(fullRooms_.count(rid)){
            fullRooms_.erase(rid);
            occupiedRooms_.insert(rid);
        }
        
        //match start, every player in this room needs to be changed status.
        std::vector<UID> playersID = AllRooms_[rid].getAllPlayersID();
        for(UID pid: playersID)
            AllPlayers_.MatchStart(pid);

        MatchStart(rid, playersID);
        return true;
    }
    return false;
}