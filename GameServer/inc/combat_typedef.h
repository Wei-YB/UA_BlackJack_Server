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
    HANDEL_GRPC_BY_PARENT = 0,           //命令行启动的状态，所有命令均由启动的服务管理并通过该服务发送回去
    HANDEL_GRPC_BY_ITSELF,               //通过fork->exec创建的子进程刚启动时的状态，此时对于父进程的grpc命令将在子进程进行callback但仍通过父进程的grpc进行通信
    HANDEL_GRPC_BY_PARENT_START_FORWARD, //父进程接收到了restart的命令，对于手头的已经创建了房间的玩意儿都在自己这边处理，对于新来的连接将命令转发给子进程，并通过自己的rpc与其他模块进行通信。这个状态结束之后应该完全退出父进程
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
enum class MSG_KEY_E
{
    MSG_KEY_GAME_START = 123,
    MSG_KEY_LEAVE_ROOM = 124,
    MSG_KEY_DOUBLE = 125,
    MSG_KEY_SURRENDER = 126,
    MSG_KEY_COMPLETE_ALL_OPE = 127,
};

extern std::string ProxyServiceAddr;
extern std::string LobbyServiceAddr;
extern std::string DatabaseServiceAddr;
extern std::string controlTcpPort;
extern std::string gameRpcAddr;
extern bool isProgramRelase;
extern uint64_t sumOfRoom;
extern ServiceStatus serviceStatus;
extern std::string configFilePath;
extern std::string logFilePath;