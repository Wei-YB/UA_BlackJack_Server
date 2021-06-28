#include <iostream>
#include <queue>
#include "GameProcess.h"
#include "AskForDatabaseRequest.h"
#include "AskForLobby.h"
#include "AskForUserRequest.h"
#include "spdlog/spdlog.h"
#include <sstream>
int main(int agrc, char *argv[])
{

    std::thread thread_ = std::thread(&ClientForTestUser::AsyncCompleteRpc, &ClientForTestUser::getInstance());
    thread_.detach();

    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    ServerImpl server;
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
    return 0;
}
