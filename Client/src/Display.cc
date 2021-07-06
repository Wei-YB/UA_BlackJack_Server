#include "Display.h"

#include "global.h"

using namespace ua_blackjack::display;

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
        case ua_blackjack::Request::DELETE_WAIT_FRIEND:
            DisplayResponseDeleteWaitingFriend(response);
            break;
        case ua_blackjack::Request::LIST_FRIEND:
            DisplayResponseFriendList(response);
            break;
        case ua_blackjack::Request::LIST_WAITTING:
            DisplayResponseWaitingFriendList(response);
            break;
        case ua_blackjack::Request::LIST_MATCH:
            DisplayResponseMatchList(response);
            break;
        case ua_blackjack::Request::GET_MATCH_INFO:
            DisplayResponseMatchInfo(response);
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
    std::cout << "uid:" << response.uid() << std::endl;
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

    if (sz == 0) std::cout << "No available room" << std::endl;
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

    std::cout << "Room id: " << response.args()[0] << std::endl;
}
void Display::DisplayResponseQuickMatch(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( QuickMatch Failed" << std::endl;
        return;
    }

    PrintPrompt("QuickMatch Successful");

    std::cout << "Room id: " << response.args()[0] << std::endl;
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

    PrintPrompt("Surrender Successful");
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
        if (response.args().size() > 0) {
            if (response.args()[0] == "none") std::cout << ":( AddFriend Failed, user doesn't exist" << std::endl;
            if (response.args()[0] == "self")
                std::cout << ":( AddFriend Failed, can't operate on yourself" << std::endl;
            if (response.args()[0] == "friend") std::cout << ":( AddFriend Failed, already your friend" << std::endl;
        } else {
            std::cout << ":( Failed, Already in user's WaitingFriendList" << std::endl;
        }
        return;
    }
    PrintPrompt("Send AddFriend Request Successful");
}
void Display::DisplayResponseAcceptFriend(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        if (response.args().size() > 0) {
            if (response.args()[0] == "none") std::cout << ":( AcceptFriend Failed, user doesn't exist" << std::endl;
            if (response.args()[0] == "self")
                std::cout << ":( AcceptFriend Failed, can't operate on yourself" << std::endl;
            if (response.args()[0] == "bad")
                std::cout << ":( AcceptFriend Failed, doesn't exist on the waiting list" << std::endl;
        } else {
            std::cout << ":( AcceptFriend Failed, unknow error" << std::endl;
        }
        return;
    }
    PrintPrompt("AcceptFriend Successful");
}
void Display::DisplayResponseDeleteFriend(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        if (response.args().size() > 0) {
            if (response.args()[0] == "none" || response.args()[0] == "not")
                std::cout << ":( DeleteFriend Failed, user is not your friend" << std::endl;
            if (response.args()[0] == "self")
                std::cout << ":( DeleteFriend Failed, can't operate on yourself" << std::endl;

        } else {
            std::cout << ":( DeleteFriend Failed, unknow error" << std::endl;
        }
        return;
    }
    PrintPrompt("DeleteFriend Successful");
}
void Display::DisplayResponseDeleteWaitingFriend(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        if (response.args().size() > 0) {
            if (response.args()[0] == "none" || response.args()[0] == "not")
                std::cout << ":( DeleteWaitingFriend Failed, user not in the waiting list" << std::endl;
            if (response.args()[0] == "self")
                std::cout << ":( DeleteWaitingFriend Failed, can't operate on yourself" << std::endl;
        } else {
            std::cout << ":( DeleteWaitingFriend Failed, unknow error" << std::endl;
        }
        return;
    }
    PrintPrompt("DeleteWaitingFriend Successful");
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

    if (sz == 0) std::cout << "No available friend" << std::endl;
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

    if (sz == 0) std::cout << "No available waiting friend" << std::endl;
}

void Display::DisplayResponseMatchList(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( MatchList Failed" << std::endl;
        return;
    }
    PrintPrompt("MatchList Successful");

    int sz = response.args().size();
    for (int i = 0; i < sz; ++i) {
        std::cout << response.args()[i] << std::endl;
    }

    if (sz == 0) std::cout << "No available match list" << std::endl;
}

void Display::DisplayResponseMatchInfo(ua_blackjack::Response& response) {
    if (response.status() == -1) {
        std::cout << ":( MatchInfo Failed" << std::endl;
        return;
    }
    PrintPrompt("MatchInfo Successful");

    int sz = response.args().size();
    assert(sz >= 2);

    std::cout << response.args()[0] << ": " << response.args()[1] << std::endl;
    for (int i = 2; i < sz; i += 2) {
        std::cout << response.args()[i] << ": " << response.args()[i + 1] << std::endl;
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

void Display::DisplayCards(int idx_, std::unordered_map<int, std::string>& idx2name_,
                           std::unordered_map<std::string, int>& name2idx_, std::string& name_,
                           std::vector<std::pair<int, int>>* cards_, bool dealer_) {
    PrintPrompt("Cards Update");

    logger->info("display cards");
    int dealer_idx = -1;
    if (dealer_) {
        dealer_idx = name2idx_[name_];
    } else {
        for (int i = 0; i < idx_; ++i) {
            if (cards_[i][0].second == 0) dealer_idx = i;
        }
    }

    if (dealer_idx >= 0 && cards_[dealer_idx].size() == 3) {
        cards_[dealer_idx][0] = cards_[dealer_idx][2];
        cards_[dealer_idx][2] = {-1, -1};
    }

    for (int i = 0; i < idx_; ++i) {
        std::cout << idx2name_[i] << ": ";

        for (int j = 0; j < cards_[i].size(); ++j) {
            if (cards_[i][j].second < 0) continue;

            if (cards_[i][j].second == 0)
                std::cout << "* ";
            else
                std::cout << cards_[i][j].second << " ";
        }

        std::cout << std::endl;
    }
}

void Display::DisplayHelp() {
    std::cout << "  SignUp [name] [password]" << std::endl;
    std::cout << "  Login [name] [password]" << std::endl;
    std::cout << "  Logout" << std::endl;
    std::cout << "  RoomList" << std::endl;
    std::cout << "  JoinRoom [room id]" << std::endl;
    std::cout << "  CreateRoom" << std::endl;
    std::cout << "  QuickMatch" << std::endl;
    std::cout << "  Ready" << std::endl;
    std::cout << "  LeaveRoom" << std::endl;
    std::cout << "  Bet [money]" << std::endl;
    std::cout << "  Hit" << std::endl;
    std::cout << "  Stand" << std::endl;
    std::cout << "  Double" << std::endl;
    std::cout << "  Surrender" << std::endl;
    std::cout << "  RankMe" << std::endl;
    std::cout << "  RankTop [number]" << std::endl;
    std::cout << "  AddFriend [name]" << std::endl;
    std::cout << "  AcceptFriend [name]" << std::endl;
    std::cout << "  DeleteFriend [name]" << std::endl;
    std::cout << "  DeleteWaitingFriend [name]" << std::endl;
    std::cout << "  FriendList" << std::endl;
    std::cout << "  WaitingFriendList" << std::endl;
    std::cout << "  MatchList" << std::endl;
    std::cout << "  MatchInfo [match id]" << std::endl;
    std::cout << "  Quit" << std::endl;
}

void Display::DisplayGameEnd(std::string& result) {
    if (result == "win") result = "WIN";
    if (result == "lose") result = "LOSE";
    if (result == "draw") result = "DRAW";
    if (result == "surrender") result = "SURRENDER";
    result = "Game over. You " + result;

    PrintPrompt(result);
}
