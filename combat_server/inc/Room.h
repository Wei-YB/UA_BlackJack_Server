#pragma once
#include "combat_typedef.h"
#include <iostream>
#include <list>
#include <memory>
#include <unordered_map>
#include "ShuffledPokers.h"
#include "Player.h"
class Room;
extern std::unordered_map<BlackJackRoomID, std::weak_ptr<Room>> roomHashMap;
class Room : std::enable_shared_from_this<Room>
{
private:
    BlackJackRoomID rid = -1;

public:
    typedef std::shared_ptr<Room> ptr;
    using PalyerPointer = Player::ptr;
    using PlayerList = std::list<PalyerPointer>;

    bool isGameBegin = false; //对局是否开始
    inline BlackJackRoomID getRoomId(void) { return rid; }
    Room(BlackJackRoomID _rid, std::list<int> &uids) : rid(_rid)
    {
        shuffledPokers = std::make_shared<ShuffledPokers>();
        for (auto i : uids)
        {
            playerList.emplace_back(std::make_shared<Player>(i));
            playerList.back()->setRoom(rid);
            playerHashMap[i] = playerList.back(); //这个不能放在player的构造函数中，因为构造函数执行完时暂时没有shared_ptr指向他
        }
        playerList.front()->isDealer = true; //创建房间时，庄家为第一个进入房间的用户
    };
    Room(BlackJackRoomID _rid, PlayerList &_players) : rid(_rid), playerList(_players)
    {
        shuffledPokers = std::make_shared<ShuffledPokers>();
        for (auto &player : playerList)
        {
            playerHashMap[player->uid] = player; //这个不能放在player的构造函数中，因为构造函数执行完时暂时没有shared_ptr指向他
            playerList.back()->setRoom(rid);
        }
        playerList.front()->isDealer = true; //创建房间时，庄家为第一个进入房间的用户
    };
    void showMessage(void) const;
    Poker::ptr getPokerFromShuffledPokers(void);
    inline void setExpire(void) { expired = true; };
    inline bool isExpired(void) { return expired; };

    void judgeWinOrLose(void); //判断最后的输赢
    int setBettingMoney(BlackJackUID uid, BlackJackMoney money);

    ~Room();
    PlayerList playerList;

private:
    ShuffledPokers::ptr shuffledPokers;
    BlackJackMoney dealerFinalWinMoney = 0;
    bool expired = false; //房间是否已经解散
};
Room::ptr malloOneRoom(BlackJackRoomID rid, std::list<int> &uids);