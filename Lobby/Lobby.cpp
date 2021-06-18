#include "Lobby.h"

/*
innitialize Allrooms_, the index of allrooms is roomid, index 0 is not used!
*/
ua_black_jack_server::lobby::Lobby::Lobby():
AllRooms_(std::vector<Room> (maxRooms+1))
{
    for(int i=1;i<=maxRooms; i++){
        emptyRooms_.insert(i);
    }

}

/*
1 
*/
ua_black_jack_server::lobby::Lobby::UID ua_black_jack_server::lobby::Lobby::Login(NICKNAME nickname, PASSWORD password){
    //needs to update player's status, now in lobby 
    // to do . rpc call redis service,needs to check password
    UID uid;
    AllPlayers_.NewPlayer(uid);

    
    return uid;

}

void ua_black_jack_server::lobby::Lobby::Logout(UID uid){
    // to do . rpc call redis service
    if(AllPlayers_.GetStatus(uid) == ua_black_jack_server::lobby::Players::Status::IN_ROOM_NOT_READY ||
       AllPlayers_.GetStatus(uid) == ua_black_jack_server::lobby::Players::Status::IN_ROOM_READY ||
       AllPlayers_.GetStatus(uid) == ua_black_jack_server::lobby::Players::Status::PLAYING){
        LeaveRoom(uid);

    }
    AllPlayers_.LogOut(uid);
    

}

ua_black_jack_server::lobby::Lobby::RoomID ua_black_jack_server::lobby::Lobby::CreateRoom(UID uid){
    RoomID rid = *(emptyRooms_.begin());
    //don't need to update the room status
    return rid;

}

bool ua_black_jack_server::lobby::Lobby::JoinRoom(UID uid, RoomID rid){
    /*first don't consider the exception
    */
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
        AllPlayers_.GetStatus(uid) != ua_black_jack_server::lobby::Players::Status::IN_ROOM_READY)
        return false;
    // the roomid is not available, the match is started, cannot leave    
    RoomID rid = AllPlayers_.getRoom(uid);
    if(occupiedRooms_.count(rid) || emptyRooms_.count(rid))
        return false;
    
    AllPlayers_.LeaveRoom(uid);
    AllRooms_[rid].Leave_room(uid);
    
    if(fullRooms_.count(rid)){
        fullRooms_.erase(rid);
        availableRooms_.insert(rid);
    }
        
    if(AllRooms_[rid].isEmpty()){
        availableRooms_.erase(rid);
        emptyRooms_.insert(rid);
    }
    
    return true;
}

bool ua_black_jack_server::lobby::Lobby::PlayerReady(UID uid){
    if(AllPlayers_.GetStatus(uid) != ua_black_jack_server::lobby::Players::Status::IN_ROOM_NOT_READY)
        return false;

    AllPlayers_.Ready(uid);
    //get roomid
    RoomID rid = AllPlayers_.getRoom(uid);
    assert(rid>0);
    AllRooms_[rid].Ready(uid);

    //every time playerReady needs 
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
        for(UID id: playersID)
            AllPlayers_.MatchStart(id);

        MatchStart(rid, playersID);

    }
    return true;
}

 ua_black_jack_server::lobby::Lobby::RoomID ua_black_jack_server::lobby::Lobby::QuickMatch(UID uid){
    /*this logic is complex
    first search in the availabeRooms, if not exists, return a empty room
    */
    int minUnreadySize = 10;
    RoomID rid = 0;//in lobby
    for(auto roomid:availableRooms_){
        int unreadySize = AllRooms_[roomid].TotalSize() - AllRooms_[roomid].ReadySize();
        if(unreadySize == 1){
            rid = roomid;
            break;
        }
        if(unreadySize < minUnreadySize){
            minUnreadySize = unreadySize;
            rid = roomid;
        }
            
    }
    if(rid == 0){
        rid = CreateRoom(uid);//get roomid of an empty room
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
    for(auto playerid:playersID)
        AllPlayers_.MatchEnd(playerid);
    
    occupiedRooms_.erase(rid);

    if(AllRooms_[rid].isFull())
        fullRooms_.insert(rid);
    else
        availableRooms_.insert(rid);
}

void ua_black_jack_server::lobby::Lobby::MatchStart(RoomID rid, std::vector<UID> playersID){
    //rpc call

}

