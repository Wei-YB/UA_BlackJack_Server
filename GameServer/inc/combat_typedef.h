#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include "co_routine_inner.h"
typedef int64_t BlackJackUID;
typedef int64_t BlackJackMoney;
typedef int64_t BlackJackRoomID;
typedef std::list<BlackJackUID> UidList;
enum class FinalResultOfGame
{
    WIN = 0,
    DRAW = 1,
    LOSE = 2
};
enum class OperateID
{
    OPERATE_HIT = 0,  //拿牌
    OPERATE_STAND,    //停牌
    OPERATE_BETMONEY, //设置钱数
    OPERATE_SURRAND,  //投降
    OPERATE_QUIT,
};
enum class ServiceStatus
{
    HANDEL_GRPC_BY_PARENT = 0, //由父进程管理rpc的发送，子进程只做callback具体内容
    HANDEL_GRPC_BY_ITSELF,
};
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
    unsigned int sizeOfCompleteBetting = 0;
    stEnv_t(BlackJackRoomID _roomID, UidList &_uids) : roomID(_roomID), uids(_uids){};

    std::mutex mutex;
};
#define LOG_ON
extern std::string ProxyServiceAddr;
extern std::string LobbyServiceAddr;
extern std::string DatabaseServiceAddr;
extern std::string controlTcpPort;
extern std::string gameRpcAddr;
extern bool isProgramRelase;
extern uint64_t sumOfRoom;
extern ServiceStatus serviceStatus;