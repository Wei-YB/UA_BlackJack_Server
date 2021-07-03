#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <chrono> // std::chrono::system_clock
#include "Poker.h"
namespace ua_blackjack
{
    namespace Game
    {
        class ShuffledPokers
        {
        public:
            typedef std::shared_ptr<ShuffledPokers> ptr;

            ShuffledPokers(void);
            void showMessage(void) const;
            void shuffle(void);
            std::vector<ua_blackjack::Game::Poker::ptr> pokers;
            int nowIndex = 0; //已发到的牌
        };
    }
}