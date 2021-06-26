#include <iostream>
#include <queue>
#include "GameProcess.h"
#include "AskForDatabaseRequest.h"
#include "AskForLobby.h"
#include "AskForUserRequest.h"
#include "spdlog/spdlog.h"
int main(int agrc, char *argv[])
{
    // std::cout<<"Welcome to spdlog!");
    ServerImpl server;
    server.Run();

#ifdef PRINT_LOG
    std::cout << "grpc async begin..." << std::endl;
#endif

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
