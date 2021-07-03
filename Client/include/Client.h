#ifndef CLIENT_H
#define CLIENT_H

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
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

#include "Display.h"
#include "Rio.h"
#include "UA_BlackJack.pb.h"
#include "global.h"

#define MAX_EVENTS 5
#define BUFFSIZE 8192
#define MAX_PLAYER_NUM 6
#define TIME_OUT 5

namespace ua_blackjack {
namespace client {
class Client {
public:
    friend class Display;

    Client();

    void Run(const char *ip, const char *port);
    std::string GetState() { return state2str_[state_]; }

private:
    int sfd_;   // server fd
    int uid_;   // user id
    int rid_;   // room id
    int epfd_;  // epoll fd
    std::string name_;
    std::string name_tmp_;
    bool dealer_;
    std::unordered_map<std::string, std::pair<ua_blackjack::Request::RequestType, int>> cmd2req_;
    enum State { INVALID, OFFLINE, ONLINE, INROOM_UNREADY, INROOM_READY, INGAME_IDLE, INGAME_TURN } state_, next_state_;
    enum DataType { REQUEST = 1, RESPONSE = 2 };
    std::unordered_map<State, std::string> state2str_;

    int Connect(const char *host, const char *service, int type);

    void GameInit();
    static void *ListenProxy(void *arg);
    void GameEnd(ua_blackjack::Request &request);

    void ProcessCommand(ua_blackjack::robust_io::Rio &rio);
    void ProcessSocket(ua_blackjack::robust_io::Rio &rio);

    std::vector<std::string> GetCommandArgs();
    std::vector<std::string> Parse(const std::string &command);

    State GetNextState(const std::string &cmd);          // for command line input
    State GetNextState(ua_blackjack::Request &request);  // for server request input

    ua_blackjack::Request ConstructRequest(const std::vector<std::string> &args);
    int SerializeRequest(ua_blackjack::Request &request);

    ua_blackjack::Response ConstructRoomResponse(bool valid, ua_blackjack::Request &request);
    int SerializeRoomResponse(ua_blackjack::Response &response);

    void ProcessResponse(ua_blackjack::Response &reponse);
    void ProcessRoomRequest(ua_blackjack::Request &request);

    void UpdateCards(ua_blackjack::Request &request);
    std::vector<int> ParseCards(const std::string &str);

    void addfd(int epollfd, int fd);
    void removefd(int epollfd, int fd);

    bool DealTimeout(ua_blackjack::Response &response);

    unsigned char buffer_in_[BUFFSIZE];
    unsigned char buffer_out_[BUFFSIZE];

    int idx_;
    std::vector<std::pair<int, int>> cards_[MAX_PLAYER_NUM];
    std::unordered_map<std::string, int> name2idx_;
    std::unordered_map<int, std::string> idx2name_;

    ua_blackjack::Request::RequestType request_type_;
};
}  // namespace client

}  // namespace ua_blackjack

#endif