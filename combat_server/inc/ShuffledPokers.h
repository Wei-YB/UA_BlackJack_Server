#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <chrono> // std::chrono::system_clock
#include "Poker.h"
class Poker;
class ShuffledPokers
{
public:
    typedef std::shared_ptr<ShuffledPokers> ptr;

    ShuffledPokers(void);
    void showMessage(void) const;

    std::vector<Poker::ptr> pokers;
    int nowIndex = 0; //已发到的牌
};