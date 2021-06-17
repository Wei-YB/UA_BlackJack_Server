#include "Player.h"
#include "Room.h"
std::unordered_map<BlackJackUID, std::weak_ptr<Player>> playerHashMap;
const int maxAcePokerValue[] = {-1, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21}; //n张ACE最大组成的值（前提是不超过21）
int Player::getAccumulateOfPoker(void) const                                                                             //获取用户当前手头的牌的大小
{
    int res = 0;
    int aceNum = 0; //A的个数
    for (auto &poke : this->pokerList)
    {
        int value = poke->getValue();
        value %= 14;    //不在意花色
        if (value == 0) //A
        {
            ++aceNum;
            continue;
        }
        else if (value < 9) //1-9
        {
            res += (value + 1);
        }
        else //10 J Q K
        {
            res += 10;
        }
    }
    if (aceNum == 0)
        return res;
    if (aceNum > 21) //超过21张ace
    {
        res += aceNum;
        return res;
    }
    int remainValue = 21 - res;
    while (remainValue < maxAcePokerValue[aceNum]) //第一张ace需要作为1
    {
        remainValue -= 1;
        res += 1;
        aceNum--;
        if (aceNum == 0) //已将所有的ace作为1了
        {
            return res;
        }
    }
    res += maxAcePokerValue[aceNum];
    return res;
}
void Player::getPoker(const Poker::ptr &poker) //强行拿一张牌
{
    this->pokerList.emplace_back(poker);
}
void Player::hitPoker(void) //用户从牌堆中拿取一张牌
{
    if (std::shared_ptr<Room> roomPtr = room.lock())
    {
        this->pokerList.push_back(roomPtr->getPokerFromShuffledPokers());
    }
}
void Player::standPoker(void) //用户停牌
{
    this->isStand = true;
}
void Player::doubleBettingMoney(void) //用户加倍
{
    this->bettingMoney *= 2;
}
void Player::showMessage(void) const
{

    std::cout << "Player-"
              << "-uid-" << this->uid << "-bankMoney-" << bankMoney << "-roomid-";
    if (std::shared_ptr<Room> roomPtr = room.lock())
    {
        std::cout << roomPtr->getRoomId() << std::endl;
    }

    for (auto &poker : pokerList)
    {
        poker->showMessage();
    }
    std::cout << "-accumulate-" << this->getAccumulateOfPoker() << std::endl
              << std::endl;
}
