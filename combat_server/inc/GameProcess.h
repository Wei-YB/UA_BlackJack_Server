#pragma once
#include <iostream>
#include <queue>
#include "Poker.h"
#include "ShuffledPokers.h"
#include "Player.h"
#include "Room.h"
#include "co_routine.h"
#include "server-asyn.h"
#include "MycondintonCo.h"
#include "AskForUserRequest.h"
struct stEnv_t
{
    typedef std::shared_ptr<stEnv_t> ptr;
    stCoRoutine_t *coRoutine; //协程句柄
    BlackJackRoomID roomID;   //创建的房间号
    UidList &uids;            //所有玩家的uid
    int cond;                 //信号量
    OperateID operateId;      //操作码
    void *arg;                //操作数
    stEnv_t(BlackJackRoomID _roomID, UidList &_uids) : roomID(_roomID), uids(_uids){};
};
extern std::queue<BlackJackRoomID> unUsedstEnvRoomID;                      //结束了的游戏ID
extern std::unordered_map<BlackJackRoomID, stEnv_t::ptr> roomEnvirHashMap; //roomid和句柄的hash映射
extern std::unordered_map<BlackJackRoomID, bool> roomEnvirExistHashMap;    //roomid和句柄是否存在的hash映射
extern stCoRoutine_t *receiveSignalFromRPC;
extern stCoRoutine_t *recoverystCo;
int createstEnv_t(BlackJackRoomID roomID, UidList &uids);
void *createOneGame(void *arg);
void *waitingSignalFromOtherModule(void *arg);
void *recoveryUnusedCo(void *arg); //回收协程的协程
void askPlayerToSetBettingMoney(BlackJackUID uid);
void UpdateAll(std::list<Player::ptr> &list, BlackJackUID uid);