#ifndef _PROXYSERVER_H_
#define _PROXYSERVER_H_

#include <mutex>
#include <unordered_map>
#include <functional>

#include "UA_BlackJack.pb.h"
#include "common.h"

using ua_blackjack::Request;
using ua_blackjack::Response;

#define MAX_FORWORD_FAILURE 5

#define BUFFER_SIZE 1024 * 4     // 4 KB
#define QUEUE_SIZE  128

// forward declarations
class ServiceClient;
class Client;

namespace Net {
    class TcpServer;
    class TcpConnection;
    class EventLoop;
}


class ProxyServer {
public:
    // ip: which ip to bind
    // port: which port to listen
    // loop: pointer to the eventloop of the main thread
    ProxyServer(const char *ip, unsigned short port, Net::EventLoop *loop);
    
    // request: request from remote rpc caller
    int SendRequest(Request &request);

    // clientResponseCallBack: callback function when client sent response
    void SetClientResponseCallBack(const std::function<void(Response &)> &clientResponseCallBack)
    {clientResponseCallBack_ = clientResponseCallBack;}

    //  register rpc client
    void RegisterServiceClient(Request::RequestType requestType, std::shared_ptr<ServiceClient> client)
    {requestTypeToServiceClient_.emplace(requestType, client);}

    // response: response from rpc server
    void OnServiceResponse(Response& response);

// private:
    // conn: shared ptr to new tcp connection
    void OnNewClient(std::shared_ptr<Net::TcpConnection> conn);

    // fd: file descriptor for the tcp connection of the client
    // request: request from client
    void OnClientRequest(FileDesc fd, Request request);

    // response: response from client
    void OnClientResponse(Response response);

    // fd: file descriptor for the tcp connection of the client
    void OnDisConnection(FileDesc fd);

    void OnError(FileDesc fd);

private:
    std::mutex uidToClientLock_;
    std::unordered_map<UserId, std::shared_ptr<Client>> uidToClient_;   
    std::unordered_map<FileDesc, std::shared_ptr<Client>> fdToClient_;  // only accessible from the main thread, no lock protected
    std::mutex stampToUnloginClientLock_;
    std::unordered_map<int64_t, std::weak_ptr<Client>> stampToUnloginClient_;
    std::mutex stampToSignupClientLock_;
    std::unordered_map<int64_t, std::weak_ptr<Client>> stampToSignupClient_;
    std::unordered_map<Request::RequestType, std::weak_ptr<ServiceClient>> requestTypeToServiceClient_;
    std::function<void(Response &)> clientResponseCallBack_;
    std::shared_ptr<Net::TcpServer> server_;
};

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
    {Request::LEAVE_ROOM, BackEndModule::Lobby},
    // Room request
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

#endif