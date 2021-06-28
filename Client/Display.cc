#include "Display.h"

void Display::DisplayResponse(ua_blackjack::Response& response, const ua_blackjack::Request::RequestType& type) {
    switch (type) {
        case ua_blackjack::Request::SIGNUP:
            DisplayResponseSignUp(response);
            break;
        case ua_blackjack::Request::LOGIN:
            DisplayResponseLogin(response);
            break;
        case ua_blackjack::Request::LOGOUT:
            DisplayResponseLogout(response);
            break;
        case ua_blackjack::Request::ROOM_LIST:
            DisplayResponseRoomList(response);
            break;
        case ua_blackjack::Request::JOIN_ROOM:
            DisplayResponseJoinRoom(response);
            break;
        case ua_blackjack::Request::CREATE_ROOM:
            DisplayResponseCreateRoom(response);
            break;
        case ua_blackjack::Request::QUICK_MATCH:
            DisplayResponseQuickMatch(response);
            break;
        case ua_blackjack::Request::READY:
            DisplayResponseReady(response);
            break;
        case ua_blackjack::Request::LEAVE_ROOM:
            DisplayResponseLeaveRoom(response);
            break;
        case ua_blackjack::Request::DOUBLE:
            DisplayResponseDouble(response);
            break;
        case ua_blackjack::Request::SURRENDER:
            DisplayResponseSurrender(response);
            break;
        case ua_blackjack::Request::RANK_ME:
            DisplayResponseRankMe(response);
            break;
        case ua_blackjack::Request::RANK_TOP:
            DisplayResponseRankTop(response);
            break;
        case ua_blackjack::Request::ADD_FRIEND:
            DisplayResponseAddFriend(response);
            break;
        case ua_blackjack::Request::ACCEPT_FRIEND:
            DisplayResponseAcceptFriend(response);
            break;
        case ua_blackjack::Request::DELETE_FRIEND:
            DisplayResponseDeleteFriend(response);
            break;
        case ua_blackjack::Request::LIST_FRIEND:
            DisplayResponseFriendList(response);
            break;
        case ua_blackjack::Request::LIST_WAITTING:
            DisplayResponseWaitingFriendList(response);
            break;
    }

    std::cout << std::endl;
}

void Display::DisplayResponseSignUp(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( SignUp Failed" << std::endl;
        return;
    }

    PrintPrompt("SignUp Successful");
}

void Display::DisplayResponseLogin(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( Nickname or Password wrong" << std::endl;
        return;
    }

    PrintPrompt("Login Successful");
}
void Display::DisplayResponseLogout(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( Logout Failed" << std::endl;
        return;
    }

    PrintPrompt("Logout Successful");
}

void Display::DisplayResponseRoomList(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( RoomList Failed" << std::endl;
        return;
    }

    PrintPrompt("RoomList Successful");
    int sz = response.args().size();
    for (int i = 0; i < sz; ++i) {
        std::cout << response.args()[i] << std::endl;
    }
}

void Display::DisplayResponseJoinRoom(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( JoinRoom Failed" << std::endl;
        return;
    }

    PrintPrompt("JoinRoom Successful");
}
void Display::DisplayResponseCreateRoom(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( CreateRoom Failed" << std::endl;
        return;
    }

    PrintPrompt("CreateRoom Successful");

    std::cout << response.args()[0] << std::endl;
}
void Display::DisplayResponseQuickMatch(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( QuickMatch Failed" << std::endl;
        return;
    }

    PrintPrompt("QuickMatch Successful");
}
void Display::DisplayResponseReady(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( Ready Failed" << std::endl;
        return;
    }

    PrintPrompt("Ready Now");
}
void Display::DisplayResponseLeaveRoom(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( LeaveRoom Failed" << std::endl;
        return;
    }

    PrintPrompt("LeaveRoom Successful");
}
void Display::DisplayResponseDouble(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( Double Failed" << std::endl;
        return;
    }

    PrintPrompt("Double Successful");
}
void Display::DisplayResponseSurrender(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( Surrender Failed" << std::endl;
        return;
    }

    PrintPrompt("I will be back!");
}
void Display::DisplayResponseRankMe(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( RankMe Failed" << std::endl;
        return;
    }

    PrintPrompt("RankMe Successful");
    std::cout << "Rank: " << response.args()[0] << std::endl;
    std::cout << "Score: " << response.args()[1] << std::endl;
}
void Display::DisplayResponseRankTop(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( RankTop Failed" << std::endl;
        return;
    }

    PrintPrompt("RankTop Successful");
    int sz = response.args().size();
    for (int i = 0; i < sz; ++i) {
        std::cout << (i + 1) << ". " << response.args()[i] << std::endl;
    }
}
void Display::DisplayResponseAddFriend(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( AddFriend Failed" << std::endl;
        return;
    }
    PrintPrompt("Send AddFriend Request Successful");
}
void Display::DisplayResponseAcceptFriend(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( AcceptFriend Failed" << std::endl;
        return;
    }
    PrintPrompt("AcceptFriend Successful");
}
void Display::DisplayResponseDeleteFriend(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( DeleteFriend Failed" << std::endl;
        return;
    }
    PrintPrompt("DeleteFriend Successful");
}
void Display::DisplayResponseFriendList(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( FriendList Failed" << std::endl;
        return;
    }
    PrintPrompt("FriendList Successful");

    int sz = response.args().size();
    for (int i = 0; i < sz; ++i) {
        std::cout << response.args()[i] << std::endl;
    }
}
void Display::DisplayResponseWaitingFriendList(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( WaitingFriendList Failed" << std::endl;
        return;
    }
    PrintPrompt("WaitingFriendList Successful");

    int sz = response.args().size();
    for (int i = 0; i < sz; ++i) {
        std::cout << response.args()[i] << std::endl;
    }
}

void Display::PrintPrompt(const std::string& prompt) {
    std::string star = "************************************";

    int sz = star.size(), prompt_sz = prompt.size();
    int space_num = (sz - 2 - prompt_sz) / 2;

    std::cout << std::endl;
    std::cout << star << std::endl;
    std::cout << "*";
    for (int i = 0; i < space_num; ++i) std::cout << " ";
    std::cout << prompt;
    for (int i = 0; i < space_num; ++i) std::cout << " ";
    std::cout << "*";
    std::cout << std::endl;
    std::cout << star << std::endl;
}

void Display::DisplayCards(int idx_, std::unordered_map<int, int>& idx2uid_, std::vector<std::pair<int, int>>* cards_) {
    PrintPrompt("Cards Update");

    for (int i = 0; i < idx_; ++i) {
        std::cout << idx2uid_[i] << ": ";
        for (int j = 0; j < cards_[i].size(); ++j) {
            if (cards_[i][j].second == 0)
                std::cout << "* ";
            else
                std::cout << cards_[i][j].second << " ";
        }
        std::cout << std::endl;
    }
}