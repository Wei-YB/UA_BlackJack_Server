#pragma once
#include <iostream>
#include <list>
#include <memory>
#include "co_routine_inner.h"
typedef int64_t BlackJackUID;
typedef int64_t BlackJackMoney;
typedef int64_t BlackJackRoomID;
typedef std::list<BlackJackUID> UidList;
typedef enum
{
    WIN = 0,
    DRAW = 1,
    LOSE = 2
} FinalResultOfGame;
typedef enum
{
    OPERATE_HIT = 0,  //拿牌
    OPERATE_STAND,    //停牌
    OPERATE_BETMONEY, //设置钱数
    OPERATE_SURRAND,  //投降
    OPERATE_QUIT,
} OperateID;
struct HitArgument
{
    BlackJackUID uid;
};
struct SurrandArgument
{
    BlackJackUID uid;
};
struct QuitArgument
{
    BlackJackUID uid;
};
struct BetMoneyArgument
{
    BlackJackUID uid;
    BlackJackMoney money;
};
struct stEnv_t
{
    typedef std::shared_ptr<stEnv_t> ptr;
    stCoRoutine_t *coRoutine; //协程句柄
    BlackJackRoomID roomID;   //创建的房间号
    UidList &uids;            //所有玩家的uid
    int cond;                 //信号量
    OperateID operateId;      //操作码
    void *arg;                //操作数
    int sizeOfCompleteBetting = 0;
    stEnv_t(BlackJackRoomID _roomID, UidList &_uids) : roomID(_roomID), uids(_uids){};
};
#define LOG_ON
extern std::string ProxyServiceAddr;
extern std::string LobbyServiceAddr;
extern std::string DatabaseServiceAddr;
extern std::string controlTcpPort;
extern std::string gameRpcAddr;
extern bool isProgramRelase;
extern uint64_t sumOfRoom;