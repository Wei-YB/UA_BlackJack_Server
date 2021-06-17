#include "Room.h"
const Poker::ptr Room::getPokerFromShuffledPokers(void)
{
    int index = this->shuffledPokers->nowIndex;
    this->shuffledPokers->nowIndex++;
    return this->shuffledPokers->pokers[index];
}

void Room::gameWhile(void)
{
}

void Room::showMessage(void) const
{
    std::cout << "Room-"
              << "-roomid-" << this->rid << std::endl;
    for (auto &player : this->players)
    {
        player->showMessage();
    }
    // this->shuffledPokers->showMessage();
}