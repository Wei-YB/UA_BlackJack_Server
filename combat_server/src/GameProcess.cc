#include "GameProcess.h"
std::queue<BlackJackRoomID> unUsedstEnvRoomID;                      //结束了的游戏ID
std::unordered_map<BlackJackRoomID, stEnv_t::ptr> roomEnvirHashMap; //roomid和句柄的hash映射
std::unordered_map<BlackJackRoomID, bool> roomEnvirExistHashMap;    //roomid和句柄是否存在的hash映射
stCoRoutine_t *receiveSignalFromRPC;
stCoRoutine_t *recoverystCo;
int createstEnv_t(BlackJackRoomID roomID, UidList &uids) //创建协程
{
#ifdef PRINT_LOG
    std::cout << "roomid = " << roomID << std::endl;
    std::cout << "uids = " << std::endl;
    for (auto uid : uids)
    {
        std::cout << uid << " ";
    }
    std::cout << std::endl;
#endif
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
    ptr->cond = createCondition(0); //创建信号量
    ptr->arg = malloc(sizeof(BetMoneyArgument));
    //开启创建房间协程
    co_create(&(ptr->coRoutine), NULL, createOneGame, ptr.get());
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
    // std::cout << "NO ERROR" << std::endl;
    auto room = malloOneRoom(env->roomID, env->uids); //创建一个房间
    int conRet = 0;
    //选择筹码
    for (auto &player : room->playerList)
    {
        if (player->isDealer == true) //庄家无需设置筹码
            continue;
        if (player->isQuit == true) //玩家已退出游戏,不需要设置筹码
            continue;

        player->client->askBettingMoney(player->uid);
        conRet = 0;
        conRet = myConditionWait(env->cond, 30000); //30秒内应收到信号
        if (conRet == 0)                            //超时未收到信号，认为玩家已退出游戏
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

    //发初始牌,每人2张
    for (int i = 0; i < 2; i++)
    {
        for (auto &player : room->playerList)
        {
            player->hitPoker();
        }
    }
    room->playerList.front()->pokerList.front()->setHide(); //将第一个玩家的第一张牌设为不可见(庄家的第一张牌为暗牌)

    for (auto &player : room->playerList) //通知所有玩家把所有人的牌更新一下
    {
        UpdateAll(room->playerList, player->uid);
    }

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
                UpdateAll(room->playerList, player->uid); //抽牌，更新下
                continue;
            }

            player->client->askHitOrStand(player->uid);
            conRet = 0;
            conRet = myConditionWait(env->cond, 30000); //30秒内应收到信号
            if (conRet == 0)                            //超时未收到信号，认为玩家已退出游戏
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
                    UpdateAll(room->playerList, player->uid); //抽牌，更新下
#ifdef PRINT_LOG
                    std::cout << ((QuitArgument *)env->arg)->uid << " Hit " << std::endl;
#endif
                }
                else if (env->operateId == OPERATE_STAND)
                {
                    player->standPoker();
#ifdef PRINT_LOG
                    std::cout << ((QuitArgument *)env->arg)->uid << " Stand " << std::endl;
#endif
                }
                continue;
            }
        }

        if (hasPlayerContinuePlay == false) //所有玩家均已停牌
            break;
    }
    room->deleteRoom();
    room->playerList.front()->pokerList.front()->cancelHide();  //庄家最后名牌
    UpdateAll(room->playerList, room->playerList.front()->uid); //更新庄家的牌
    for (auto &player : room->playerList)
    {
        player->client->askEnd(player->uid, player->finalResult);
    }
    unUsedstEnvRoomID.push((room->getRoomId())); //协程结束，return后栈中资源释放，接着回收协程的协程开始工作
    return NULL;
}
uint64_t cnt = 0;
void *waitingSignalFromOtherModule(void *arg)
{
    co_enable_hook_sys();
    ServerImpl *server = (ServerImpl *)arg;

    // Spawn a new CallData instance to serve new clients.
    new ServerImpl::CallData(&server->service_, server->cq_.get());
    void *tag; // uniquely identifies a request.
    bool ok;
    gpr_timespec deadline;
    deadline.clock_type = GPR_TIMESPAN;
    deadline.tv_sec = 0;
    deadline.tv_nsec = 100;
    while (true)
    {
        while (server->cq_->AsyncNext<gpr_timespec>(&tag, &ok, deadline) != grpc::CompletionQueue::NextStatus::GOT_EVENT)
        {
            poll(NULL, 0, 1); //必须要有挂起函数
        }

        GPR_ASSERT(ok);
        static_cast<ServerImpl::CallData *>(tag)->Proceed();
    }
}
void *recoveryUnusedCo(void *arg) //回收协程的协程
{
    co_enable_hook_sys();

    while (true)
    {
        if (unUsedstEnvRoomID.empty()) //没有需要释放的资源
        {
            // #ifdef PRINT_LOG
            //             std::cout << cnt++ << "Free.." << std::endl;
            // #endif
            poll(NULL, 0, 1); //必须要有挂起函数
        }
        else
        {
            BlackJackRoomID roomid = unUsedstEnvRoomID.front();
            unUsedstEnvRoomID.pop();
            free(roomEnvirHashMap[roomid]->arg);
            co_release(roomEnvirHashMap[roomid]->coRoutine); //释放协程资源
            roomEnvirHashMap[roomid].reset();                //释放指针
            roomEnvirExistHashMap[roomid] = false;
#ifdef PRINT_LOG
            std::cout << "delete one room" << std::endl;
#endif
        }
    }
}
void UpdateAll(std::list<Player::ptr> &list, BlackJackUID uid)
{
    for (auto player : list)
    {
        player->client->askUpdate(uid);
    }
}