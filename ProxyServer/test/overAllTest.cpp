#include <string>
#include <stdarg.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <atomic>
#include <vector>
#include <chrono>
// #include "ClientProxyProtocol.h"
#include "co_routine.h"
#include "UA_BlackJack.pb.h"
#include "common.h"

using ua_blackjack::Request;
using ua_blackjack::Response;

typedef std::chrono::steady_clock SteadyClock;
typedef std::chrono::time_point<SteadyClock> TimePoint;
typedef std::chrono::milliseconds MilliSeconds;

std::atomic<int> g_active_player;

int need_to_stop(void *arg)
{
    return static_cast<std::atomic<int>*>(arg)->load() == 0 ? -1 : 0;
}

static int setNonBlocking(int fd)
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}


struct Player
{
    stCoRoutine_t *co_ = NULL;
    int64_t playerId_ = -1;
    UserId uid_ = -1;
    FileDesc fd_ = -1;
    int requestSent_ = 0;
    double avgDelay_ = 0.0;
    bool successful_ = false;
};

#define PACKAGE_HDR_LEN 8
#define REQUEST 1
#define RESPONSE 2

void handle_failure(FileDesc fd)
{
    close(fd);
    g_active_player--;
}

// this function always assume that the size of the buffer is larger than 8
const char *unpack(const char *buf, int size)
{
    int32_t type = ntohl(*(int32_t*)buf);
    int32_t msgLen = ntohl(*(int32_t*)(buf + 4));

    if (msgLen + PACKAGE_HDR_LEN > size)
    {
        return NULL;
    }
    return buf + PACKAGE_HDR_LEN;
}

void prepareRequest(Request &request, Request::RequestType requestType, int64_t uid, int64_t stamp, std::vector<std::string> args)
{
    request.set_requesttype(requestType);
    request.set_uid(uid);
    request.set_stamp(stamp);
    request.clear_args();
    for (int i = 0; i < args.size(); ++i)
    {
        request.add_args(std::move(args[i]));
    }
}

bool sendRequest(FileDesc fd, const Request &request, char *buffer, int size)
{
    if (request.ByteSizeLong() + PACKAGE_HDR_LEN > size)
    {
        return false;
    }
    int32_t msgType = htonl(REQUEST);
    int32_t msgLen = htonl(request.ByteSizeLong());
    *(int32_t*)buffer = msgType;
    *(int32_t*)(buffer + 4) = msgLen;
    request.SerializeToArray(buffer + PACKAGE_HDR_LEN, size - PACKAGE_HDR_LEN);

    int byteToSend = request.ByteSizeLong() + PACKAGE_HDR_LEN;
    int byteHasSent = 0;
    while (byteHasSent < byteToSend)
    {
        int ret = write(fd, buffer + byteHasSent, byteToSend - byteHasSent);
        if (ret < 0 && errno == EAGAIN)
        {
            struct pollfd pf = {0};
            pf.fd = fd;
            pf.events = (POLLOUT | POLLERR | POLLHUP);
            co_poll(co_get_epoll_ct(), &pf, 1, 500);
            continue;
        }
        else if (ret < 0)
            return false;
        byteHasSent += ret;
    }
    return true;
}

bool receiveResponse(FileDesc fd, Response &response, char *buffer, int size)
{   
    int byteRead = 0;
    while (byteRead < 8)
    {
        int ret = read(fd, buffer + byteRead, size - byteRead);
        if (ret < 0 && errno == EAGAIN)
        {
            struct pollfd pf = {0};
            pf.fd = fd;
            pf.events = (POLLIN | POLLERR | POLLHUP);
            co_poll(co_get_epoll_ct(), &pf, 1, 500);
            continue;
        }
        else if (ret <= 0)
            return false;
        byteRead += ret;
    }
    // parse result
    int32_t msgType = ntohl(*(int32_t*)buffer);
    int32_t msgLen = ntohl(*(int32_t*)(buffer + 4));
    if (msgType != RESPONSE || msgLen != byteRead - PACKAGE_HDR_LEN)
        return false;
    response.clear_args();
    response.ParseFromArray(buffer + PACKAGE_HDR_LEN, byteRead - PACKAGE_HDR_LEN);
    return true;
}

bool sendRequestRecvResponse(FileDesc fd, const Request &request, Response &response, char *buffer, int size)
{
    if (!sendRequest(fd, request, buffer, size))
    {
        std::cout << "fail at send request." <<std::endl;
        handle_failure(fd);
        return false;
    }
    if (!receiveResponse(fd, response, buffer, size))
    {
        std::cout << "fail at receive response." <<std::endl;
        handle_failure(fd);
        return false;
    }
    return true;
}

static void *LobbyTestRoutine(void *arg)
{
    co_enable_hook_sys();

    char buffer[2048];

    Player *player = static_cast<Player*>(arg);
    Request request;
    Response response;
    int64_t roomId;
    TimePoint begin, end;
    double delay = 0.0;
    int requestSent = 0;
STEP1:
    // login
    prepareRequest(request, Request::LOGIN, 0, 0, {"player_" + std::to_string(player->playerId_), "pass"});
    begin = SteadyClock::now();
    if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer))
        || response.status() < 0)
    {
        std::cout << "plyer_" << player->playerId_ << " fail at step 1" << std::endl;
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    ++requestSent;
    player->uid_ = response.uid();
STEP2:
    // roomlist
    prepareRequest(request, Request::ROOM_LIST, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer))
        || response.status() < 0)
    {
        std::cout << "plyaer_" << player->playerId_ << " fail at step 2" << std::endl;
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    if (response.args_size() > 0)
    {
        roomId = std::stoi(response.args(player->fd_ % response.args_size()));
    }
    else
    {
        goto STEP5;
    }
STEP3:
    // quick match
    prepareRequest(request, Request::QUICK_MATCH, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)))
    {
        std::cout << "plyaer_" << player->playerId_ << " fail at step 3" << std::endl;
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    if (response.status() > -1)
    {
        goto STEP7;
    }
STEP4:
    // create room
    prepareRequest(request, Request::CREATE_ROOM, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer))
        || response.status() < 0 || response.args_size() < 1)
    {
        std::cout << "plyaer_" << player->playerId_ << " fail at step 4" << std::endl;
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    roomId = std::stoi(response.args(0));
STEP5:
    // join room
    prepareRequest(request, Request::JOIN_ROOM, player->uid_, 0, {std::to_string(roomId)});
    begin = SteadyClock::now();
    if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)))
    {
        std::cout << "plyaer_" << player->playerId_ << " fail at step 5" << std::endl;
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    if (response.status() < 0)
    {
        goto STEP7;
    }
STEP6:
    // LEAVE room
    prepareRequest(request, Request::LEAVE_ROOM, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer))
        || response.status() < 0)
    {
        std::cout << "plyaer_" << player->playerId_ << " fail at step 6" << std::endl;
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
STEP7:
    // LOGOUT room
    prepareRequest(request, Request::LOGOUT, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)))
    {
        close(player->fd_);
        g_active_player--;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    player->requestSent_ = ++requestSent;
    player->successful_ = true;
    player->avgDelay_ = delay;
    return 0;
}

static void *SocialTestRoutine(void *arg)
{
    co_enable_hook_sys();

    char buffer[2048];

    Player *player = static_cast<Player*>(arg);
    Request request;
    Response response;
    TimePoint begin, end;
    double delay = 0.0;
    int requestSent = 0;

    std::vector<Request::RequestType> types = {Request::LOGIN, Request::RANK_ME, 
                                                Request::RANK_TOP, Request::LIST_MATCH,
                                                Request::GET_MATCH_INFO};
    std::vector<std::vector<std::string>> args_vec = {{"player_" + std::to_string(player->playerId_), "pass"},
                                                      {}, {"5"}, {}, {"1"}};

    for (int i = 0; i < types.size(); ++i)
    {
        prepareRequest(request, types[i], player->uid_, 0, args_vec[i]);
        begin = SteadyClock::now();
        if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer))
            || response.status() < 0)
        {
            std::cout << "player_" << player->playerId_ << " fail at step " << i + 1 << std::endl;
            return 0;
        }
        end = SteadyClock::now();
        delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
        ++requestSent;
        if (types[i] == Request::LOGIN) player->uid_ = response.uid();
    }
    player->requestSent_ = requestSent;
    player->successful_ = true;
    player->avgDelay_ = delay;
    close(player->fd_);
    g_active_player--;
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cout << "usage: " << std::string(argv[0]) << " ip port playerno." << std::endl;
        return 0;
    }
    struct sockaddr_in hostAddr;
    hostAddr.sin_family = AF_INET;
    hostAddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &hostAddr.sin_addr);

    int nConnected = 0, offset = 0;
    std::vector<Player> players(atoi(argv[3]));
    if (argc == 5)
    {
        offset = atoi(argv[4]);
    }

    for (int i = 0; i < players.size(); ++i)
    {
        players[i].playerId_ = i + offset;
        if ((players[i].fd_ = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
        {
            if (-1 < connect(players[i].fd_, (struct sockaddr*)&hostAddr, sizeof(hostAddr)))
            {
                setNonBlocking(players[i].fd_);
                nConnected++;
                continue;
            }
            players[i].fd_ = -1;
            close(players[i].fd_);
        }
    }
    std::cout << nConnected << " players connected to server." << std::endl;
    if (nConnected == 0)
    {
        return 0;
    }

    g_active_player = nConnected;
    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].fd_ > -1)
        {
            co_create(&(players[i].co_), NULL, SocialTestRoutine, &players[i]);
            co_resume(players[i].co_);
        }
    }
    TimePoint begin = SteadyClock::now();

    co_eventloop(co_get_epoll_ct(), need_to_stop, &g_active_player);

    TimePoint end = SteadyClock::now();
    // compute the average delay  
    int time_tot = std::chrono::duration_cast<MilliSeconds>(end - begin).count();
    double delay = 0.0;
    int cnt = 0;
    int requestSent = 0;
    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].successful_) 
        {
            cnt++;
            requestSent += players[i].requestSent_;
            delay += players[i].avgDelay_;
        }
    }
    if (cnt > 0)
    {   
        delay /= cnt;
        std::cout << "Lobby: QPS = " << (requestSent * 1000) / (time_tot) << ", average delay = " << delay << "ms" << std::endl;
    }
    else
    {
        std::cout << "no player finish the test." << std::endl;
    }
    exit(0);
}