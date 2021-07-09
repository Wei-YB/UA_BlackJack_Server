#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include "LobbyService.h"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <unistd.h>
#include <getopt.h>
#include <fstream>     
#include <sstream>
#include <iostream>

using ua_blackjack::LobbyServer;

static void ReadConfig(const char *config_path, 
                       std::unordered_map<std::string, std::string> &config)
{
    std::ifstream fin(config_path, std::ifstream::in);
    if (!fin.good())
    {
        std::cerr << "In ReadConfig: fail to read config file." << std::endl;
        exit(EXIT_FAILURE);
    }
    while (fin.good())
    {
        std::string line, key, value;
        getline(fin, line);
        std::istringstream iss(line);
        iss >> key >> value;
        config.emplace(key, value);
    }
}

int main (int argc, char **argv) {
    
    std::unordered_map<std::string, std::string> lobby_config;
    if(argc<2){
        std::cout<<"usage: LobbyServer </path/to/lobby.config>"<<std::endl;
        exit(0);
    }
    ReadConfig(argv[1], lobby_config);
    auto logger = spdlog::basic_logger_mt<spdlog::async_factory>(lobby_config["logger_name"], lobby_config["log_path"]);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);
    LobbyServer server(lobby_config["gameAddress"], lobby_config["databaseAddress"]);

    server.Run(lobby_config["myAddress"]);
}