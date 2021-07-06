#pragma once
#include "combat_typedef.h"
#include <iostream>
#include <list>
#include <queue>
#include <memory>
#include <unordered_map>
#include "ShuffledPokers.h"
#include "Player.h"
#include "spdlog/spdlog.h"
#include <sstream>
#include "ClientForDatebase.h"
#include "ClientForTestUser.h"
namespace ua_blackjack
{
    namespace Game
    {
        class Room;

        class Room : std::enable_shared_from_this<Room>
        {
        public:
            BlackJackRoomID rid = -1;

        public:
            typedef std::shared_ptr<Room> ptr;
            using PalyerPointer = ua_blackjack::Game::Player::ptr;
            using PlayerList = std::list<PalyerPointer>;

            inline BlackJackRoomID getRoomId(void) { return rid; }
            Room(BlackJackRoomID _rid, UidList &uids) : rid(_rid)
            {
                shuffledPokers = std::make_shared<ua_blackjack::Game::ShuffledPokers>();
                for (auto i : uids)
                {
                    playerList.emplace_back(std::make_shared<ua_blackjack::Game::Player>(i));
                    playerList.back()->setRoom(rid);
                    playerHashMap[i] = playerList.back(); //这个不能放在player的构造函数中，因为构造函数执行完时暂时没有shared_ptr指向他
                    if (isProgramRelase == true)
                    {
                        ua_blackjack::Game::ClientForDatebase::getInstance().askPlayerNickName(i); //设置nickname
                    }
                }
                playerList.front()->isDealer = true; //创建房间时，庄家为第一个进入房间的用户
            };
            Room(BlackJackRoomID _rid, PlayerList &_players) : rid(_rid), playerList(_players)
            {
                shuffledPokers = std::make_shared<ua_blackjack::Game::ShuffledPokers>();
                for (auto &player : playerList)
                {
                    playerHashMap[player->uid] = player; //这个不能放在player的构造函数中，因为构造函数执行完时暂时没有shared_ptr指向他
                    playerList.back()->setRoom(rid);
                }
                playerList.front()->isDealer = true; //创建房间时，庄家为第一个进入房间的用户
            };
            void showMessage(void) const;
            ua_blackjack::Game::Poker::ptr getPokerFromShuffledPokers(void);

            void judgeWinOrLose(void); //判断最后的输赢
            int setBettingMoney(BlackJackUID uid, BlackJackMoney money);
            void deleteRoom(void);
            ~Room();
            PlayerList playerList;
            void reset(void);
            bool isGameBegin = false;

        private:
            ua_blackjack::Game::ShuffledPokers::ptr shuffledPokers;
            BlackJackMoney dealerFinalWinMoney = 0;
        };
    }
}

ua_blackjack::Game::Room::ptr malloOneRoom(BlackJackRoomID rid, UidList &uids);
extern std::unordered_map<BlackJackRoomID, std::weak_ptr<ua_blackjack::Game::Room>> roomHashMap;
extern std::queue<std::shared_ptr<ua_blackjack::Game::Room>> roomPool[];
extern std::vector<std::shared_ptr<ua_blackjack::Game::Room>> roomLists;