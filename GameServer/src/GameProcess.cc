#include "GameProcess.h"
#include "spdlog/spdlog.h"
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include "ControlTcpServer.h"
std::queue<BlackJackRoomID> unUsedstEnvRoomID;                      //结束了的游戏ID
std::unordered_map<BlackJackRoomID, stEnv_t::ptr> roomEnvirHashMap; //roomid和句柄的hash映射
stCoRoutine_t *receiveSignalFromRPC;
stCoRoutine_t *recoverystCo;
int conditionForWaitingRpc; //接受rpc的信号量
int conditionForClearRoom;  //清楚房间的信号量
#define TIMEOUT_FOR_USER -1
uint64_t sumOfRoom = 0;
int createstEnv_t(BlackJackRoomID roomID, UidList &uids) //创建协程
{
    sumOfRoom++;
    std::stringstream ss;
    ss << " roomid = " << roomID;
    ss << " uids = ";
    for (auto uid : uids)
    {
        ss << uid << " ";
    }
    spdlog::info(ss.str());

    if (roomEnvirHashMap.count(roomID) > 0) //原先已经有房间存在了
    {
        spdlog::error("room has already existed");
        return -1; //无法再创建同一个房间号的ID
    }

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

    //

    spdlog::info("GAME BEGIN");

    int playerSize = env->uids.size();

    ua_blackjack::Game::Room::ptr room;
    if (roomPool[playerSize - 2].size() == 0)
    {
        room = malloOneRoom(env->roomID, env->uids); //创建一个房间
        roomLists.push_back(room);
    }
    else //池中房间不够，重创一个
    {
        room = roomPool[playerSize - 2].front();
        room->rid = env->roomID; //room id

        auto iterUids = env->uids.begin();
        auto iterPlayers = room->playerList.begin();
        while (iterUids != env->uids.end())
        {
            (*iterPlayers)->uid = *iterUids;
            (*iterPlayers)->setRoom(room->rid);
            playerHashMap[*iterUids] = (*iterPlayers);
            if (isProgramRelase == true)
            {
                ua_blackjack::Game::ClientForDatebase::getInstance().askPlayerNickName(*iterUids); //设置nickname
            }
            iterUids++;
            iterPlayers++;
        }
        room->playerList.front()->isDealer = true;

        roomHashMap[env->roomID] = room;
        roomPool[playerSize - 2].pop();
    }
    int conRet = 0;
    //选择筹码
    for (auto &player : room->playerList)
    {
        ua_blackjack::Game::ClientForTestUser::getInstance().askBettingMoney(player->uid);
    }
    conRet = 0;
    conRet = myConditionWait(env->cond, TIMEOUT_FOR_USER); //30秒内应收到信号
    if (conRet == 0)
    {
        for (auto &player : room->playerList)
        {
            if (player->isDealer == true)
                continue;                                        //庄家无需设置筹码
            if (conRet == 0 && player->isFinishBetting == false) //超时未收到信号，认为玩家已退出游戏
            {
                player->quit(); //托管
                spdlog::warn("uid {0:d} timeout and quit game", player->uid);
                continue;
            }
        }
    }
    room->isGameBegin = true;
    //发初始牌,每人2张
    for (int i = 0; i < 2; i++)
    {
        for (auto &player : room->playerList)
        {
            if (player->isStand == false) //用户没下注不准抽牌
                player->hitPoker();
        }
    }
    room->playerList.front()->pokerList.front()->setHide(); //将第一个玩家的第一张牌设为不可见(庄家的第一张牌为暗牌)
    for (auto &player : room->playerList)
    {
        if (player->nickName.size() == 0) //用户没下注不准抽牌
            player->nickName = "null";
    }
    UpdateAll(room->playerList, 0, false);
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

            ua_blackjack::Game::ClientForTestUser::getInstance().askHitOrStand(player->uid);
            myConditionSignal(conditionForWaitingRpc); //有机会唤醒
            conRet = 0;
            player->isWaitingReply = true;
            conRet = myConditionWait(env->cond, TIMEOUT_FOR_USER); //30秒内应收到信号
            player->isWaitingReply = false;
            if (conRet == 0) //超时未收到信号，认为玩家已退出游戏
            {
                player->quit(); //托管
                player->hitPoker();
                UpdateAll(room->playerList, player->uid);
                spdlog::warn("uid {0:d} timeout and quit game", player->uid);
                continue;
            }
            //接收到玩家的打牌/停牌信号
            if ((env->operateId == OperateID::OPERATE_HIT || env->operateId == OperateID::OPERATE_STAND || env->operateId == OperateID::OPERATE_SURRAND) && (((BetMoneyArgument *)env->arg)->uid == player->uid))
            {
                if (env->operateId == OperateID::OPERATE_HIT)
                {
                    player->hitPoker();
                    UpdateAll(room->playerList, player->uid); //抽牌，更新下
                                                              //

                    spdlog::info("uid {0:d} hit", player->uid);
                }
                else if (env->operateId == OperateID::OPERATE_STAND)
                {
                    player->standPoker();
                    //

                    spdlog::info("uid {0:d} stand", player->uid);
                }
                else if (env->operateId == OperateID::OPERATE_SURRAND)
                {
                    spdlog::info("{0:d} surrond by itself", player->uid);
                    if (player->isStand == false) //还未停牌
                        player->bettingMoney /= 2;
                    player->isStand = true;
                    player->finalResult = FinalResultOfGame::LOSE;
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
        if (player->isQuit == false)
            ua_blackjack::Game::ClientForTestUser::getInstance().askEnd(player->uid, player->finalResult); //send end request
    }
    unUsedstEnvRoomID.push(room->getRoomId()); //协程结束，return后栈中资源释放，接着回收协程的协程开始工作
    myConditionSignal(conditionForClearRoom);  //唤醒清空协程
    return NULL;
}

void *waitingSignalFromOtherModule(void *arg)
{
    co_enable_hook_sys();
    if (arg != NULL) //由main函数调用的co_create
    {
        if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_ITSELF) //正常创建信号量
        {
            conditionForWaitingRpc = createCondition(0);
        }
        else if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT)
        {
            spdlog::info("son:: complete reload!!!!");
            serviceStatus = ServiceStatus::HANDEL_GRPC_BY_ITSELF; //完成重启！！！！
        }
        ua_blackjack::Game::ServerImpl *server = (ua_blackjack::Game::ServerImpl *)arg;

        // Spawn a new CallData instance to serve new clients.
        new ua_blackjack::Game::CallData(&server->service_, server->cq_.get());
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
            static_cast<ua_blackjack::Game::CallData *>(tag)->Proceed();
        }
    }
    else if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT)
    {
        conditionForWaitingRpc = createCondition(0);
        while (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT)
        {
            while (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT)
            {
                ua_blackjack::Game::connectToParent::getInstance().mtx.lock();
                if (ua_blackjack::Game::connectToParent::getInstance().forwardRequestQueue.empty())
                {
                    ua_blackjack::Game::connectToParent::getInstance().mtx.unlock();
                    myConditionWait(conditionForWaitingRpc, 1);
                }
                else
                {
                    ua_blackjack::Game::connectToParent::getInstance().mtx.unlock();
                    break;
                }
            }
            if (serviceStatus != ServiceStatus::HANDEL_GRPC_BY_PARENT)
                break;
            spdlog::info("son receive forward message from father");
            //接收到信息了
            ua_blackjack::Request request_;
            ua_blackjack::Response responce_;
            //反序列化
            {
                std::unique_lock<std::mutex> lock(ua_blackjack::Game::connectToParent::getInstance().mtx);
                auto &str = ua_blackjack::Game::connectToParent::getInstance().forwardRequestQueue.front(); //no copy
                request_.ParseFromString(str);
                ua_blackjack::Game::connectToParent::getInstance().forwardRequestQueue.pop();
            }
            handelRpcRequest(request_, responce_, false); //处理一下返回
            {                                             //枷锁
                std::unique_lock<std::mutex> lock(ua_blackjack::Game::connectToParent::getInstance().mtx2);
                auto str = responce_.SerializeAsString();
                ua_blackjack::Game::connectToParent::getInstance().forwardResponceQueue.push(str);
                ua_blackjack::Game::connectToParent::getInstance().cond2.notify_one();
            }
        }

        ua_blackjack::Game::ServerImpl::getInstance().Run();

        spdlog::info("son::grpc async begin...");
        //重新等待RPC的协程
        co_create(&receiveSignalFromRPC, NULL, waitingSignalFromOtherModule, &ua_blackjack::Game::ServerImpl::getInstance());
        co_resume(receiveSignalFromRPC);
    }
    return NULL;
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
            auto room = roomHashMap[roomid].lock();
            roomHashMap.erase(roomid);
            int playerSize = room->playerList.size();
            spdlog::info("start delete room {0:d}", roomid);
            unUsedstEnvRoomID.pop();
            free(roomEnvirHashMap[roomid]->arg);
            co_release(roomEnvirHashMap[roomid]->coRoutine); //释放协程资源
            roomEnvirHashMap.erase(roomid);

            room->reset(); //重置房间
            roomPool[playerSize - 2].push(room);

            spdlog::info("complete delete room {0:d}", roomid);

            if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT_START_FORWARD)
            {
                if (roomEnvirHashMap.size() == 0)
                {
                    spdlog::info("Father go to the hell");
                    {
                        //加锁
                        std::unique_lock<std::mutex> lock(ua_blackjack::Game::connectToson::getInstance().mtx);
                        ua_blackjack::Game::connectToson::getInstance().forwardRequestQueue.push("Goodbye");
                        ua_blackjack::Game::connectToson::getInstance().cond.notify_one();
                    }
                }
            }
        }
    }
}
void UpdateAll(std::list<ua_blackjack::Game::Player::ptr> &list, BlackJackUID uid)
{
    for (auto player : list)
    {
        if (player->isQuit == false)
            ua_blackjack::Game::ClientForTestUser::getInstance().askUpdate(uid, player->uid);
    }
}
void UpdateAll(std::list<ua_blackjack::Game::Player::ptr> &list, BlackJackUID uid, bool showDealerHide)
{
    if (showDealerHide == true)
    {
        for (auto player : list)
        {
            if (player->isQuit == false)
                ua_blackjack::Game::ClientForTestUser::getInstance().askUpdate(uid, player->uid, showDealerHide);
        }
    }
    else //这里不在意是谁的uid了
    {
        for (auto player : list)
        {
            if (player->isQuit == false)
                ua_blackjack::Game::ClientForTestUser::getInstance().askUpdate(list, player->uid);
        }
    }
}