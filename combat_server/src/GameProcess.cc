#include "GameProcess.h"
#include "spdlog/spdlog.h"
#include <sstream>
std::queue<BlackJackRoomID> unUsedstEnvRoomID;                      //结束了的游戏ID
std::unordered_map<BlackJackRoomID, stEnv_t::ptr> roomEnvirHashMap; //roomid和句柄的hash映射
std::unordered_map<BlackJackRoomID, bool> roomEnvirExistHashMap;    //roomid和句柄是否存在的hash映射
stCoRoutine_t *receiveSignalFromRPC;
stCoRoutine_t *recoverystCo;
int conditionForWaitingRpc;                              //接受rpc的信号量
int conditionForClearRoom;                               //清楚房间的信号量
int createstEnv_t(BlackJackRoomID roomID, UidList &uids) //创建协程
{
    std::stringstream ss;
    ss << " roomid = " << roomID;
    ss << " uids = ";
    for (auto uid : uids)
    {
        ss << uid << " ";
    }
    spdlog::info(ss.str());

    if (roomEnvirExistHashMap.count(roomID) > 0 && roomEnvirExistHashMap[roomID] == true) //原先已经有房间存在了
    {
        spdlog::error("room has already existed");
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

    spdlog::info("GAME BEGIN");
    auto room = malloOneRoom(env->roomID, env->uids); //创建一个房间
    int conRet = 0;
    //选择筹码
    for (auto &player : room->playerList)
    {
        if (player->isDealer == true) //庄家无需设置筹码
            continue;
        if (player->isQuit == true) //玩家已退出游戏,不需要设置筹码
            continue;

        ClientForTestUser::getInstance().askBettingMoney(player->uid);
        myConditionSignal(conditionForWaitingRpc); //有机会唤醒
        conRet = 0;
        conRet = myConditionWait(env->cond, 30000); //30秒内应收到信号
        if (conRet == 0)                            //超时未收到信号，认为玩家已退出游戏
        {
            player->quit();             //托管
            player->isStand = true;     //玩家停牌
            player->finalResult = DRAW; //筹码阶段退出应判输0元
            spdlog::warn("uid {0:d} timeout and quit game", player->uid);
            continue;
        }
        //接收到玩家的筹码信号
        if ((env->operateId == OPERATE_BETMONEY) && (((BetMoneyArgument *)env->arg)->uid == player->uid))
        {
            auto money = ((BetMoneyArgument *)env->arg)->money;
            player->bettingMoney = money;
            spdlog::info("uid {0:d} set betting money", player->uid);

            continue;
        }
    }

    //发初始牌,每人2张
    for (int i = 0; i < 2; i++)
    {
        for (auto &player : room->playerList)
        {
            if (player->isStand == false) //用户没下注不准抽牌
                player->hitPoker();
            room->playerList.front()->pokerList.front()->setHide(); //将第一个玩家的第一张牌设为不可见(庄家的第一张牌为暗牌)

            UpdateAll(room->playerList, player->uid);
        }
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

            ClientForTestUser::getInstance().askHitOrStand(player->uid);
            myConditionSignal(conditionForWaitingRpc); //有机会唤醒
            conRet = 0;
            conRet = myConditionWait(env->cond, 30000); //30秒内应收到信号
            if (conRet == 0)                            //超时未收到信号，认为玩家已退出游戏
            {
                player->quit(); //托管
                player->hitPoker();
                UpdateAll(room->playerList, player->uid);
                spdlog::warn("uid {0:d} timeout and quit game", player->uid);
                continue;
            }
            //接收到玩家的打牌/停牌信号
            if ((env->operateId == OPERATE_HIT || env->operateId == OPERATE_STAND) && (((BetMoneyArgument *)env->arg)->uid == player->uid))
            {
                if (env->operateId == OPERATE_HIT)
                {
                    player->hitPoker();
                    UpdateAll(room->playerList, player->uid); //抽牌，更新下
                    spdlog::info("uid {0:d} hit", player->uid);
                }
                else if (env->operateId == OPERATE_STAND)
                {
                    player->standPoker();
                    spdlog::info("uid {0:d} stand", player->uid);
                }
                continue;
            }
        }

        if (hasPlayerContinuePlay == false) //所有玩家均已停牌
            break;
    }
    room->playerList.front()->pokerList.front()->cancelHide();        //庄家最后名牌
    UpdateAll(room->playerList, room->playerList.front()->uid, true); //更新庄家的牌
    room->deleteRoom();                                               //检查输赢
    for (auto &player : room->playerList)
    {
        ClientForTestUser::getInstance().askEnd(player->uid, player->finalResult); //send end request
    }
    unUsedstEnvRoomID.push((room->getRoomId())); //协程结束，return后栈中资源释放，接着回收协程的协程开始工作
    myConditionSignal(conditionForClearRoom);    //唤醒清空协程
    return NULL;
}

void *waitingSignalFromOtherModule(void *arg)
{
    co_enable_hook_sys();
    conditionForWaitingRpc = createCondition(0);
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
            myConditionWait(conditionForWaitingRpc, 1); //最长1ms检查一次
            //poll(NULL, 0, 1); //必须要有挂起函数
        }

        GPR_ASSERT(ok);
        static_cast<ServerImpl::CallData *>(tag)->Proceed();
    }
}
void *recoveryUnusedCo(void *arg) //回收协程的协程
{
    co_enable_hook_sys();
    conditionForClearRoom = createCondition(0);
    while (true)
    {
        if (unUsedstEnvRoomID.empty()) //没有需要释放的资源
        {
            myConditionWait(conditionForClearRoom, -1);
        }
        else
        {
            BlackJackRoomID roomid = unUsedstEnvRoomID.front();
            spdlog::info("start delete room {0:d}", roomid);
            unUsedstEnvRoomID.pop();
            free(roomEnvirHashMap[roomid]->arg);
            co_release(roomEnvirHashMap[roomid]->coRoutine); //释放协程资源
            roomEnvirHashMap[roomid].reset();                //释放智能 指针
            roomEnvirExistHashMap[roomid] = false;
            spdlog::info("complete delete room {0:d}", roomid);
        }
    }
}
void UpdateAll(std::list<Player::ptr> &list, BlackJackUID uid)
{
    for (auto player : list)
    {
        ClientForTestUser::getInstance().askUpdate(uid, player->uid);
    }
}
void UpdateAll(std::list<Player::ptr> &list, BlackJackUID uid, bool showDealerHide)
{
    for (auto player : list)
    {
        ClientForTestUser::getInstance().askUpdate(uid, player->uid, showDealerHide);
    }
}