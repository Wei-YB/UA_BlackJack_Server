#include <iostream>
#include <queue>
#include "GameProcess.h"

int main(int agrc, char *argv[])
{

    // auto player1 = std::make_shared<Player>(50052);
    // auto player2 = std::make_shared<Player>(50053);
    // player1->client = std::make_shared<ClientForTestUser>(grpc::CreateChannel(
    //                                                "localhost:50052", grpc::InsecureChannelCredentials()),
    //                                            50052);
    // player2->client = std::make_shared<ClientForTestUser>(grpc::CreateChannel(
    //                                                "localhost:50053", grpc::InsecureChannelCredentials()),
    //                                            50053);

    // std::thread thread1_ = std::thread(&ClientForTestUser::AsyncCompleteRpc, player1->client.get()); //每个用户都用一个线程去监控发送给他的rpc命令有没有相应
    // std::thread thread2_ = std::thread(&ClientForTestUser::AsyncCompleteRpc, player2->client.get()); //每个用户都用一个线程去监控发送给他的rpc命令有没有相应

    // thread1_.detach();
    // thread2_.detach();
    // player1->client->askBettingMoney(50052);
    // player2->client->askBettingMoney(50053);
    // while (true) //block here
    // {

    //     sleep(1);
    // }

    // while (true)
    // {
    //     std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()); //获取当前时间点
    //     std::time_t timestamp = tp.time_since_epoch().count();                                                                                                                        //计算距离1970-1-1,00:00的时间长度
    //     std::stringstream ss;
    //     ss << timestamp;
    //     std::string tmpStr = ss.str();
    //     std::cout << tmpStr << std::endl;
    // }
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

/*************************************托管代码***************************************************/
//         if (env->operateId == OPERATE_QUIT) //有玩家要退出游戏
//         {
//             if (auto playerPtr = playerHashMap[((QuitArgument *)env->arg)->uid].lock())
//             {
// #ifdef PRINT_LOG
//                 std::cout << ((QuitArgument *)env->arg)->uid << " quit" << std::endl;
// #endif
//                 playerPtr->quit();             //托管
//                 playerPtr->isStand = true;     //玩家停牌
//             }
//         }
/*************************************托管代码***************************************************/