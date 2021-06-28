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

#define PRINT_LOG

#define ProxyServiceAddr "9.134.125.154:50050"
#define LobbyServiceAddr "9.134.69.87:50050"
#define DatabaseServiceAddr "9.134.69.87:50050"