#ifndef _COMMON_H_
#define _COMMON_H_
#include <iostream>
#include <string>
#include <unordered_map>
#include "UA_BlackJack.pb.h"

using ua_blackjack::Response;
using ua_blackjack::Request;

#define DEBUG_MODE  0

#define NS UA_BLACKJACK_SERVER
#define NAMESPACE_BEGIN namespace NS {

#define NAMESPACE_END   }
typedef int FileDesc;
typedef int64_t UserId;

enum BackEndModule {Proxy = 0, Lobby, Room, Social, Player, DataBase};

static std::unordered_map<ua_blackjack::Request_RequestType, BackEndModule> 
requestTypeToModule = {
    // Lobby request
    {Request::LOGIN, BackEndModule::Lobby}, 
    {Request::LOGOUT, BackEndModule::Lobby},
    {Request::ROOM_LIST, BackEndModule::Lobby},
    {Request::JOIN_ROOM, BackEndModule::Lobby},
    {Request::CREATE_ROOM, BackEndModule::Lobby},
    {Request::QUICK_MATCH, BackEndModule::Lobby},
    {Request::READY, BackEndModule::Lobby},
    // Room request
    {Request::LEAVE_ROOM, BackEndModule::Room},
    {Request::BET, BackEndModule::Room},
    {Request::HIT, BackEndModule::Room},
    {Request::STAND, BackEndModule::Room},
    {Request::DOUBLE, BackEndModule::Room},
    {Request::SURRENDER, BackEndModule::Room},
    // Player request
    {Request::ADD_FRIEND, BackEndModule::Social},
    {Request::ADD_WAIT_FRIEND, BackEndModule::Social},
    {Request::DELETE_WAIT_FRIEND, BackEndModule::Social},
    {Request::ACCEPT_FRIEND, BackEndModule::Social},
    {Request::DELETE_FRIEND, BackEndModule::Social},
    {Request::LIST_FRIEND, BackEndModule::Social},
    {Request::LIST_WAITTING, BackEndModule::Social},
    
    // Social request
    {Request::SIGNUP, BackEndModule::Player},
    {Request::INFO, BackEndModule::Player},
    {Request::RANK_ME, BackEndModule::Player},
    {Request::RANK_TOP, BackEndModule::Player},
    {Request::LIST_MATCH, BackEndModule::Player},
    {Request::GET_MATCH_INFO, BackEndModule::Player},
    {Request::GET_SCORE, BackEndModule::Player},

    // Proxy request
    {Request::NOTIFY_USER, BackEndModule::Proxy},
};

static std::unordered_map<ua_blackjack::Request_RequestType, std::string>
requestTypeToStr = {
    {Request::INVAL, "INVAL"}, 
    // Lobby request
    {Request::LOGIN, "LOGIN"}, 
    {Request::LOGOUT, "LOGOUT"},
    {Request::ROOM_LIST, "ROOM_LIST"},
    {Request::JOIN_ROOM, "JOIN_ROOM"},
    {Request::CREATE_ROOM, "CREATE_ROOM"},
    {Request::QUICK_MATCH, "QUICK_MATCH"},
    {Request::READY, "READY"},
    // Room request
    {Request::LEAVE_ROOM, "LEAVE_ROOM"},
    {Request::BET, "BET"},
    {Request::HIT, "HIT"},
    {Request::STAND, "STAND"},
    {Request::DOUBLE, "DOUBLE"},
    {Request::SURRENDER, "SURRENDER"},
    // Player request
    {Request::ADD_FRIEND, "ADD_FRIEND"},
    {Request::ADD_WAIT_FRIEND, "ADD_WAIT_FRIEND"},
    {Request::DELETE_WAIT_FRIEND, "DELETE_WAIT_FRIEND"},
    {Request::ACCEPT_FRIEND, "ACCEPT_FRIEND"},
    {Request::DELETE_FRIEND, "DELETE_FRIEND"},
    {Request::LIST_FRIEND, "LIST_FRIEND"},
    {Request::LIST_WAITTING, "LIST_WAITTING"},
    // Social request
    {Request::SIGNUP, "SIGNUP"},
    {Request::INFO, "INFO"},
    {Request::RANK_ME, "RANK_ME"},
    {Request::RANK_TOP, "RANK_TOP"},
    {Request::LIST_MATCH, "LIST_MATCH"},
    {Request::GET_MATCH_INFO, "GET_MATCH_INFO"},
    {Request::GET_SCORE, "GET_SCORE"},
    
    // Proxy request
    {Request::NOTIFY_USER, "NOTIFY_USER"},
};

static std::unordered_map<std::string, Request::RequestType> 
strToRequestType = {
    {"INVAL", Request::INVAL},
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
    {"LIST_FRIEND", Request::LIST_FRIEND},
    {"LIST_MATCH", Request::LIST_MATCH},
    {"LIST_WAITTING", Request::LIST_WAITTING},
    {"NOTIFY_USER", Request::NOTIFY_USER},
    {"GET_SCORE", Request::GET_SCORE},
    {"GET_MATCH_INFO", Request::GET_MATCH_INFO},
    {"ADD_WAIT_FRIEND", Request::ADD_WAIT_FRIEND},
    {"DELETE_WAIT_FRIEND", Request::DELETE_WAIT_FRIEND}
};

void print(std::ostream &os, const Response &response);

void print(std::ostream &os, const Request &request);

void print(std::ostream &os, int events);

#endif
