#include "ShuffledPokers.h"
ShuffledPokers::ShuffledPokers()
{
    for (int i = 0; i < 52; i++)
    {
        this->pokers.emplace_back(std::make_shared<Poker>(i));
    }
    auto rng = std::default_random_engine{};
    std::shuffle(pokers.begin(), pokers.end(), rng);
}
void ShuffledPokers::showMessage(void) const
{
    std::cout << "ShuffledMessage-" << std::endl;
    for (auto &poke : pokers)
    {
        poke->showMessage();
    }
}