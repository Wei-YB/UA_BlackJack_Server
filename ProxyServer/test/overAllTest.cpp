#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

#include <atomic>
#include <chrono>
#include <string>
#include <vector>
// #include "ClientProxyProtocol.h"
#include "ProxyServer.h"
#include "UA_BlackJack.pb.h"
#include "co_routine.h"
#include "common.h"
#include "ClientProxyProtocol.h"
#include "CircularBuffer.h"
#include "log.h"

using ua_blackjack::Request;
using ua_blackjack::Response;
using Net::CircularBuffer;

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
        {
            logger_ptr->error(std::string(strerror(errno)));
            return false;
        }
        byteHasSent += ret;
    }
    return true;
}

bool recvMsg(FileDesc fd, 
             std::vector<Request> &requests, 
             std::vector<Response> &responses, 
             char *buffer, int size)
{
    int byteToRead = 0;
    int byteRead = 0;
    int pkgCnt = 0;
    int failCnt = 0;
    bool needToRead = true;
    while (byteRead < 8 || byteRead < byteToRead)
    {
        int ret = read(fd, buffer + byteRead, size - byteRead);
        if (ret < 0)
        {
            if (errno == EAGAIN)
            {
                if (failCnt++ > 5)
                    return false;
                struct pollfd pf = {0};
                pf.fd = fd;
                pf.events = (POLLIN | POLLERR | POLLHUP);
                co_poll(co_get_epoll_ct(), &pf, 1, 1000);
                continue;
            }
            logger_ptr->error("fatal error on fd {0}, details: {1}", fd, std::string(strerror(errno)));
            return false;
        }
        else if (ret == 0)
        {
            logger_ptr->warn("fd: {}, connection shutdown by peer.");
            return false;
        }
        byteRead += ret;
        // if the buffer is full, break the loop
        if (byteRead == size)
        {
            logger_ptr->warn("fd {} read data with size larger than the buffer, cannot handle it.");
            return false;
        }
        // read the first pkg hdr
        if (byteRead >= 8 && byteToRead == 0)
        {
            byteToRead = ntohl(*(int32_t*)(buffer + 4)) + 8;
            pkgCnt++;
        }
        // it should wait for the next read for the incomplete package
        else if (byteToRead != 0 && byteRead > byteToRead + 8)
        {
            byteToRead += ntohl(*(int32_t*)(buffer + byteToRead + 4)) + 8;
            pkgCnt++;
        }
    }
    logger_ptr->info("recv {0} bytes, {1} packages in total.", byteRead, pkgCnt);
    if (pkgCnt == 0)
        return false;
    // parse result
    int offset = 0;
    while (pkgCnt--)
    {
        int32_t msgType = ntohl(*(int32_t *)(buffer + offset));
        int32_t msgLen = ntohl(*(int32_t *)(buffer + 4 + offset));
        if (msgType == REQUEST)
        {
            Request request;
            request.ParseFromArray(buffer + offset + PACKAGE_HDR_LEN, msgLen);
            offset += PACKAGE_HDR_LEN + msgLen;
            requests.push_back(std::move(request));
        }
        else if (msgType == RESPONSE)
        {
            Response response;
            response.ParseFromArray(buffer + offset + PACKAGE_HDR_LEN, msgLen);
            offset += PACKAGE_HDR_LEN + msgLen;
            responses.push_back(std::move(response));
        }
        else
        {
            logger_ptr->warn("fd {} received an unknown type request.", fd);
            return false;
        }
    }
    return true;
}


template<typename T>
bool recvMsg(FileDesc fd, int type, T &msg, char *buffer, int size)
{
    int byteToRead = 0;
    int byteRead = 0;
    int pkgCnt = 0;
    while (byteRead < 8 || byteRead < byteToRead)
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
        else if (ret < 0)
        {
            logger_ptr->error(std::string(strerror(errno)));
            return false;
        }
        else if (ret == 0)
        {
            logger_ptr->error("server shutdown the connection");
            return false;
        }
        byteRead += ret;
        if (byteToRead != 0 && byteRead > byteToRead + 8)
        {
            byteToRead += ntohl(*(int32_t*)(buffer + byteToRead + 4)) + 8;
            pkgCnt++;
        }
        else if (byteRead >= 8 && byteToRead == 0)
        {
            byteToRead = ntohl(*(int32_t*)(buffer + 4)) + 8;
        }
    }
    logger_ptr->info("recv {0} bytes, need to read {1} bytes", byteRead, byteToRead);
    // parse result
    int32_t msgType = ntohl(*(int32_t *)buffer);
    int32_t msgLen = ntohl(*(int32_t *)(buffer + 4));
    if (type != msgType || msgLen > byteRead - PACKAGE_HDR_LEN)
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
        logger_ptr->error("fd {} fail at send msg.", fd);
        return false;
    }
    if (!recvMsg(fd, type2, msg2, buffer, size))
    {
        logger_ptr->error("fd {} fail at recv msg.", fd);
        return false;
    }
    return true;
}

static void *LobbyTestRoutine(void *arg)
{
    co_enable_hook_sys();

    char buffer[2048];

    VPlayer *player = static_cast<VPlayer *>(arg);
    Request request;
    Response response;
    std::string roomId;
    TimePoint begin, end;
    double delay = 0.0;
    int requestSent = 0;

    std::vector<Request::RequestType> types = {Request::LOGIN, Request::ROOM_LIST, Request::QUICK_MATCH,
                                               Request::CREATE_ROOM, Request::JOIN_ROOM, Request::LEAVE_ROOM,
                                               Request::LOGOUT};
    std::vector<std::vector<std::string>> args_vec = {
        {"player_" + std::to_string(player->playerId_), "pass"}, {}, {}, {}, {}, {}, {}};

    for (int i = 0; i < types.size(); ++i)
    {
        if (types[i] == Request::JOIN_ROOM)
            prepareRequest(request, types[i], player->uid_, 0, {roomId});
        else if (types[i] == Request::QUICK_MATCH)
            continue;
        else
            prepareRequest(request, types[i], player->uid_, 0, args_vec[i]);
        begin = SteadyClock::now();
        if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
        {
            handle_failure(player->fd_);
            logger_ptr->error("plyer_{0} fail at sending {1}", player->playerId_, requestTypeToStr[types[i]]);
            return 0;
        }
        if (response.status() < 0)
        {   
            logger_ptr->warn("plyer_{0} fail at {1}", player->playerId_, requestTypeToStr[types[i]]);
            if (types[i] == Request::LOGIN)
            {
                handle_failure(player->fd_);
                return 0;
            }
            else if (types[i] == Request::CREATE_ROOM || types[i] == Request::JOIN_ROOM)
            {
                i = 5;
            }
        }
        else
        {
            if (types[i] == Request::QUICK_MATCH)
            {
                i = 4;
            }
            else if (types[i] == Request::CREATE_ROOM)
            {
                roomId = response.args(0);
            }
            else if (types[i] == Request::LOGIN)
            {
                player->uid_ = response.uid();
            }
        }
        end = SteadyClock::now();
        delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
        ++requestSent;
    }
    player->requestSent_ = requestSent;
    player->successful_ = true;
    player->avgDelay_ = delay;
    close(player->fd_);
    g_active_player--;
    return 0;
}

static void *PlayerTestRoutine(void *arg)
{
    co_enable_hook_sys();

    char buffer[2048];

    VPlayer *player = static_cast<VPlayer *>(arg);
    Request request;
    Response response;
    TimePoint begin, end;
    double delay = 0.0;
    int requestSent = 0;

    std::vector<Request::RequestType> types = {Request::LOGIN, Request::RANK_ME, Request::RANK_TOP,
                                               Request::LIST_MATCH, Request::GET_MATCH_INFO, Request::LOGOUT};
    std::vector<std::vector<std::string>> args_vec = {
        {"player_" + std::to_string(player->playerId_), "pass"}, {}, {"5"}, {}, {"1"}, {}};

    for (int i = 0; i < types.size(); ++i)
    {
        prepareRequest(request, types[i], player->uid_, 0, args_vec[i]);
        begin = SteadyClock::now();
        if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
        { // fail because of IO
            std::cout << "player_" << player->playerId_ << " fail at " << requestTypeToStr[types[i]] << std::endl;
            return 0;
        }
        if (response.status() < 0)
        {
            handle_failure(player->fd_);
            return 0;
        }
        end = SteadyClock::now();
        delay =
            (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
        ++requestSent;
        if (types[i] == Request::LOGIN)
            player->uid_ = response.uid();
    }
    player->requestSent_ = requestSent;
    player->successful_ = true;
    player->avgDelay_ = delay;
    close(player->fd_);
    g_active_player--;
    return 0;
}

static void *SocialTestRoutine(void *arg)
{
    co_enable_hook_sys();

    char buffer[2048];

    VPlayer *player = static_cast<VPlayer *>(arg);
    Request request;
    Response response;
    TimePoint begin, end;
    double delay = 0.0;
    int requestSent = 0;

    std::vector<Request::RequestType> types = {Request::LOGIN, Request::ADD_FRIEND, Request::LIST_WAITTING,
                                               Request::ACCEPT_FRIEND, Request::LIST_FRIEND, Request::DELETE_FRIEND,
                                               Request::LOGOUT};
    std::vector<std::vector<std::string>> args_vec = {{"player_" + std::to_string(player->playerId_), "pass"},
                                                      {"player_" + std::to_string((player->playerId_ + 1) % g_nPlayer)},
                                                      {},
                                                      {},
                                                      {},
                                                      {},
                                                      {}};
    std::vector<std::string> waitingList;
    std::vector<std::string> friendList;
    for (int i = 0; i < types.size();)
    {
        if (types[i] == Request::ACCEPT_FRIEND)
        {
            if (waitingList.empty())
            {
                ++i;
                continue;
            }
            for (int k = 0; k < waitingList.size(); ++k)
                args_vec[i].push_back(std::move(waitingList[k]));
        }
        else if (types[i] == Request::DELETE_FRIEND)
        {
            if (friendList.empty())
            {
                ++i;
                continue;
            }
            args_vec[i].clear();
            args_vec[i].push_back(friendList.back());
            friendList.pop_back();
        }

        prepareRequest(request, types[i], player->uid_, 0, args_vec[i]);
        begin = SteadyClock::now();
        if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
        { // fail because of IO
            logger_ptr->error("player_{0} fail at send/recv {1}", player->playerId_, requestTypeToStr[types[i]]);
            return 0;
        }
        if ((types[i] == Request::LOGIN) && response.status() < 0)
        {
            logger_ptr->error("player_{0} fail at {1}", player->playerId_, requestTypeToStr[types[i]]);
            handle_failure(player->fd_);
            return 0;
        }
        end = SteadyClock::now();
        delay =
            (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
        ++requestSent;
        if (types[i] == Request::LOGIN)
            player->uid_ = response.uid();
        else if (types[i] == Request::LIST_WAITTING)
        {
            for (int k = 0; k < response.args_size(); ++k)
                waitingList.emplace_back(std::move(response.args(k)));
        }
        else if (types[i] == Request::LIST_FRIEND)
        {
            for (int k = 0; k < response.args_size(); ++k)
                friendList.emplace_back(std::move(response.args(k)));
        }
        if (types[i] != Request::DELETE_FRIEND || friendList.empty())
        {
            ++i;
        }
    }
    player->requestSent_ = requestSent;
    player->successful_ = true;
    player->avgDelay_ = delay;
    close(player->fd_);
    g_active_player--;
    return 0;
}

static void *RoomCreateRoutine(void *arg)
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

    std::vector<Request::RequestType> types = {Request::LOGIN, Request::CREATE_ROOM, Request::LOGOUT};
    std::vector<std::vector<std::string>> args_vec = {
        {"player_" + std::to_string(player->playerId_), "pass"}, {}, {}};

    for (int i = 0; i < types.size(); ++i)
    {
        prepareRequest(request, types[i], player->uid_, 0, args_vec[i]);
        begin = SteadyClock::now();
        if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
        { // fail because of IO
            std::cout << "player_" << player->playerId_ << " fail at " << requestTypeToStr[types[i]] << std::endl;
            return 0;
        }
        if (types[i] == Request::LOGIN && response.status() < 0)
        {
            handle_failure(player->fd_);
            return 0;
        }
        end = SteadyClock::now();
        delay =
            (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
        ++requestSent;
        if (types[i] == Request::LOGIN)
            player->uid_ = response.uid();
    }
    player->requestSent_ = requestSent;
    player->successful_ = true;
    player->avgDelay_ = delay;
    close(player->fd_);
    g_active_player--;
    return 0;
}

// static void *GameTestRoutine(void *arg)
// {
//     co_enable_hook_sys();

//     char buffer[2048];

//     VPlayer *player = static_cast<VPlayer *>(arg);
//     Request request;
//     Response response;
//     std::vector<Request> requests;
//     std::vector<Response> responses;
//     TimePoint begin, end;
//     double delay = 0.0;
//     int requestSent = 0;
//     int nextStep = 0;
//     int noResponseCnt = 0;
//     std::vector<std::string> roomList;

// LOGIN:
//     prepareRequest(request, Request::LOGIN, 0, 0, {"player_" + std::to_string(player->playerId_), "pass"});
//     begin = SteadyClock::now();
//     if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer))
//         || response.status() < 0)
//     {
//         logger_ptr->error("player_{0} fail at Login.", player->playerId_);
//         goto ERROR;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
//     player->uid_ = response.uid();
// JOINROOM:
//     prepareRequest(request, Request::JOIN_ROOM, player->uid_, 0, {std::to_string(player->playerId_ / 4 + 1)});
//     begin = SteadyClock::now();
//     if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer))
//         || response.status() < 0)
//     {
//         logger_ptr->error("player_{0} fail at JOIN_ROOM.");
//         goto ERROR;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
// READY:
//     prepareRequest(request, Request::READY, player->uid_, 0, {});
//     begin = SteadyClock::now();
//     if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer))
//         || response.status() < 0)
//     {
//         logger_ptr->error("player_{0} fail at READY.");
//         goto ERROR;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
// BET:  
//     requests.clear(); responses.clear();
//     begin = SteadyClock::now();
//     if (!recvMsg(player->fd_, requests, responses, buffer, sizeof(buffer))
//         || requests.size() != 1 || requests[0].args_size() == 0 
//         || requests[0].args(0) != "start")
//     {
//         logger_ptr->error("player_{} fail to recv bet requets from server", player->playerId_);
//         goto ERROR;
//     }
//     prepareResponse(response, 1, player->uid_, requests[0].stamp(), {"Bet", "100"});
//     if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
//     {
//         logger_ptr->error("player_{} fail to send bet response to server", player->playerId_);
//         goto ERROR;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
//     // check if I am a dealer
//     if (requests[0].args_size() > 1)
//     {
//         logger_ptr->info("player_{0} is dealer", player->playerId_);
//         goto STEP9;
//     }
// STAND:
//     while (true)
//     {
//         requests.clear(); responses.clear();
//         begin = SteadyClock::now();
//         if (!recvMsg(player->fd_, requests, responses, buffer, sizeof(buffer))
//             || requests.empty())
//         {
//             logger_ptr->error("player_{} fail to recv hit/end requets from server", player->playerId_);
//             goto ERROR;
//         }
        
//         for (int i = 0; i < requests.size(); ++i)
//         {
//             if (requests[i].args(0) == "update")
//             {
//                 prepareResponse(response, 1, player->uid_, requests[i].stamp(), {"update"});
//             }
//             else if (requests[i].args(0) == "hit")
//             {
//                 prepareResponse(response, 1, player->uid_, requests[i].stamp(), {"Stand"});
//                 nextStep = 1;
//             }
//             else if (requests[i].args(0) == "end")
//             {
//                 prepareResponse(response, 1, player->uid_, requests[i].stamp(), {"end"});
//                 nextStep = 2;
//             }
//             if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
//             {
//                 logger_ptr->error("player_{0} fail to send {1} response to server", player->playerId_, requests[i].args(0));
//                 goto ERROR;
//             }
//             end = SteadyClock::now();
//             delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//             requestSent++;
//         }
//         if (nextStep == 1)
//         {
//             break;
//         }
//         else if (nextStep == 2)
//         {
//             goto LEAVEROOM;
//         }  
//     }
// END:  
//     while (true)
//     {
//         requests.clear(); responses.clear();
//         begin = SteadyClock::now();
//         if (!recvMsg(player->fd_, requests, responses, buffer, sizeof(buffer))
//             || requests.empty())
//         {
//             logger_ptr->error("player_{} fail to recv end requets from server", player->playerId_);
//             goto ERROR;
//         }
//         for (int i = 0; i < requests.size(); ++i)
//         {
//             if (requests[i].args(0) == "update")
//             {
//                 prepareResponse(response, 1, player->uid_, requests[i].stamp(), {"update"});
//             }
//             else if (requests[i].args(0) == "hit")
//             {
//                 prepareResponse(response, 1, player->uid_, requests[i].stamp(), {"Stand"});
//                 logger_ptr->warn("player_{} received an hit package again after he standed.", player->playerId_);
//             }
//             else if (requests[i].args(0) == "end")
//             {
//                 prepareResponse(response, 1, player->uid_, requests[i].stamp(), {"end"});
//                 nextStep = 1;
//             }
//             if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
//             {
//                 logger_ptr->error("player_{0} fail to send {1} response to server", player->playerId_, requests[i].args(0));
//                 goto ERROR;
//             }
//             end = SteadyClock::now();
//             delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//             requestSent++;
//         }
//         if (nextStep == 1)
//         {
//             break;
//         } 
//     }
// LEAVEROOM:  // recv stand stand request
//     prepareRequest(request, Request::LEAVE_ROOM, player->uid_, 0, {});
//     begin = SteadyClock::now();
//     if (!sendMsg(player->fd_, REQUEST, request, buffer, sizeof(buffer)))
//     {
//         logger_ptr->error("player_{} fail to send LEAVE ROOM request to server.", player->playerId_);
//         goto ERROR;
//     }
//     while (true)
//     {
//         requests.clear(); responses.clear();
//         begin = SteadyClock::now();
//         if (!recvMsg(player->fd_, requests, responses, buffer, sizeof(buffer)))
//         {
//             logger_ptr->error("player_{} fail to recv LEAVE_ROOM from server", player->playerId_);
//             goto ERROR;
//         }
//         if (responses.empty())
//         {
//             if (noResponseCnt++ > 3)
//             {
//                 logger_ptr->error("player_{} fail to recv LEAVE_ROOM from server", player->playerId_);
//                 goto ERROR;
//             }
//         }
//         for (int i = 0; i < requests.size(); ++i)
//         {
//             prepareResponse(response, 1, player->uid_, requests[i].stamp(), {requests[i].args(0)});
//             if (!sendMsg(player->fd_, RESPONSE, response, buffer, sizeof(buffer)))
//             {
//                 logger_ptr->error("player_{0} fail to send {1} response to server", player->playerId_, requests[i].args(0));
//                 goto ERROR;
//             }
//             end = SteadyClock::now();
//             delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//             requestSent++;
//         }
//         if (nextStep == 1)
//         {
//             break;
//         }
//     }
// ERROR:
    
    
//     logger_ptr->info("player_{0} try to logout", player->playerId_);
//     if (!sendAndRecvMsg(player->fd_, REQUEST, request, RESPONSE, response, buffer, sizeof(buffer)))
//     {
//         logger_ptr->error("player_{0} fail at sending logout.");
//         return 0;
//     }
//     end = SteadyClock::now();
//     delay = (delay * requestSent + std::chrono::duration_cast<MilliSeconds>(end - begin).count()) / (requestSent + 1);
//     requestSent++;
//     player->requestSent_ = requestSent;
//     player->successful_ = true;
//     player->avgDelay_ = delay;
//     close(player->fd_);
//     g_active_player--;
//     return 0;
// }

static void initSockAddr(struct sockaddr_in &addr, const char *ip, unsigned short port)
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
}

int main(int argc, char **argv)
{
    if (argc < 6)
    {
        std::cout << "usage: " << std::string(argv[0]) << " ip port playerno log_path routine." << std::endl;
        return 0;
    }

    set_logger_name("test_logger");
    set_log_path(std::string(argv[4]));
    logger_flush_on();
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
    g_active_player = nConnected;\
    std::string routine_name = std::string(argv[5]);
    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].fd_ > -1)
        {
            if (routine_name == "RoomCreateRoutine")
                co_create(&(players[i].co_), NULL, RoomCreateRoutine, &players[i]);
            // else if (routine_name == "GameTestRoutine")    
            //     co_create(&(players[i].co_), NULL, GameTestRoutine, &players[i]);
            else if (routine_name == "LobbyTestRoutine")
                co_create(&(players[i].co_), NULL, LobbyTestRoutine, &players[i]);
            else if (routine_name == "PlayerTestRoutine")
                co_create(&(players[i].co_), NULL, PlayerTestRoutine, &players[i]);
            else if (routine_name == "SocialTestRoutine")
                co_create(&(players[i].co_), NULL, SocialTestRoutine, &players[i]);
            else
                {std::cout << "unknown routine." <<std::endl; return 0;}
            co_resume(players[i].co_);
        }
    }

    co_eventloop(co_get_epoll_ct(), need_to_stop, &g_active_player);

    TimePoint end = SteadyClock::now();
    // compute the average delay
    int time_tot = std::chrono::duration_cast<MilliSeconds>(end - begin).count();
    double delay = 0.0;
    int cnt = 0;
    int finishCnt = 0;
    int requestSent = 0;
    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].requestSent_ > 0)
        {
            cnt++;
            requestSent += players[i].requestSent_;
            delay += players[i].avgDelay_;
        }
        if (players[i].successful_)
            finishCnt++;
    }
    if (cnt > 0)
    {
        std::cout << finishCnt << " players finish the test." << std::endl;
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