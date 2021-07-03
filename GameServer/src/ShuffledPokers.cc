#include "ShuffledPokers.h"
#include "spdlog/spdlog.h"
#include <sstream>
ua_blackjack::Game::ShuffledPokers::ShuffledPokers()
{
    for (int i = 0; i < 52; i++)
    {
        this->pokers.emplace_back(std::make_shared<ua_blackjack::Game::Poker>(i));
    }
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::default_random_engine(seed);
    std::shuffle(pokers.begin(), pokers.end(), rng);
}
void ua_blackjack::Game::ShuffledPokers::shuffle(void)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::default_random_engine(seed);
    std::shuffle(pokers.begin(), pokers.end(), rng);
}
void ua_blackjack::Game::ShuffledPokers::showMessage(void) const
{
    spdlog::info("ShuffledMessage-");
    for (auto &poke : pokers)
    {
        poke->showMessage();
    }
}