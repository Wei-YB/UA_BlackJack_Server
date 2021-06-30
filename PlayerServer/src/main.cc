#include <signal.h>

#include <fstream>

#include "PlayerClientDB-syn.h"
#include "PlayerServer-asyn.h"
#include "global.h"

std::shared_ptr<spdlog::logger> logger = nullptr;

void StartServer() {
    daemon(1, 0);

    std::ofstream ofs;
    ofs.open("/var/run/PlayerServer.pid", std::ofstream::out | std::ofstream::trunc);

    auto pid = getpid();
    ofs << pid << std::endl;
    ofs.close();

    logger = spdlog::basic_logger_mt("basic_logger", "PlayerLog.log");
    logger->flush_on(spdlog::level::trace);

    ua_blackjack::player_server::ServerImpl server;
    server.Run();
}

void StopServer() {
    std::ifstream ifs;
    ifs.open("/var/run/PlayerServer.pid");
    int pid;
    ifs >> pid;
    ifs.close();

    kill(pid, SIGINT);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        StartServer();
    } else if (argc == 2 && strcmp(argv[1], "stop") == 0) {
        StopServer();
    }

    return 0;
}
