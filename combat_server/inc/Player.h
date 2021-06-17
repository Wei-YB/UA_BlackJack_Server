#pragma once
#include "combat_typedef.h"
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include "Poker.h"
class Room;
class Player
{
public:
    typedef std::shared_ptr<Player> ptr;

    std::list<Poker::ptr> pokers;    //用户手上的牌
    black_jack_money_t bettingMoney; //用户台面上的筹码

    bool isReady = false;  //该局游戏用户是否准备
    bool isDealer = false; //用户是否为庄家
    bool isStand = false;  //是否停牌

    int getAccumulateOfPoker(void) const;   //获取用户当前手头的牌的大小
    void hitPoker(void);                    //用户从牌堆中拿取一张牌
    void standPoker(void);                  //用户停牌
    void doubleBettingMoney(void);          //用户加倍
    void getPoker(const Poker::ptr &poker); //强行拿牌

    void showMessage(void) const;
    Player(black_jack_uid_t _uid, Room *_room) : uid(_uid), room(_room){};
    Player(black_jack_uid_t _uid) : uid(_uid){};
    inline void setRoom(Room *_room) { this->room = _room; }

private:
    black_jack_uid_t uid;
    black_jack_money_t bankMoney; //用户账户中的钱
    Room *room;                   //用户当前所在房间，在一局对局当中，用户的room是不能被更改的
};