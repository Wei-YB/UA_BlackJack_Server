#ifndef _COMMON_H_
#define _COMMON_H_
#include <string>
#include <unordered_map>
#include "common.pb.h"

#define NS UA_BLACKJACK_SERVER
#define NAMESPACE_BEGIN namespace NS {

#define NAMESPACE_END   }
typedef int FileDesc;
typedef int64_t UserId;

enum BackEndModule {Proxy = 0, Lobby, Room, Social, DataBase};

const std::unordered_map<common::Request_RequestType, BackEndModule> cmdTypeToModule = {
    // Lobby request
    {common::Request_RequestType_LOGIN, BackEndModule::Lobby}, 
    {common::Request_RequestType_LOGOUT, BackEndModule::Lobby},
    {common::Request_RequestType_ROOM_LIST, BackEndModule::Lobby},
    {common::Request_RequestType_CREATE_ROOM, BackEndModule::Lobby},
    {common::Request_RequestType_QUICK_MATCH, BackEndModule::Lobby},
    {common::Request_RequestType_READY, BackEndModule::Lobby},
    // Room request
    {common::Request_RequestType_LEAVE_ROOM, BackEndModule::Room},
    {common::Request_RequestType_BET, BackEndModule::Room},
    {common::Request_RequestType_HIT, BackEndModule::Room},
    {common::Request_RequestType_STAND, BackEndModule::Room},
    {common::Request_RequestType_DOUBLE, BackEndModule::Room},
    {common::Request_RequestType_SURRENDER, BackEndModule::Room},
    // Social request
    {common::Request_RequestType_SIGNUP, BackEndModule::Social},
    {common::Request_RequestType_INFO, BackEndModule::Social},
    {common::Request_RequestType_RANK_ME, BackEndModule::Social},
    {common::Request_RequestType_RANK_TOP, BackEndModule::Social},
    {common::Request_RequestType_ADD_FRIEND, BackEndModule::Social},
    {common::Request_RequestType_ACCEPT_FRIEND, BackEndModule::Social},
    {common::Request_RequestType_DELETE_FRIEND, BackEndModule::Social},
    {common::Request_RequestType_LIST_FRIEND, BackEndModule::Social},
    {common::Request_RequestType_LIST_MATCH, BackEndModule::Social},
    {common::Request_RequestType_LIST_WAITTING, BackEndModule::Social},
    // Proxy request
    {common::Request_RequestType_NOTIFY_USER, BackEndModule::Proxy},
};

const std::string cmdTypeToStr[] = {
        "INVAL",  
        // client to proxy, proxy to lobby 
        "LOGIN",
        "LOGOUT", 
        "ROOM_LIST",
        "JOIN_ROOM",
        "CREATE_ROOM",
        "QUICK_MATCH",
        "READY",
        // client to proxy, proxy to room
        "LEAVE_ROOM",
        "BET",
        "HIT",
        "STAND",
        "DOUBLE",
        "SURRENDER",
        // client to proxy, proxy to social
        "SIGNUP",
        "INFO",  
        "RANK_ME",
        "RANK_TOP",  
        "ADD_FRIEND",
        "ACCEPT_FRIEND",
        "DELETE_FRIEND",
        "LIST_FRIEND",
        "LIST_MATCH",
        "LIST_WAITTING",
        // room, lobby, and social to proxy, proxy to client
        "NOTIFY_USER"
        // 
};

std::unordered_map<std::string, Request::RequestType> 
strToType = {
    {"NULL", Request::INVAL},
    {"LOGIN", Request::LOGIN},
    {"LOGOUT", Request::LOGOUT},
    {"ROOM_LIST", Request::ROOM_LIST},
    {"JOIN_ROOM", Request::JOIN_ROOM},
    {"CREATE_ROOM", Request::CREATE_ROOM},
    {"QUICK_MATCH", Request::QUICK_MATCH},
    // forward to room
    {"LEAVE_ROOM", Request::LEAVE_ROOM},
    {"READY", Request::READY},
    {"BET", Request::BET},
    {"HIT", Request::HIT},
    {"STAND", Request::STAND},
    {"DOUBLE", Request::DOUBLE},
    {"SURRENDER", Request::SURRENDER},
    // forward to social
    {"SIGNIN", Request::SIGNUP},
    {"INFO", Request::INFO},
    {"RANK_ME", Request::RANK_ME},
    {"RANK_TOP", Request::RANK_TOP},
    {"ADD_FRIEND", Request::ADD_FRIEND},
    {"ACCEPT_FRIEND", Request::ACCEPT_FRIEND},
    {"DELETE_FRIEND", Request::DELETE_FRIEND},
    {"LIST_FRIEND", Request::LIST_FRIEND}
};

#endif