#pragma once
#include <iostream>
#include <queue>
#include "Poker.h"
#include "ShuffledPokers.h"
#include "Player.h"
#include "Room.h"
#include "co_routine.h"
#include "ServerAsync.h"
#include "MycondintonCo.h"
#include "ClientForTestUser.h"

extern std::queue<BlackJackRoomID> unUsedstEnvRoomID;                      //结束了的游戏ID
extern std::unordered_map<BlackJackRoomID, stEnv_t::ptr> roomEnvirHashMap; //roomid和句柄的hash映射
extern stCoRoutine_t *receiveSignalFromRPC;
extern stCoRoutine_t *recoverystCo;
int createstEnv_t(BlackJackRoomID roomID, UidList &uids);
void *createOneGame(void *arg);
void *waitingSignalFromOtherModule(void *arg);
void *recoveryUnusedCo(void *arg); //回收协程的协程
void askPlayerToSetBettingMoney(BlackJackUID uid);
void UpdateAll(std::list<ua_blackjack::Game::Player::ptr> &list, BlackJackUID uid);
void UpdateAll(std::list<ua_blackjack::Game::Player::ptr> &list, BlackJackUID uid, bool showDealerHide);