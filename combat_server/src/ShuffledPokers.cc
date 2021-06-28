#include "ShuffledPokers.h"
#include "spdlog/spdlog.h"
#include <sstream>
ShuffledPokers::ShuffledPokers()
{
    for (int i = 0; i < 52; i++)
    {
        this->pokers.emplace_back(std::make_shared<Poker>(i));
    }
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::default_random_engine(seed);
    std::shuffle(pokers.begin(), pokers.end(), rng);
}
void ShuffledPokers::showMessage(void) const
{
    spdlog::info("ShuffledMessage-");
    for (auto &poke : pokers)
    {
        poke->showMessage();
    }
}