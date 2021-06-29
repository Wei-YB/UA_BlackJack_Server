#include <iostream>
#include <queue>
#include "GameProcess.h"
#include "ClientForDatebase.h"
#include "ClientForLobby.h"
#include "ClientForTestUser.h"
#include "spdlog/spdlog.h"
#include <sstream>
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
int main(int agrc, char *argv[])
{

    std::thread thread_ = std::thread(&ua_blackjack::Game::ClientForTestUser::AsyncCompleteRpc, &ua_blackjack::Game::ClientForTestUser::getInstance());
    std::thread thread2_ = std::thread(&ua_blackjack::Game::ClientForDatebase::AsyncCompleteRpc, &ua_blackjack::Game::ClientForDatebase::getInstance());

    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.log");
    spdlog::set_default_logger(async_file);

    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    ua_blackjack::Game::ServerImpl server;
    server.Run();

    spdlog::info("grpc async begin...");

    //创建等待RPC的协程
    co_create(&receiveSignalFromRPC, NULL, waitingSignalFromOtherModule, &server);
    co_resume(receiveSignalFromRPC);

    //回收协程的协程
    co_create(&recoverystCo, NULL, recoveryUnusedCo, NULL);
    co_resume(recoverystCo);

    //开启协程
    co_eventloop(co_get_epoll_ct(), NULL, NULL);

    thread_.join();
    thread2_.join();
    return 0;
}
