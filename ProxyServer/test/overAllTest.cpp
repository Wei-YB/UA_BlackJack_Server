#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <string>
#include <vector>
// #include "ClientProxyProtocol.h"
#include "ProxyServer.h"
#include "UA_BlackJack.pb.h"
#include "co_routine.h"
#include "common.h"
#include "log.h"

using ua_blackjack::Request;
using ua_blackjack::Response;

typedef std::chrono::steady_clock SteadyClock;
typedef std::chrono::time_point<SteadyClock> TimePoint;
typedef std::chrono::milliseconds MilliSeconds;

std::atomic<int> g_active_player;
int g_nPlayer = 0;

int need_to_stop(void *arg) { return static_cast<std::atomic<int> *>(arg)->load() == 0 ? -1 : 0; }

static int setNonBlocking(int fd) { return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK); }

struct VPlayer
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
    int32_t type = ntohl(*(int32_t *)buf);
    int32_t msgLen = ntohl(*(int32_t *)(buf + 4));

    if (msgLen + PACKAGE_HDR_LEN > size)
    {
        return NULL;
    }
    return buf + PACKAGE_HDR_LEN;
}

void prepareRequest(Request &request, Request::RequestType requestType, int64_t uid, int64_t stamp,
                    std::vector<std::string> args)
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

void prepareResponse(Response &response, int64_t status, int64_t uid, int64_t stamp,
                    std::vector<std::string> args)
{
    response.set_status(status);
    response.set_uid(uid);
    response.set_stamp(stamp);
    response.clear_args();
    for (int i = 0; i < args.size(); ++i)
    {
        response.add_args(std::move(args[i]));
    }
}

template<typename T>
bool sendMsg(FileDesc fd, int type, const T &msg, char *buffer, int size)
{
    if (msg.ByteSizeLong() + PACKAGE_HDR_LEN > size)
    {
        return false;
    }
    int32_t msgType = htonl(type);
    int32_t msgLen = htonl(msg.ByteSizeLong());
    *(int32_t *)buffer = msgType;
    *(int32_t *)(buffer + 4) = msgLen;
    msg.SerializeToArray(buffer + PACKAGE_HDR_LEN, size - PACKAGE_HDR_LEN);

    int byteToSend = msg.ByteSizeLong() + PACKAGE_HDR_LEN;
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

template<typename T>
bool recvMsg(FileDesc fd, int type, T &msg, char *buffer, int size)
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
    int32_t msgType = ntohl(*(int32_t *)buffer);
    int32_t msgLen = ntohl(*(int32_t *)(buffer + 4));
    if (type != msgType || msgLen != byteRead - PACKAGE_HDR_LEN)
        return false;
    msg.clear_args();
    msg.ParseFromArray(buffer + PACKAGE_HDR_LEN, byteRead - PACKAGE_HDR_LEN);
    return true;
}

template<typename T1, typename T2>
bool sendAndRecvMsg(FileDesc fd, int type1, T1 &msg1, int type2, T2 &msg2, char *buffer, int size)
{
    if (!sendMsg(fd, type1, msg1, buffer, size))
    {
        logger_ptr->error("fail at send msg.");
        handle_failure(fd);
        return false;
    }
    if (!recvMsg(fd, type2, msg2, buffer, size))
    {
        logger_ptr->error("fail at recv msg.");
        handle_failure(fd);
        return false;
    }
    return true;
}


// static void *LobbyTestRoutine(void *arg)
// {
//     co_enable_hook_sys();

//     char buffer[2048];

//     VPlayer *player = static_cast<VPlayer *>(arg);
//     Request request;
//     Response response;
//     int64_t roomId;
//     TimePoint begin, end;
//     double delay = 0.0;
//     int requestSent = 0;
// STEP1:
//     // login
//     prepareRequest(request, Request::LOGIN, 0, 0, {"player_" + std::to_string(player->playerId_), "pass"});
//     begin = SteadyClock::now();
//     if (!senAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)) || response.status() < 0)
//     {
//         std::cout << "plyer_" << player->playerId_ << " fail at step 1" << std::endl;
//         return 0;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     ++requestSent;
//     player->uid_ = response.uid();
// STEP2:
//     // roomlist
//     prepareRequest(request, Request::ROOM_LIST, player->uid_, 0, {});
//     begin = SteadyClock::now();
//     if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)) || response.status() < 0)
//     {
//         std::cout << "plyaer_" << player->playerId_ << " fail at step 2" << std::endl;
//         return 0;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
//     if (response.args_size() > 0)
//     {
//         roomId = std::stoi(response.args(player->fd_ % response.args_size()));
//     }
//     else
//     {
//         goto STEP5;
//     }
// STEP3:
//     // quick match
//     prepareRequest(request, Request::QUICK_MATCH, player->uid_, 0, {});
//     begin = SteadyClock::now();
//     if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)))
//     {
//         std::cout << "plyaer_" << player->playerId_ << " fail at step 3" << std::endl;
//         return 0;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
//     if (response.status() > -1)
//     {
//         goto STEP7;
//     }
// STEP4:
//     // create room
//     prepareRequest(request, Request::CREATE_ROOM, player->uid_, 0, {});
//     begin = SteadyClock::now();
//     if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)) || response.status() < 0 ||
//         response.args_size() < 1)
//     {
//         std::cout << "plyaer_" << player->playerId_ << " fail at step 4" << std::endl;
//         return 0;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
//     roomId = std::stoi(response.args(0));
// STEP5:
//     // join room
//     prepareRequest(request, Request::JOIN_ROOM, player->uid_, 0, {std::to_string(roomId)});
//     begin = SteadyClock::now();
//     if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)))
//     {
//         std::cout << "plyaer_" << player->playerId_ << " fail at step 5" << std::endl;
//         return 0;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
//     if (response.status() < 0)
//     {
//         goto STEP7;
//     }
// STEP6:
//     // LEAVE room
//     prepareRequest(request, Request::LEAVE_ROOM, player->uid_, 0, {});
//     begin = SteadyClock::now();
//     if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)) || response.status() < 0)
//     {
//         std::cout << "plyaer_" << player->playerId_ << " fail at step 6" << std::endl;
//         return 0;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
// STEP7:
//     // LOGOUT room
//     prepareRequest(request, Request::LOGOUT, player->uid_, 0, {});
//     begin = SteadyClock::now();
//     if (sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)))
//     {
//         close(player->fd_);
//         g_active_player--;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     player->requestSent_ = ++requestSent;
//     player->successful_ = true;
//     player->avgDelay_ = delay;
//     return 0;
// }

// static void *PlayerTestRoutine(void *arg)
// {
//     co_enable_hook_sys();

//     char buffer[2048];

//     VPlayer *player = static_cast<VPlayer *>(arg);
//     Request request;
//     Response response;
//     TimePoint begin, end;
//     double delay = 0.0;
//     int requestSent = 0;

//     std::vector<Request::RequestType> types = {Request::LOGIN, Request::RANK_ME, Request::RANK_TOP,
//                                                Request::LIST_MATCH, Request::GET_MATCH_INFO, Request::LOGOUT};
//     std::vector<std::vector<std::string>> args_vec = {
//         {"player_" + std::to_string(player->playerId_), "pass"}, {}, {"5"}, {}, {"1"}, {}};

//     for (int i = 0; i < types.size(); ++i)
//     {
//         prepareRequest(request, types[i], player->uid_, 0, args_vec[i]);
//         begin = SteadyClock::now();
//         if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)))
//         { // fail because of IO
//             std::cout << "player_" << player->playerId_ << " fail at " << requestTypeToStr[types[i]] << std::endl;
//             return 0;
//         }
//         if (response.status() < 0)
//         {
//             handle_failure(player->fd_);
//             return 0;
//         }
//         end = SteadyClock::now();
//         delay =
//             (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//         ++requestSent;
//         if (types[i] == Request::LOGIN)
//             player->uid_ = response.uid();
//     }
//     player->requestSent_ = requestSent;
//     player->successful_ = true;
//     player->avgDelay_ = delay;
//     close(player->fd_);
//     g_active_player--;
//     return 0;
// }

// static void *SocialTestRoutine(void *arg)
// {
//     co_enable_hook_sys();

//     char buffer[2048];

//     VPlayer *player = static_cast<VPlayer *>(arg);
//     Request request;
//     Response response;
//     TimePoint begin, end;
//     double delay = 0.0;
//     int requestSent = 0;

//     std::vector<Request::RequestType> types = {Request::LOGIN, Request::ADD_FRIEND, Request::LIST_WAITTING,
//                                                Request::ACCEPT_FRIEND, Request::LIST_FRIEND, Request::DELETE_FRIEND,
//                                                Request::LOGOUT};
//     std::vector<std::vector<std::string>> args_vec = {{"player_" + std::to_string(player->playerId_), "pass"},
//                                                       {"player_" + std::to_string((player->playerId_ + 1) % g_nPlayer)},
//                                                       {},
//                                                       {},
//                                                       {},
//                                                       {},
//                                                       {}};
//     std::vector<std::string> waitingList;
//     std::vector<std::string> friendList;
//     for (int i = 0; i < types.size();)
//     {
//         if (types[i] == Request::ACCEPT_FRIEND)
//         {
//             if (waitingList.empty())
//             {
//                 ++i;
//                 continue;
//             }
//             for (int k = 0; k < waitingList.size(); ++k)
//                 args_vec[i].push_back(std::move(waitingList[k]));
//         }
//         else if (types[i] == Request::DELETE_FRIEND)
//         {
//             if (friendList.empty())
//             {
//                 ++i;
//                 continue;
//             }
//             args_vec[i].clear();
//             args_vec[i].push_back(friendList.back());
//             friendList.pop_back();
//         }

//         prepareRequest(request, types[i], player->uid_, 0, args_vec[i]);
//         begin = SteadyClock::now();
//         if (!sendRequestRecvResponse(player->fd_, request, response, buffer, sizeof(buffer)))
//         { // fail because of IO
//             logger_ptr->error("player_{0} fail at send/recv {1}", player->playerId_, requestTypeToStr[types[i]]);
//             return 0;
//         }
//         if ((types[i] == Request::LOGIN) && response.status() < 0)
//         {
//             logger_ptr->error("player_{0} fail at {1}", player->playerId_, requestTypeToStr[types[i]]);
//             handle_failure(player->fd_);
//             return 0;
//         }
//         end = SteadyClock::now();
//         delay =
//             (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//         ++requestSent;
//         if (types[i] == Request::LOGIN)
//             player->uid_ = response.uid();
//         else if (types[i] == Request::LIST_WAITTING)
//         {
//             for (int k = 0; k < response.args_size(); ++k)
//                 waitingList.emplace_back(std::move(response.args(k)));
//         }
//         else if (types[i] == Request::LIST_FRIEND)
//         {
//             for (int k = 0; k < response.args_size(); ++k)
//                 friendList.emplace_back(std::move(response.args(k)));
//         }
//         if (types[i] != Request::DELETE_FRIEND || friendList.empty())
//         {
//             ++i;
//         }
//     }
//     player->requestSent_ = requestSent;
//     player->successful_ = true;
//     player->avgDelay_ = delay;
//     close(player->fd_);
//     g_active_player--;
//     return 0;
// }

static void *GameTestRoutine(void *arg)
{
    co_enable_hook_sys();

    char buffer[2048];

    VPlayer *player = static_cast<VPlayer *>(arg);
    Request request;
    Response response;
    TimePoint begin, end;
    double delay = 0.0;
    int requestSent = 0;
    std::vector<std::string> roomList;

STEP1:
    prepareRequest(request, Request::LOGIN, 0, 0, {"player_" + std::to_string(player->playerId_), "pass"});
    begin = SteadyClock::now();
    if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{0} fail at Login.", player->playerId_);
        return 0;
    }
    if (response.status() < 0)
    {
        logger_ptr->error("player_{0} fail at Login.", player->playerId_);
        handle_failure(player->fd_);
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    player->uid_ = response.uid();
STEP2:
    prepareRequest(request, Request::QUICK_MATCH, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{0} fail at sending QUICK MATCH.", player->playerId_);
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    if (response.status() > -1)
        goto STEP6;
STEP3:
    prepareRequest(request, Request::ROOM_LIST, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{0} fail at sending ROOM LIST.");
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    if (response.status() < 0 || response.args_size() == 0)
        goto STEP11;
    for (int i = 0; i < response.args_size(); ++i)
    {
        roomList.push_back(std::move(response.args(i)));
    }
STEP4:
    for (auto rid : roomList)
    {
        prepareRequest(request, Request::JOIN_ROOM, player->uid_, 0, {rid});
        begin = SteadyClock::now();
        if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
        {
            logger_ptr->error("player_{0} fail at sending JOIN_ROOM.");
            return 0;
        }
        end = SteadyClock::now();
        delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
        requestSent++;
        if (response.status() > -1)
            goto STEP5;
    }
    logger_ptr->error("player_{0} fail at to JOIN_ROOM, logout.");
    goto STEP11;
    return 0;
STEP5:
    prepareRequest(request, Request::READY, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{0} fail at sending ready.");
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    if (response.status() < 0)
        goto STEP10;
STEP6:  // recv bet request
    prepareResponse(response, 1, player->uid_, 0, {"Bet", "100"});
    begin = SteadyClock::now();
    if (!recvMsg(player->fd_, REQUEST, request, buffer, sizeof(buffer)) 
        || request.args_size() == 0 || request.args(0) != "start")
    {
        logger_ptr->error("player_{} fail to recv bet requets from server", player->playerId_);
        handle_failure(player->fd_);
        return 0;
    }
    if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{} fail to send bet response to server", player->playerId_);
        handle_failure(player->fd_);
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
STEP7:  // recv hit request
    prepareResponse(response, 1, player->uid_, 0, {"update"});
    while (true)
    {
        begin = SteadyClock::now();
        if (!recvMsg(player->fd_, REQUEST, request, buffer, sizeof(buffer)) 
            || request.args_size() == 0)
        {
            logger_ptr->error("player_{} fail to recv hit requets from server", player->playerId_);
            handle_failure(player->fd_);
            return 0;
        }
        if (request.args(0) == "update")
        {
            if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
            {
                logger_ptr->error("player_{} fail to send update response to server", player->playerId_);
                handle_failure(player->fd_);
                return 0;
            }
            end = SteadyClock::now();
            delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
            requestSent++;
        }
        else
        {
            break;
        }
    }
    prepareResponse(response, 1, player->uid_, 0, {"Hit"});
    if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{} fail to send hit response to server", player->playerId_);
        handle_failure(player->fd_);
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
STEP8:  // recv stand stand request
    prepareResponse(response, 1, player->uid_, 0, {"update"});
    while (true)
    {
        begin = SteadyClock::now();
        if (!recvMsg(player->fd_, REQUEST, request, buffer, sizeof(buffer)) 
            || request.args_size() == 0)
        {
            logger_ptr->error("player_{} fail to recv stand requets from server", player->playerId_);
            handle_failure(player->fd_);
            return 0;
        }
        if (request.args(0) == "update")
        {
            if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
            {
                logger_ptr->error("player_{} fail to send update response to server", player->playerId_);
                handle_failure(player->fd_);
                return 0;
            }
            end = SteadyClock::now();
            delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
            requestSent++;
        }
        else
        {
            break;
        }
    }
    if (request.args(0) == "hit")
    {
        prepareResponse(response, 1, player->uid_, 0, {"Stand"});
    }
    else
    {
        prepareResponse(response, 1, player->uid_, 0, {"end"});
    }
    if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{} fail to send stand/end response to server", player->playerId_);
        handle_failure(player->fd_);
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    if (request.args(0) == "end")
    {
        goto STEP10;
    }
STEP9:  // recv stand stand request
    prepareResponse(response, 1, player->uid_, 0, {"update"});
    while (true)
    {
        begin = SteadyClock::now();
        if (!recvMsg(player->fd_, REQUEST, request, buffer, sizeof(buffer)) 
            || request.args_size() == 0)
        {
            logger_ptr->error("player_{} fail to recv stand requets from server", player->playerId_);
            handle_failure(player->fd_);
            return 0;
        }
        if (request.args(0) == "update")
        {
            if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
            {
                logger_ptr->error("player_{} fail to send update response to server", player->playerId_);
                handle_failure(player->fd_);
                return 0;
            }
            end = SteadyClock::now();
            delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
            requestSent++;
        }
        else
        {
            break;
        }
    }
    prepareResponse(response, 1, player->uid_, 0, {"end"});
    if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{} fail to send end response to server", player->playerId_);
        handle_failure(player->fd_);
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
STEP10:  // send leave room
    prepareRequest(request, Request::LEAVE_ROOM, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{0} fail at sending LEAVE ROOM.");
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
STEP11:
    prepareRequest(request, Request::LOGOUT, player->uid_, 0, {});
    begin = SteadyClock::now();
    if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
    {
        logger_ptr->error("player_{0} fail at sending logout.");
        return 0;
    }
    end = SteadyClock::now();
    delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
    requestSent++;
    player->requestSent_ = requestSent;
    player->successful_ = true;
    player->avgDelay_ = delay;
    close(player->fd_);
    g_active_player--;
    return 0;
}

static void initSockAddr(struct sockaddr_in &addr, const char *ip, unsigned short port)
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
}

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        std::cout << "usage: " << std::string(argv[0]) << " ip port playerno log_path." << std::endl;
        return 0;
    }

    set_logger_name("test_logger");
    set_log_path(std::string(argv[4]));
    create_logger();
    set_log_level("info");

    struct sockaddr_in hostAddr;
    initSockAddr(hostAddr, argv[1], atoi(argv[2]));

    std::vector<VPlayer> players(atoi(argv[3]));
    g_nPlayer = players.size();

    int nConnected = 0;
    for (int i = 0; i < players.size(); ++i)
    {
        players[i].playerId_ = i;
        if ((players[i].fd_ = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
        {
            if (-1 < connect(players[i].fd_, (struct sockaddr *)&hostAddr, sizeof(hostAddr)))
            {
                setNonBlocking(players[i].fd_);
                nConnected++;
                continue;
            }
            players[i].fd_ = -1;
            close(players[i].fd_);
        }
    }
    logger_ptr->info("{}  players connected to server.", nConnected);
    // std::cout << nConnected << " players connected to server." << std::endl;
    if (nConnected == 0)
    {
        return 0;
    }

    TimePoint begin = SteadyClock::now();
    g_active_player = nConnected;
    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].fd_ > -1)
        {
            co_create(&(players[i].co_), NULL, GameTestRoutine, &players[i]);
            co_resume(players[i].co_);
        }
    }

    co_eventloop(co_get_epoll_ct(), need_to_stop, &g_active_player);

    TimePoint end = SteadyClock::now();
    // compute the average delay
    int time_tot = std::chrono::duration_cast<MilliSeconds>(end - begin).count();
    double delay = 0.0;
    int cnt = 0;
    int requestSent = 0;
    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].requestSent_ > 0)
        {
            cnt++;
            requestSent += players[i].requestSent_;
            delay += players[i].avgDelay_;
        }
    }
    if (cnt > 0)
    {
        delay /= cnt;
        std::cout << "QPS = " << (requestSent * 1000) / (time_tot) << ", average delay = " << delay << "ms"
                  << std::endl;
    }
    else
    {
        std::cout << "no player finish the test." << std::endl;
    }
    exit(0);
}