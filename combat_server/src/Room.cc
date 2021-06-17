#include "Room.h"
#include <stdlib.h>
#include <queue>
#include "co_routine.h"

std::unordered_map<BlackJackRoomID, std::weak_ptr<Room>> roomHashMap;

const Poker::ptr Room::getPokerFromShuffledPokers(void)
{
    int index = this->shuffledPokers->nowIndex;
    this->shuffledPokers->nowIndex++;
    return this->shuffledPokers->pokers[index];
}
struct stTask_t
{
    int id;
};
struct stEnv_t
{
    stCoCond_t *cond;
    std::queue<stTask_t *> task_queue;
};
void Room::gameWhile(void)
{
}

void Room::showMessage(void) const
{
    std::cout << "Room-"
              << "-roomid-" << this->rid << std::endl;
    for (auto &player : this->playerList)
    {
        player->showMessage();
    }
    // this->shuffledPokers->showMessage();
}