#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
// #include "ClientProxyProtocol.h"
#include "co_routine.h"
#include "UA_BlackJack.pb.h"
#include "common.h"

using ua_blackjack::Request;
using ua_blackjack::Response;

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
    bool isLogin_ = false;
    bool isSignUp_ = false;
};

#define PACKAGE_HDR_LEN 8
#define REQUEST 1
#define RESPONSE 2

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

// signup
static void *SignUpRoutine(void *arg)
{
    co_enable_hook_sys();

    Player *player = static_cast<Player*>(arg);
    Request request;
    request.set_requesttype(Request::LOGIN);
    request.add_args("player_"+std::to_string(player->playerId_));
    request.add_args("pass");
    // prepare request package
    std::string pkg(PACKAGE_HDR_LEN + request.ByteSizeLong(), '\0');
    int32_t type = htonl(REQUEST);
    int32_t msgLen = htonl(request.ByteSizeLong());
    memcpy((void*)pkg.c_str(), (void*)&type, sizeof(int32_t));
    memcpy((void*)(pkg.c_str() + 4), (void*)&msgLen, sizeof(int32_t));
    request.SerializeToArray((char*)pkg.c_str() + 8, request.ByteSizeLong());
    // send request package
    int idx = 0;
    while (idx < pkg.size())
    {
        int ret = write(player->fd_, pkg.c_str() + idx, pkg.size() - idx);
        if (ret < 0)
        {   // fail to signup
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
            {
                continue;
            }
            player->isSignUp_ = false;
            close(player->fd_);
            return 0;
        }
        idx += ret;
    }
    // wait for response
    idx = 0;
    char buf[2048];
    struct pollfd pf = {0};
    pf.fd = player->fd_;
    pf.events = (POLLIN | POLLERR | POLLHUP);
    co_poll(co_get_epoll_ct(), &pf, 1, 1000);
    int ret = read(player->fd_, buf, sizeof(buf));
    if (ret < 8)
    {   // fail to signup, I don't care whether it is EAGIN case
        player->isSignUp_ = false;
        close(player->fd_); 
        return 0;     
    }
    // parse package
    const char *rawResponse = unpack(buf, ret);
    if (!rawResponse)
    {
        player->isSignUp_ = false;
        close(player->fd_); 
        return 0; 
    }
    Response response;
    response.ParseFromArray(rawResponse, ret - PACKAGE_HDR_LEN);
    if (response.status() < 0)
    {
        player->isSignUp_ = false;
        close(player->fd_); 
        return 0; 
    }
    player->uid_ = response.uid();
    player->isSignUp_ = true;
    return 0;
}

static void *LobbyTestRoutine(void *arg)
{
    co_enable_hook_sys();

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

    int nConnected = 0;
    std::vector<Player> players(atoi(argv[3]));
    for (int i = 0; i < players.size(); ++i)
    {
        players[i].playerId_ = i;
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

    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].fd_ > -1)
        {
            co_create(&(players[i].co_), NULL, SignUpRoutine, &players[i]);
            co_resume(players[i].co_);
        }
    }

    co_eventloop(co_get_epoll_ct(), 0, 0);

    // count the successfully signuped player
    int nSignUped = 0;
    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].isSignUp_)
            nSignUped++;
    }

    std::cout << nSignUped << " players sign up successfully." << std::endl;
    exit(0);
}