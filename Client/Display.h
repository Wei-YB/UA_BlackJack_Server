#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>

#include "UA_BlackJack.pb.h"

class Display {
public:
    static Display& DisplayInstance() {
        static Display instance;
        return instance;
    }

    Display(const Display&) = delete;
    void operator=(const Display&) = delete;

    void DisplayResponse(ua_blackjack::Response& response, const ua_blackjack::Request::RequestType& type);
    void PrintPrompt(const std::string& prompt);
    void DisplayCards(int idx_, std::unordered_map<int, int>& idx2uid_, std::vector<std::pair<int, int>>* cards_);

private:
    Display() = default;

    void DisplayResponseSignUp(ua_blackjack::Response& response);
    void DisplayResponseLogin(ua_blackjack::Response& response);
    void DisplayResponseLogout(ua_blackjack::Response& response);
    void DisplayResponseRoomList(ua_blackjack::Response& response);
    void DisplayResponseJoinRoom(ua_blackjack::Response& response);
    void DisplayResponseCreateRoom(ua_blackjack::Response& response);
    void DisplayResponseQuickMatch(ua_blackjack::Response& response);
    void DisplayResponseReady(ua_blackjack::Response& response);
    void DisplayResponseLeaveRoom(ua_blackjack::Response& response);
    void DisplayResponseDouble(ua_blackjack::Response& response);
    void DisplayResponseSurrender(ua_blackjack::Response& response);
    void DisplayResponseRankMe(ua_blackjack::Response& response);
    void DisplayResponseRankTop(ua_blackjack::Response& response);
    void DisplayResponseAddFriend(ua_blackjack::Response& response);
    void DisplayResponseAcceptFriend(ua_blackjack::Response& response);
    void DisplayResponseDeleteFriend(ua_blackjack::Response& response);
    void DisplayResponseFriendList(ua_blackjack::Response& response);
    void DisplayResponseWaitingFriendList(ua_blackjack::Response& response);
};

#endif