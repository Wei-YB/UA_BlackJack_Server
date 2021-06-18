#include <iostream>
#include <queue>
#include "Poker.h"
#include "ShuffledPokers.h"
#include "Player.h"
#include "Room.h"
#include "co_routine.h"

struct stEnv_t
{
    typedef std::shared_ptr<stEnv_t> ptr;
    stCoRoutine_t *coRoutine;    //协程句柄
    BlackJackRoomID roomID;      //创建的房间号
    UidList uids;                //所有玩家的uid
    stCoCond_t *cond;            //信号量
    OperateID operateId;         //操作码
    void *arg;                   //操作数
    OperateID expectedOperateId; //期望的操作码
    stEnv_t(BlackJackRoomID _roomID, UidList &_uids) : roomID(_roomID), uids(_uids){};
};
std::queue<BlackJackRoomID> unUsedstEnvRoomID;                      //结束了的游戏ID
std::unordered_map<BlackJackRoomID, stEnv_t::ptr> roomEnvirHashMap; //roomid和句柄的hash映射
std::unordered_map<BlackJackRoomID, bool> roomEnvirExistHashMap;    //roomid和句柄是否存在的hash映射
int createstEnv_t(BlackJackRoomID roomID, UidList &uids);
void *createOneGame(void *arg);
void *waitingSignalFromOtherModule(void *arg);
void *recoverystEnv_t(void *arg);                        //回收协程的协程
int createstEnv_t(BlackJackRoomID roomID, UidList &uids) //创建协程
{
    if (roomEnvirExistHashMap.count(roomID) > 0 && roomEnvirExistHashMap[roomID] == true) //原先已经有房间存在了
    {
#ifdef PRINT_LOG
        std::cout << "room has already existed" << std::endl;
#endif
        return -1; //无法再创建同一个房间号的ID
    }
    roomEnvirExistHashMap[roomID] = true; //宣告房间存在
    auto ptr = std::make_shared<stEnv_t>(roomID, uids);
    roomEnvirHashMap[roomID] = ptr; //将房间加入hash表
    ptr->cond = co_cond_alloc();    //创建信号量
    //开启创建房间协程
    co_create(&(ptr->coRoutine), NULL, createOneGame, NULL);
    co_resume(ptr->coRoutine);
    return 0;
}
void *createOneGame(void *arg) //开启一局游戏
{
    co_enable_hook_sys();
    stEnv_t *env = (stEnv_t *)arg;

#ifdef PRINT_LOG
    std::cout << "GAME BEGIN" << std::endl;
#endif
    auto room = malloOneRoom(env->roomID, env->uids); //创建一个房间
    int conRet = 0;
    //选择筹码
    for (auto &player : room->playerList)
    {
        if (player->isDealer == true) //庄家无需设置筹码
            continue;
        if (player->isQuit == true) //玩家已退出游戏,不需要设置筹码
            continue;

        /*void askPlayerOneToSetBettingMoney*/ //  这个函数中应设置期望的operateID
        conRet = 0;
        conRet = co_cond_timedwait(env->cond, 30000); //30秒内应收到信号
        if (conRet != 0)                              //超时未收到信号，认为玩家已退出游戏
        {
            player->quit();             //托管
            player->isStand = true;     //玩家停牌
            player->finalResult = DRAW; //筹码阶段退出应判平
#ifdef PRINT_LOG
            std::cout << ((QuitArgument *)env->arg)->uid << " quit" << std::endl;
#endif
            continue;
        }
        //接收到玩家的筹码信号
        if ((env->operateId == OPERATE_BETMONEY) && (((BetMoneyArgument *)env->arg)->uid == player->uid))
        {
            auto money = ((BetMoneyArgument *)env->arg)->money;
            player->bettingMoney = money;
#ifdef PRINT_LOG
            std::cout << ((QuitArgument *)env->arg)->uid << " setBetMonney " << money << std::endl;
#endif
            continue;
        }
    }

    //发初始牌
    for (int i = 0; i < 2; i++)
    {
        for (auto &player : room->playerList)
        {
            player->hitPoker();
        }
    }
    room->playerList.front()->pokerList.front()->setHide(); //将第一个玩家的第一张牌设为不可见(庄家的第一张牌为暗牌)

    //玩家可操作游戏
    while (true)
    {
        bool hasPlayerContinuePlay = false; //还有玩家可以选择是否要出牌
        for (auto &player : room->playerList)
        {
            if (player->isDealer == true) //庄家没有操作权限
                continue;
            if (player->isStand == true) //玩家已停牌，无需选择
            {
                continue;
            }
            hasPlayerContinuePlay = true; //依然还有玩家可以选择是否要出牌
            if (player->isQuit == true)   //玩家还没爆或blackjack，但进入托管模式
            {
                player->hitPoker();
                continue;
            }

            /*void askPlayerOneToHitOrStand*/ //  这个函数中应设置期望的operateID
            conRet = 0;
            conRet = co_cond_timedwait(env->cond, 30000); //30秒内应收到信号
            if (conRet != 0)                              //超时未收到信号，认为玩家已退出游戏
            {
                player->quit(); //托管
                player->hitPoker();
#ifdef PRINT_LOG
                std::cout << ((QuitArgument *)env->arg)->uid << " quit" << std::endl;
#endif
                continue;
            }
            //接收到玩家的打牌/停牌信号
            if ((env->operateId == OPERATE_HIT || env->operateId == OPERATE_STAND) && (((BetMoneyArgument *)env->arg)->uid == player->uid))
            {
                if (env->operateId == OPERATE_HIT)
                {
                    player->hitPoker();
#ifdef PRINT_LOG
                    std::cout << ((QuitArgument *)env->arg)->uid << " Hit " << std::endl;
#endif
                }
                else if (env->operateId == OPERATE_STAND)
                {
                    player->standPoker();
#ifdef PRINT_LOG
                    std::cout << ((QuitArgument *)env->arg)->uid << " Hit " << std::endl;
#endif
                }
                continue;
            }
        }

        if (hasPlayerContinuePlay == false) //所有玩家均已停牌
            break;
    }

    unUsedstEnvRoomID.push((room->getRoomId())); //协程结束，return后栈中资源释放，接着回收协程的协程开始工作
    return NULL;
}
uint64_t cnt = 0;
void *waitingSignalFromOtherModule(void *arg)
{
    co_enable_hook_sys();

    while (true)
    {
        std::cout << cnt++ << "Waiting.." << std::endl;
        co_yield_ct();
        poll(NULL, 0, 1); //必须要有挂起函数
    }
}
void *recoveryUnusedCo(void *arg) //回收协程的协程
{
    co_enable_hook_sys();

    while (true)
    {
        if (unUsedstEnvRoomID.empty()) //没有需要释放的资源
        {
            std::cout << cnt++ << "Free.." << std::endl;
            poll(NULL, 0, 1); //必须要有挂起函数
        }
        else
        {
            BlackJackRoomID roomid = unUsedstEnvRoomID.front();
            unUsedstEnvRoomID.pop();
            co_release(roomEnvirHashMap[roomid]->coRoutine); //释放协程资源
            roomEnvirHashMap[roomid].reset();                //释放指针
            roomEnvirExistHashMap[roomid] = false;
        }
    }
}
int main(int agrc, char *argv[])
{
    //创建等待RPC的协程
    stCoRoutine_t *receiveSignalFromRPC;
    co_create(&receiveSignalFromRPC, NULL, waitingSignalFromOtherModule, NULL);
    co_resume(receiveSignalFromRPC);

    //回收协程的协程
    stCoRoutine_t *recoverystCo;
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
//                 playerPtr->finalResult = DRAW; //筹码阶段退出应判平
//             }
//         }
/*************************************托管代码***************************************************/