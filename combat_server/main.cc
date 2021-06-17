#include <iostream>
#include "Poker.h"
#include "ShuffledPokers.h"
#include "Player.h"
#include "Room.h"
int main(int agrc, char *argv[])
{
    std::list<Player::ptr> players;
    players.emplace_back(std::make_shared<Player>(0));
    players.emplace_back(std::make_shared<Player>(1));
    players.emplace_back(std::make_shared<Player>(2));
    players.emplace_back(std::make_shared<Player>(3));
    Room room(0, players);
    room.showMessage();
}