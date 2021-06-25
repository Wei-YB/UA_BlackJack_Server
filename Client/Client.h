#ifndef CLIENT_H
#define CLIENT_H

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "client.pb.h"

// #define MAX_EVENTS 5
#define BUFFSIZE 8192
#define MAX_PLAYER_NUM 6

class Client {
public:
    Client()
        : sfd_(-1),
          uid_(-1),
          rid_(-1),
          epfd_(-1),
          cmd2req_{{"SignUp", {proxy::Request::SIGNUP, 2}},
                   {"Login", {proxy::Request::LOGIN, 2}},
                   {"Logout", {proxy::Request::LOGOUT, 0}},
                   {"RoomList", {proxy::Request::ROOM_LIST, 0}},
                   {"JoinRoom", {proxy::Request::JOIN_ROOM, 1}},
                   {"CreateRoom", {proxy::Request::CREATE_ROOM, 0}},
                   {"QuickMatch", {proxy::Request::QUICK_MATCH, 0}},
                   {"Ready", {proxy::Request::READY, 0}},
                   {"LeaveRoom", {proxy::Request::LEAVE_ROOM, 0}},
                   {"Bet", {proxy::Request::BET, 1}},
                   {"Hit", {proxy::Request::HIT, 0}},
                   {"Stand", {proxy::Request::STAND, 0}},
                   {"Double", {proxy::Request::DOUBLE, 0}},
                   {"Surrender", {proxy::Request::SURRENDER, 0}},
                   {"Info", {proxy::Request::INFO, 0}},
                   {"RankMe", {proxy::Request::RANK_ME, 0}},
                   {"RankTop", {proxy::Request::RANK_TOP, 1}},
                   {"AddFriend", {proxy::Request::ADD_FRIEND, 1}},
                   {"AcceptFriend", {proxy::Request::ACCEPT_FRIEND, 1}},
                   {"DeleteFriend", {proxy::Request::DELETE_FRIEND, 1}},
                   {"FriendList", {proxy::Request::LIST_FRIEND, 0}},
                   {"WaitingFriendList", {proxy::Request::LIST_WAITTING, 0}}},
          state_(OFFLINE),
          state2str_{{INVALID, "INVALID"},
                     {OFFLINE, "OFFLINE"},
                     {ONLINE, "ONLINE"},
                     {INROOM_UNREADY, "INROOM_UNREADY"},
                     {INROOM_READY, "INROOM_READY"},
                     {INGAME_IDLE, "INGAME_IDLE"},
                     {INGAME_TURN, "INGAME_TURN"}},
          room_request_(nullptr),
          idx_(0) {}

    void Run();

private:
    int sfd_;   // server fd
    int uid_;   // user id
    int rid_;   // room id
    int epfd_;  // epoll fd
    std::unordered_map<std::string, std::pair<proxy::Request::RequestType, int>> cmd2req_;
    enum State { INVALID, OFFLINE, ONLINE, INROOM_UNREADY, INROOM_READY, INGAME_IDLE, INGAME_TURN } state_;
    enum DataType { REQUEST = 1, RESPONSE = 2 };
    std::unordered_map<State, std::string> state2str_;

    int Connect(const char* host, const char* service, int type);

    void GameInit();
    static void* ListenProxy(void* arg);
    void GameEnd(proxy::Request& request);
    void PrintPrompt(const std::string& prompt);

    std::vector<std::string> GetCommandArgs();
    std::vector<std::string> Parse(const std::string& command);

    State GetNextState(const std::string& cmd);   // for command line input
    State GetNextState(proxy::Request& request);  // for server request input

    proxy::Request ConstructRequest(const std::vector<std::string>& args);
    int SerializeRequest(proxy::Request& request);

    proxy::Response ConstructRoomResponse(bool valid, proxy::Request& request);
    int SerializeRoomResponse(proxy::Response& response);

    void ProcessResponse(int64_t stamp);
    void ProcessRoomRequest(proxy::Request& request);

    void UpdateCards(proxy::Request& request);
    std::vector<int> ParseCards(const std::string& str);

    unsigned char buffer_in_[BUFFSIZE];
    unsigned char buffer_out_[BUFFSIZE];

    int idx_;
    std::vector<std::pair<int, int>> cards_[MAX_PLAYER_NUM];
    std::unordered_map<int, int> uid2idx_;
    std::unordered_map<int, int> idx2uid_;

    /*****should be thread safe, use mutex*****/
    std::unordered_map<int64_t, proxy::Response> stamp2response_;
    proxy::Request* room_request_;
    /******************************************/

    void addfd(int epollfd, int fd) {
        epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    }

    void removefd(int epollfd, int fd) {
        epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
        close(fd);
    }
};

#endif