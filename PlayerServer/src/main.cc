#include <signal.h>

#include <fstream>

#include "PlayerClientDB-syn.h"
#include "PlayerServer-asyn.h"
#include "global.h"

std::shared_ptr<spdlog::logger> logger = nullptr;

std::string option;
std::string DBServer_ip;
std::string DBServer_port;
std::string listen_port;
std::string log_file;
std::string is_daemon;

void StartServer() {
    if (is_daemon == "true") {
        std::cout << "Server start in daemon mode" << std::endl;
        daemon(1, 0);
    } else {
        std::cout << "Server start in frontground mode" << std::endl;
    }

    std::ofstream ofs;
    ofs.open("/var/run/PlayerServer.pid", std::ofstream::out | std::ofstream::trunc);

    auto pid = getpid();
    ofs << pid << std::endl;
    ofs.close();

    logger = spdlog::basic_logger_mt("basic_logger", log_file.c_str());
    logger->flush_on(spdlog::level::trace);
    logger->set_level(spdlog::level::err);

    ua_blackjack::player_server::ServerImpl server;
    server.Run(DBServer_ip, DBServer_port, listen_port);
}

void StopServer() {
    std::ifstream ifs;
    ifs.open("/var/run/PlayerServer.pid");
    int pid;
    ifs >> pid;
    ifs.close();

    kill(pid, SIGINT);
}

void InvalidArgs() {
    std::cerr << "invalid args" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        InvalidArgs();
    }

    if (strcmp(argv[1], "start") == 0) {
        if (argc != 3) {
            InvalidArgs();
        }

        std::ifstream config;
        config.open(argv[2]);

        if (config.bad()) {
            std::cerr << "bad config file path" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "---------reading config---------" << std::endl;

        config >> option >> DBServer_ip;
        config >> option >> DBServer_port;
        config >> option >> listen_port;
        config >> option >> log_file;
        config >> option >> is_daemon;

        StartServer();

    } else if (strcmp(argv[1], "stop") == 0) {
        if (argc != 2) {
            InvalidArgs();
        }

        StopServer();
    } else {
        InvalidArgs();
    }

    return 0;
}
