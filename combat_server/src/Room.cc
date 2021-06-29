#include "Room.h"
#include <stdlib.h>
#include <queue>
#include "co_routine.h"
#include "ClientForDatebase.h"
#include "ClientForLobby.h"
#include "ClientForTestUser.h"
#include "spdlog/spdlog.h"
#include <sstream>
#include "GameProcess.h"
std::unordered_map<BlackJackRoomID, std::weak_ptr<Room>> roomHashMap;
Room::ptr malloOneRoom(BlackJackRoomID rid, UidList &uids)
{

    Room::ptr room = std::make_shared<Room>(rid, uids);
    roomHashMap[room->getRoomId()] = room;
    return room;
}
Poker::ptr Room::getPokerFromShuffledPokers(void)
{
    int index = this->shuffledPokers->nowIndex;
    this->shuffledPokers->nowIndex++;
    return this->shuffledPokers->pokers[index];
}

void Room::showMessage(void) const
{
    std::stringstream ss;
    ss << "Room-"
       << "-roomid-" << this->rid;
    spdlog::info(ss.str());
    for (auto &player : this->playerList)
    {
        player->showMessage();
    }
    // this->shuffledPokers->showMessage();
}

void Room::judgeWinOrLose(void) //判断最后的输赢
{
    //庄家在游戏结束前投降了或提前退出游戏了或爆了
    if (playerList.front()->finalResult != WIN)
    {
        for (auto &player : playerList)
        {
            if (player->finalResult != WIN) //玩家在游戏结束前投降了或提前退出游戏了，不能拿钱
            {
                continue;
            }
            player->finalResult = WIN; //玩家胜利
        }
        return;
    }

    //庄家坚持到了最后
    auto dealer = playerList.front();
    int dealerFinalGameValue = dealer->getAccumulateOfPoker();
    while (dealerFinalGameValue < 17)
    {
        dealer->hitPoker(); //抽牌
        UpdateAll(playerList, dealer->uid);
        dealer->showMessage();
        int val = dealer->pokerList.back()->getValue() % 13 + 1;
        if (val == 1) //ace
        {
            if (dealerFinalGameValue <= 10)
            {
                dealerFinalGameValue += 11;
                continue;
            }
            else
            {
                dealerFinalGameValue += 1;
                continue;
            }
        }
        else if (val <= 9)
        {
            dealerFinalGameValue += val;
            continue;
        }
        else
        {
            dealerFinalGameValue += 10;
            continue;
        }
    }
    if (dealerFinalGameValue > 21) //庄家抽爆了
    {
        playerList.front()->finalResult = LOSE;
        for (auto &player : playerList)
        {
            if (player->isDealer == true)
            {
                continue;
            }
        }
        judgeWinOrLose(); //没输的玩家全赢
        return;
    }

    //庄家没有抽爆
    for (auto &player : playerList)
    {
        if (player->finalResult != WIN || player->isDealer == true) //玩家在游戏结束前投降了或提前退出游戏或爆了
        {
            continue;
        }
        int playerFinalValue = player->getAccumulateOfPoker();
        if (playerFinalValue < dealerFinalGameValue)
        {
            player->finalResult = LOSE;
        }
        else if (playerFinalValue > dealerFinalGameValue)
        {
            player->finalResult = WIN;
        }
        else
        {
            player->finalResult = DRAW;
        }
    }
}

int Room::setBettingMoney(BlackJackUID uid, BlackJackMoney money)
{
    if (Player::ptr player = playerHashMap[uid].lock())
    {
        if (auto room = roomHashMap[player->getRoom()].lock())
        {
            if (room.get() != this) //不是这个房间的用户
                return -1;
        }
        else
        {
            return -1;
        }
        player->bettingMoney = money;
        return 0;
    }
    return -1;
}
void Room::deleteRoom(void)
{
    this->judgeWinOrLose();
    for (auto player : this->playerList)
    {
        if (player->isDealer)
            continue;
        if (player->finalResult == WIN)
        {
            dealerFinalWinMoney -= player->bettingMoney;
        }
        else if (player->finalResult == LOSE)
        {
            dealerFinalWinMoney += player->bettingMoney;
        }
    }
    if (dealerFinalWinMoney > 0)
    {
        this->playerList.front()->bettingMoney = dealerFinalWinMoney;
        this->playerList.front()->finalResult = WIN;
    }
    else if (dealerFinalWinMoney < 0)
    {
        this->playerList.front()->bettingMoney = -dealerFinalWinMoney;
        this->playerList.front()->finalResult = LOSE;
    }
    else
    {
        this->playerList.front()->finalResult = DRAW;
    }

    spdlog::info("game over");
    for (auto &player : playerList)
    {
        player->showMessage();
    }
    for (auto &player : playerList)
    {
        if (player->finalResult == WIN)
            spdlog::info("{0:d} WIN {1:d}", player->uid, player->bettingMoney);
        else if (player->finalResult == DRAW)
            spdlog::info("{0:d} Draw ", player->uid);
        else if (player->finalResult == LOSE)
            spdlog::info("{0:d} LOSE {1:d}", player->uid, player->bettingMoney);
    }

    /*************存储数据到数据库**************/
    ua_blackjack::Game::ClientForDatebase::getInstance().matchEnd(this->playerList); //向数据库发送
    ua_blackjack::Game::ClientForLobby::getInstance().matchEnd(this->getRoomId());   //向lobby发送
    /*************存储数据到数据库**************/
}
Room::~Room() //房间解散
{
}