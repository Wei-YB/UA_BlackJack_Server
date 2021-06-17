#include <iostream>
#include "Poker.h"
#include "ShuffledPokers.h"
#include "Player.h"
#include "Room.h"
#include "co_routine.h"

int main(int agrc, char *argv[])
{
    if (agrc != 2)
    {
        std::cout << "Give number of player . First one is dealer" << std::endl;
        return -1;
    }
    const int playerSize = atoi(argv[1]);
    while (1)
    {
        std::list<int> uid;

        for (int i = 0; i < playerSize; i++) //id号递增
        {
            uid.push_back(i);
        }
        auto room = malloOneRoom(0, uid);

        // 玩家设置betting money

        for (int i = 1; i < playerSize; i++)
        {
            int id = i;
            int betMoney;
            std::cin >> betMoney;
            room->setBettingMoney(i, betMoney);
        }

        auto &playerList = room->playerList;

        //发初始牌
        for (int i = 0; i < 2; i++)
        {
            for (auto &player : playerList)
            {
                player->hitPoker();
            }
        }
        playerList.front()->pokerList.front()->setHide();

        room->showMessage();
        //正常玩
        while (true)
        {
            //玩家抽卡
            char hasPlayerContinuePlay = false;
            for (auto &player : playerList)
            {
                if (player->isDealer == true)
                    continue;
                if (player->isStand == true) //已停牌
                    continue;
                hasPlayerContinuePlay = true; //有玩家可以抽牌
                char in;
                std::cout << "Hit Or Stand(input h/s)" << std::endl;
                std::cin >> in;
                if (in == 'h')
                    player->hitPoker();
                else
                    player->standPoker();
            }
            room->showMessage();
            if (hasPlayerContinuePlay == false) //所有玩家都停牌了
                break;
        }

        //游戏结束
    }
}