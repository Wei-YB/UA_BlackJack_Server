#ifndef _COMMON_H_
#define _COMMON_H_
#include <iostream>
#include <string>
#include <unordered_map>
#include "UA_BlackJack.pb.h"

using ua_blackjack::Response;
using ua_blackjack::Request;

#define NS UA_BLACKJACK_SERVER
#define NAMESPACE_BEGIN namespace NS {

#define NAMESPACE_END   }
typedef int FileDesc;
typedef int64_t UserId;

enum BackEndModule {Proxy = 0, Lobby, Room, Social, DataBase};

static const std::unordered_map<ua_blackjack::Request_RequestType, BackEndModule> requestTypeToModule = {
    // Lobby request
    {ua_blackjack::Request_RequestType_LOGIN, BackEndModule::Lobby}, 
    {ua_blackjack::Request_RequestType_LOGOUT, BackEndModule::Lobby},
    {ua_blackjack::Request_RequestType_ROOM_LIST, BackEndModule::Lobby},
    {ua_blackjack::Request_RequestType_CREATE_ROOM, BackEndModule::Lobby},
    {ua_blackjack::Request_RequestType_QUICK_MATCH, BackEndModule::Lobby},
    {ua_blackjack::Request_RequestType_READY, BackEndModule::Lobby},
    // Room request
    {ua_blackjack::Request_RequestType_LEAVE_ROOM, BackEndModule::Room},
    {ua_blackjack::Request_RequestType_BET, BackEndModule::Room},
    {ua_blackjack::Request_RequestType_HIT, BackEndModule::Room},
    {ua_blackjack::Request_RequestType_STAND, BackEndModule::Room},
    {ua_blackjack::Request_RequestType_DOUBLE, BackEndModule::Room},
    {ua_blackjack::Request_RequestType_SURRENDER, BackEndModule::Room},
    // Social request
    {ua_blackjack::Request_RequestType_SIGNUP, BackEndModule::Social},
    {ua_blackjack::Request_RequestType_INFO, BackEndModule::Social},
    {ua_blackjack::Request_RequestType_RANK_ME, BackEndModule::Social},
    {ua_blackjack::Request_RequestType_RANK_TOP, BackEndModule::Social},
    {ua_blackjack::Request_RequestType_ADD_FRIEND, BackEndModule::Social},
    {ua_blackjack::Request_RequestType_ACCEPT_FRIEND, BackEndModule::Social},
    {ua_blackjack::Request_RequestType_DELETE_FRIEND, BackEndModule::Social},
    {ua_blackjack::Request_RequestType_LIST_FRIEND, BackEndModule::Social},
    {ua_blackjack::Request_RequestType_LIST_MATCH, BackEndModule::Social},
    {ua_blackjack::Request_RequestType_LIST_WAITTING, BackEndModule::Social},
    // Proxy request
    {ua_blackjack::Request_RequestType_NOTIFY_USER, BackEndModule::Proxy},
};

static const std::string requestTypeToStr[] = {
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

static std::unordered_map<std::string, Request::RequestType> strToRequestType = {
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

void print(std::ostream &os, const Response &response);

void print(std::ostream &os, const Request &request);

void print(std::ostream &os, int events);

#endif