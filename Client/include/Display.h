#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <unordered_set>
#include <vector>

#include "UA_BlackJack.pb.h"

namespace ua_blackjack {
namespace display {
class Display {
public:
    static Display &DisplayInstance() {
        static Display instance;
        return instance;
    }

    Display(const Display &) = delete;
    void operator=(const Display &) = delete;

    void DisplayResponse(ua_blackjack::Response &response, const ua_blackjack::Request::RequestType &type);
    void PrintPrompt(const std::string &prompt);
    void DisplayCards(int idx_, std::unordered_map<int, std::string> &idx2name_,
                      std::unordered_map<std::string, int> &name2idx_, std::string &name_,
                      std::vector<std::pair<int, int>> *cards_, bool dealer_);

    void DisplayHelp();
    void DisplayGameEnd(std::string &result);

private:
    Display() = default;

    void DisplayResponseSignUp(ua_blackjack::Response &response);
    void DisplayResponseLogin(ua_blackjack::Response &response);
    void DisplayResponseLogout(ua_blackjack::Response &response);
    void DisplayResponseRoomList(ua_blackjack::Response &response);
    void DisplayResponseJoinRoom(ua_blackjack::Response &response);
    void DisplayResponseCreateRoom(ua_blackjack::Response &response);
    void DisplayResponseQuickMatch(ua_blackjack::Response &response);
    void DisplayResponseReady(ua_blackjack::Response &response);
    void DisplayResponseLeaveRoom(ua_blackjack::Response &response);
    void DisplayResponseDouble(ua_blackjack::Response &response);
    void DisplayResponseSurrender(ua_blackjack::Response &response);
    void DisplayResponseRankMe(ua_blackjack::Response &response);
    void DisplayResponseRankTop(ua_blackjack::Response &response);
    void DisplayResponseAddFriend(ua_blackjack::Response &response);
    void DisplayResponseAcceptFriend(ua_blackjack::Response &response);
    void DisplayResponseDeleteFriend(ua_blackjack::Response &response);
    void DisplayResponseDeleteWaitingFriend(ua_blackjack::Response &response);
    void DisplayResponseFriendList(ua_blackjack::Response &response);
    void DisplayResponseWaitingFriendList(ua_blackjack::Response &response);
    void DisplayResponseMatchList(ua_blackjack::Response &response);
    void DisplayResponseMatchInfo(ua_blackjack::Response &response);
};
}  // namespace display
}  // namespace ua_blackjack

#endif