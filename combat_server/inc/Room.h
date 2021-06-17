#pragma once
#include "combat_typedef.h"
#include <iostream>
#include <list>
#include <memory>
#include "ShuffledPokers.h"
#include "Player.h"
class Room
{
public:
    typedef std::shared_ptr<Room> ptr;
    bool isGameBegin = false; //对局是否开始
    inline black_jack_room_id_t getRoomId(void) { return rid; }
    Room(black_jack_room_id_t _rid) : rid(_rid)
    {
        shuffledPokers = std::make_shared<ShuffledPokers>();
    };
    Room(black_jack_room_id_t _rid, std::list<Player::ptr> &_players) : rid(_rid), players(_players)
    {
        shuffledPokers = std::make_shared<ShuffledPokers>();
        for (auto &player : players)
        {
            player->setRoom(this);
        }
    };
    void showMessage(void) const;
    const Poker::ptr getPokerFromShuffledPokers(void);
    void gameWhile(void);

private:
    black_jack_room_id_t rid;
    ShuffledPokers::ptr shuffledPokers;
    std::list<Player::ptr> players;
};