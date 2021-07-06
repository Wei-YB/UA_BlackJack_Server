#include "ServiceCallback.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

void nomalStartGameCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce)
{
    auto args = request.args();
    responce.set_status(0);
    UidList uids;
    BlackJackRoomID roomid = atoi(args[0].c_str());

    spdlog::info("Create Room Ask come roomid {0}", roomid);
    for (int i = 1; i < args.size(); i++)
    {
        uids.push_back(atoi(args[i].c_str()));
    }

    createstEnv_t(roomid, uids);
}
void nomalLeaveRoomCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce)
{
    responce.set_status(0);
    auto uid = request.uid();
    if (auto playerPtr = playerHashMap[uid].lock())
    {
        spdlog::info("{0:d} quit by itself", uid);
        playerPtr->quit(); //托管

        int roomId = playerPtr->getRoom();
        auto env = roomEnvirHashMap[roomId];
        { //加锁
            std::lock_guard<std::mutex> lock(env->mutex);
            if (playerPtr->isFinishBetting == false) //在托管阶段就quit了
            {
                auto room = roomHashMap[roomId];
                env->sizeOfCompleteBetting++;
                if (env->sizeOfCompleteBetting >= room.lock()->playerList.size())
                {
                    myConditionSignal(env->cond); //所有玩家都收到筹码了
                }
            }
            if (playerPtr->isWaitingReply == true)
            {
                myConditionSignal(env->cond);
            }
        }
    }
    else
    {
        spdlog::error("Quit error uid {0:d}   not existed in any room", uid);
    }
}
void nomalDoubleCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce)
{
    auto uid = request.uid();
    if (auto playerPtr = playerHashMap[uid].lock())
    {
        responce.set_status(0);
        spdlog::info("{0:d} double by itself", uid);
        playerPtr->bettingMoney *= 2;
    }
    else
    {
        responce.set_status(-1);
        spdlog::error("Double error uid {0:d}   not existed in any room", uid);
    }
}
void nomalSurrenderCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce)
{
    auto uid = request.uid();
    if (auto playerPtr = playerHashMap[uid].lock())
    {
        responce.set_status(0);
        spdlog::info("{0:d} surrond by itself", uid);
        playerPtr->bettingMoney /= 2;
        playerPtr->isStand = true;
        playerPtr->finalResult = FinalResultOfGame::LOSE;
        if (playerPtr->isWaitingReply == true)
        {
            int roomId = playerPtr->getRoom();
            auto env = roomEnvirHashMap[roomId];
            { //加锁
                std::lock_guard<std::mutex> lock(env->mutex);
                myConditionSignal(env->cond);
            }
        }
    }
    else
    {
        responce.set_status(-1);
        spdlog::error("Surrond error uid {0:d}   not existed in any room", uid);
    }
}

void rpcForwardCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce, MSG_KEY_E key)
{
    auto req = request.SerializeAsString(); //序列化request
    int msgid = -1;
    struct msg_st data;
    strcpy(data.text, req.c_str());
    //发送request
    msgid = msgget((key_t) static_cast<int>(key), 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        spdlog::error("msgget failed with error");
        exit(EXIT_FAILURE);
    }
    if (msgsnd(msgid, (void *)&data, MAX_TEXT, 0) == -1)
    {
        spdlog::error("msgsnd failed");
        exit(EXIT_FAILURE);
    }

    //接受responce
    msgid = msgget((key_t) static_cast<int>(key) + 100, 0666 | IPC_CREAT);
    if (msgrcv(msgid, (void *)&data, BUFSIZ, 0, 0) == -1)
    {
        fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    std::string res(data.text);
    responce.ParseFromString(res);
}