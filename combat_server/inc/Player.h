#pragma once
class Room;
#include "combat_typedef.h"
#include <unordered_map>
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include "Poker.h"
class Player;
extern std::unordered_map<BlackJackUID, std::weak_ptr<Player>> playerHashMap;
class Player : std::enable_shared_from_this<Player>
{
public:
    typedef std::shared_ptr<Player> ptr;
    using PokerPointer = Poker::ptr;
    using PokerList = std::list<PokerPointer>;
    PokerList pokerList;         //用户手上的牌
    BlackJackMoney bettingMoney; //用户台面上的筹码
    BlackJackUID uid;

    bool isReady = false;                //该局游戏用户是否准备
    bool isDealer = false;               //用户是否为庄家
    bool isStand = false;                //是否停牌
    FinalResultOfGame finalResult = WIN; //默认用户会赢

    int getAccumulateOfPoker(void) const;     //获取用户当前手头的牌的大小
    void hitPoker(void);                      //用户从牌堆中拿取一张牌
    void standPoker(void);                    //用户停牌
    void doubleBettingMoney(void);            //用户加倍
    void surrender(void);                     //用户投降
    void getPoker(const PokerPointer &poker); //强行拿牌,测试用

    void showMessage(void) const;
    Player(BlackJackUID _uid, BlackJackRoomID _room) : uid(_uid), room(_room){};
    Player(BlackJackUID _uid) : uid(_uid){};
    inline void setRoom(BlackJackRoomID _room) { this->room = _room; }
    inline BlackJackRoomID getRoom(void) { return room; };

private:
    BlackJackMoney bankMoney; //用户账户中的钱
    BlackJackRoomID room;     //用户当前所在房间，在一局对局当中，用户的room是不能被更改的
};