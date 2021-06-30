#ifndef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include "LobbyService.h"
#include <unistd.h>
#include <getopt.h>

using ua_blackjack::LobbyServer;

int main (int argc, char **argv) {

    int UserOption;
    std::string ec;
    std::string b_opt_arg;
    std::string DBServiceIP   = "9.134.69.87:50051";
    std::string GameServiceIP = "9.134.76.179:50051";

    while((UserOption = getopt(argc, argv, "g:d:")) != -1)
    {
        switch(UserOption)
        {
            case 'g':
                GameServiceIP = optarg;
                printf("GameserviceIP has been set as %s\n", optarg);
                break;
            case 'd':
                DBServiceIP = optarg;
                printf("DBserviceIP has been set as %s\n", optarg);
                break;
            case '?':
                ec = (char)optopt;
                printf("invalid option args \' %c \' \n", ec);
                printf("usage: LobbyServer -g <game service IP:port> -d <database IP:port>\n");
                break;
            case ':':
                printf("missing option args！\n");
                printf("usage: LobbyServer -g <game service IP:port> -d <database IP:port>\n");
                break;
        }
    }

    spdlog::set_level(spdlog::level::trace);
    LobbyServer server(GameServiceIP, DBServiceIP);
    server.Run();
}