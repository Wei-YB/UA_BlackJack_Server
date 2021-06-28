#include "Client.h"

#include <arpa/inet.h>
#include <pthread.h>

#include <ctime>
#include <iostream>
#include <utility>

#include "Rio.h"

static Display& display = Display::DisplayInstance();

int Client::Connect(const char* host, const char* service, int type) {
    struct addrinfo hints;
    struct addrinfo* result;
    struct addrinfo* rp;
    int sfd, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = type;

    s = getaddrinfo(host, service, &hints, &result);

    if (s != 0) {
        errno = ENOSYS;
        return -1;
    }

    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }

        // connection success
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;
        }

        // connection failed
        close(sfd);
    }

    freeaddrinfo(result);

    return (rp == nullptr) ? -1 : sfd;
}

void Client::addfd(int epollfd, int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}

void Client::removefd(int epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

std::vector<std::string> Client::Parse(const std::string& command) {
    std::vector<std::string> args;
    int n = command.size(), l = 0, r = 0;
    while (l < n) {
        while (r < n && isspace(command[r])) ++r;
        l = r;
        while (r < n && !isspace(command[r])) ++r;
        if (r - l > 0) args.push_back(command.substr(l, r - l));
        l = r;
    }

    return args;
}

std::vector<std::string> Client::GetCommandArgs() {
    std::string command;
    getline(std::cin, command);

    auto args = Parse(command);
    if (args.size() < 1 || cmd2req_.find(args[0]) == cmd2req_.end()) {
        std::cout << ":( Invalid command." << std::endl;
        return {};
    }

    int input_args_num = args.size() - 1, expect_args_num = cmd2req_[args[0]].second;
    if (input_args_num != expect_args_num) {
        if (input_args_num < expect_args_num)
            std::cout << ":( Expect more args.";
        else
            std::cout << ":( Too many args.";

        return {};
    }

    return args;
}

ua_blackjack::Request Client::ConstructRequest(const std::vector<std::string>& args) {
    ua_blackjack::Request request;
    auto type = cmd2req_[args[0]].first;
    request.set_requesttype(type);
    request.set_uid(uid_);
    request.set_stamp(std::time(nullptr));
    for (int i = 1; i < args.size(); ++i) {
        request.add_args(args[i]);
    }

    return request;
}

int Client::SerializeRequest(ua_blackjack::Request& request) {
    auto data = request.SerializeAsString();

    uint32_t data_type = REQUEST;
    uint32_t data_length = data.size();

    memset(buffer_out_, '\0', sizeof(buffer_out_));
    unsigned char* bufptr = buffer_out_;

    // construct data type
    uint32_t data_type_netorder = htonl(data_type);
    unsigned char* p = (unsigned char*)&data_type_netorder;
    for (int i = 0; i < sizeof(data_type_netorder); ++i) {
        *(bufptr++) = *(p + i);
    }

    // construct data length
    uint32_t data_length_netorder = htonl(data_length);
    p = (unsigned char*)&data_length_netorder;
    for (int i = 0; i < sizeof(data_length_netorder); ++i) {
        *(bufptr++) = *(p + i);
    }

    // construct data
    snprintf((char*)bufptr, sizeof(buffer_out_) - sizeof(data_type) - sizeof(data_length), data.c_str());

    return sizeof(data_type) + sizeof(data_length) + data.size();
}

// State machine
Client::State Client::GetNextState(const std::string& cmd) {
    auto cmd_type = cmd2req_[cmd].first;

    State new_state = state_;
    bool valid = true;

    switch (cmd_type) {
        case ua_blackjack::Request::SIGNUP:
            if (state_ != OFFLINE) {
                valid = false;
            }
            break;

        case ua_blackjack::Request::LOGIN:
            if (state_ != OFFLINE) {
                valid = false;
            } else {
                new_state = ONLINE;
            }
            break;
        case ua_blackjack::Request::LOGOUT:
            if (state_ == OFFLINE) {
                valid = false;
            } else {
                new_state = OFFLINE;
            }
            break;

        case ua_blackjack::Request::ROOM_LIST:
        case ua_blackjack::Request::CREATE_ROOM:
            if (state_ != ONLINE) {
                valid = false;
            }
            break;

        case ua_blackjack::Request::JOIN_ROOM:
        case ua_blackjack::Request::QUICK_MATCH:
            if (state_ != ONLINE) {
                valid = false;
            } else {
                new_state = INROOM_UNREADY;
            }
            break;

        case ua_blackjack::Request::READY:
            if (state_ != INROOM_UNREADY) {
                valid = false;
            } else {
                new_state = INROOM_READY;
            }
            break;

        case ua_blackjack::Request::LEAVE_ROOM:
            if (state_ == OFFLINE || state_ == ONLINE) {
                valid = false;
            } else {
                new_state = ONLINE;
            }
            break;

        case ua_blackjack::Request::BET:
        case ua_blackjack::Request::HIT:
        case ua_blackjack::Request::STAND:
            if (state_ != INGAME_TURN) {
                valid = false;
            } else {
                new_state = INGAME_IDLE;
            }
            break;

        case ua_blackjack::Request::DOUBLE:
            if (state_ != INGAME_TURN && state_ != INGAME_IDLE) {
                valid = false;
            }
            break;

        case ua_blackjack::Request::SURRENDER:
            if (state_ != INGAME_IDLE && state_ != INGAME_TURN) {
                valid = false;
            } else {
                new_state = ONLINE;
            }
            break;

        case ua_blackjack::Request::INFO:
        case ua_blackjack::Request::RANK_ME:
        case ua_blackjack::Request::RANK_TOP:
        case ua_blackjack::Request::ADD_FRIEND:
        case ua_blackjack::Request::ACCEPT_FRIEND:
        case ua_blackjack::Request::DELETE_FRIEND:
        case ua_blackjack::Request::LIST_FRIEND:
        case ua_blackjack::Request::LIST_WAITTING:
            if (state_ == OFFLINE || state_ == INGAME_IDLE || state_ == INGAME_TURN) {
                valid = false;
            }
            break;
    }

    if (valid) {
        return new_state;
    } else {
        std::cout << ":( Invalid State for command" << std::endl;
        std::cout << "Current State: " << state2str_[state_] << std::endl;
        return INVALID;
    }
}

Client::State Client::GetNextState(ua_blackjack::Request& request) {
    auto& req_args = request.args();
    assert(req_args.size() > 0);

    State new_state = state_;
    std::string req = req_args[0];
    bool valid = true;

    if (req == "start") {
        if (state_ != INROOM_READY) {
            valid = false;
        } else {
            new_state = INGAME_IDLE;
        }
    } else if (req == "hit") {
        if (state_ != INGAME_IDLE) {
            valid = false;
        }
        // don't have to become INGAME_TURN
    } else if (req == "update") {
        if (state_ != INGAME_IDLE && state_ != INGAME_TURN) {
            valid = false;
        }
    } else if (req == "end") {
        if (state_ != INGAME_IDLE && state_ != INGAME_TURN) {
            valid = false;
        } else {
            new_state = INROOM_UNREADY;
        }
    } else {
        std::cout << ":( Unrecognized request from server" << std::endl;
    }

    if (valid) {
        return new_state;
    } else {
        std::cout << ":( Invalid State for request" << std::endl;
        return INVALID;
    }
}

ua_blackjack::Response Client::ConstructRoomResponse(bool valid, ua_blackjack::Request& request) {
    ua_blackjack::Response response;
    if (!valid) {
        response.set_status(-1);
        return response;
    }

    response.set_status(1);
    response.set_uid(uid_);
    response.set_stamp(request.stamp());

    std::vector<std::string> res_args;

    if (request.args()[0] == "start") {
        display.PrintPrompt("Game Start, Bet!");
        while (true) {
            std::cout << std::endl << " > " << std::flush;
            res_args = GetCommandArgs();

            if (res_args.empty()) {
                continue;

            } else {
                if (res_args[0] != "Bet") {
                    std::cout << ":( Invaild Command, Bet!" << std::endl;
                } else {
                    break;
                }
            }
        }
    } else if (request.args()[0] == "hit") {
        display.PrintPrompt("Select Hit or Stand");
        while (true) {
            std::cout << std::endl << " > " << std::flush;
            res_args = GetCommandArgs();

            if (res_args.empty()) {
                continue;

            } else {
                if (res_args[0] != "Hit" && res_args[0] != "Stand") {
                    std::cout << ":( Invaild Command, Hit or Stand!" << std::endl;
                } else {
                    break;
                }
            }
        }
    } else if (request.args()[0] == "update") {
        res_args = {"update"};
    } else if (request.args()[0] == "end") {
        res_args = {"end"};
    } else {
        std::cout << ":( Unrecognized request from server" << std::endl;
    }

    for (auto& res_arg : res_args) {
        response.add_args(res_arg);
    }

    return response;
}

int Client::SerializeRoomResponse(ua_blackjack::Response& response) {
    auto data = response.SerializeAsString();

    uint32_t data_type = RESPONSE;
    uint32_t data_length = data.size();

    memset(buffer_out_, '\0', sizeof(buffer_out_));
    unsigned char* bufptr = buffer_out_;

    // construct data type
    uint32_t data_type_netorder = htonl(data_type);
    unsigned char* p = (unsigned char*)&data_type_netorder;
    for (int i = 0; i < sizeof(data_type_netorder); ++i) {
        *(bufptr++) = *(p + i);
    }

    // construct data length
    uint32_t data_length_netorder = htonl(data_length);
    p = (unsigned char*)&data_length_netorder;
    for (int i = 0; i < sizeof(data_length_netorder); ++i) {
        *(bufptr++) = *(p + i);
    }

    // construct data
    snprintf((char*)bufptr, sizeof(buffer_out_) - sizeof(data_type) - sizeof(data_length), data.c_str());

    return sizeof(data_type) + sizeof(data_length) + data.size();
}

void Client::ProcessResponse(ua_blackjack::Response& response) {
    display.DisplayResponse(response, request_type_);
    request_type_ = ua_blackjack::Request::INVAL;

    if (response.status() == -1) return;

    if (uid_ == -1) {
        uid_ = response.uid();
    } else if (response.uid() != uid_) {
        std::cout << ":( Not my uid, wrong response" << std::endl;
    }
}

void Client::ProcessRoomRequest(ua_blackjack::Request& request) {
    Rio rio(sfd_);
    rio.RioReadInit(sfd_);

    if (request.uid() != uid_) {
        std::cout << ":( Not my uid, ignore request" << std::endl;
        return;
    }

    next_state_ = GetNextState(request);
    bool valid = (next_state_ != INVALID);

    ua_blackjack::Response room_response = ConstructRoomResponse(valid, request);
    int res_len = SerializeRoomResponse(room_response);

    // write response to proxy
    rio.RioWriten((char*)buffer_out_, res_len);

    auto& res_args = request.args();
    if (res_args.size() > 0 && res_args[0] == "update") UpdateCards(request);
    if (res_args.size() > 0 && res_args[0] == "end") GameEnd(request);
}

std::vector<int> Client::ParseCards(const std::string& str) {
    std::vector<std::string> eles = Parse(str);
    int sz = eles.size();
    std::vector<int> ret(sz);
    for (int i = 0; i < sz; ++i) {
        ret[i] = std::atoi(eles[i].c_str());
    }

    return ret;
}

void Client::UpdateCards(ua_blackjack::Request& request) {
    // update
    int num = request.args().size();
    for (int i = 1; i < num; ++i) {
        std::vector<int> parse_result = ParseCards(request.args()[i]);
        int sz = parse_result.size();
        assert(sz >= 3);
        int uid = parse_result[0];
        if (uid2idx_.find(uid) == uid2idx_.end()) {
            uid2idx_[uid] = idx_;
            idx2uid_[idx_] = uid;
            idx_++;
        }
        int idx = uid2idx_[uid];
        for (int j = 0; j < (sz - 1) / 2; ++j) {
            cards_[idx].push_back({parse_result[j * 2], parse_result[j * 2 + 1]});
        }
    }

    display.DisplayCards(idx_, idx2uid_, cards_);
}

void Client::GameEnd(ua_blackjack::Request& request) {
    std::string result = request.args()[1];
    if (result == "win") result = "WIN";
    if (result == "lose") result = "LOSE";
    result = "Game over. You " + result;

    display.PrintPrompt(result);

    // reset idx and uid2idx;
    idx_ = 0;
    uid2idx_.clear();
    idx2uid_.clear();
    for (int i = 0; i < MAX_PLAYER_NUM; ++i) {
        cards_->clear();
    }
}

void Client::GameInit() {
    std::cout << "************************************" << std::endl;
    std::cout << "*                                  *" << std::endl;
    std::cout << "* Successfully connected to server *" << std::endl;
    std::cout << "*         Welcome to 21Game        *" << std::endl;
    std::cout << "*                                  *" << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << std::endl;
}

void Client::ProcessCommand(Rio& rio) {
    std::vector<std::string> args = GetCommandArgs();
    if (args.empty()) {
        std::cout << "21Game "
                  << "(" << state2str_[state_] << ")"
                  << " > " << std::flush;
        return;
    }

    next_state_ = GetNextState(args[0]);
    if (next_state_ == INVALID) {
        std::cout << "21Game "
                  << "(" << state2str_[state_] << ")"
                  << " > " << std::flush;
        return;
    }

    ua_blackjack::Request request = ConstructRequest(args);
    request_type_ = request.requesttype();
    int req_len = SerializeRequest(request);

    // write data to proxy
    rio.RioWriten((char*)buffer_out_, req_len);
}

void Client::ProcessSocket(Rio& rio) {
    // read data type
    uint32_t data_type;
    rio.RioReadn((char*)&data_type, sizeof(data_type));
    data_type = ntohl(data_type);

    // read data length
    uint32_t data_length;
    rio.RioReadn((char*)&data_length, sizeof(data_length));
    data_length = ntohl(data_length);

    assert(data_length > 0);

    // read data
    std::string data;
    data.resize(data_length);
    char* p = &data[0];
    rio.RioReadn(p, data_length);

    if (data_type == REQUEST) {
        ua_blackjack::Request request;
        request.ParseFromString(data);
        ProcessRoomRequest(request);
    } else if (data_type == RESPONSE) {
        ua_blackjack::Response response;
        response.ParseFromString(data);
        ProcessResponse(response);
    } else {
        std::cout << "ListenProxy: Wrong data type" << std::endl;
        return;
    }

    // change state
    if (next_state_ != INVALID) state_ = next_state_;
    std::cout << "21Game "
              << "(" << state2str_[state_] << ")"
              << " > " << std::flush;
}

void Client::Run() {
    sfd_ = Connect("localhost", "50051", SOCK_STREAM);
    if (sfd_ == -1) {
        std::cout << ":( Connection failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    GameInit();

    std::cin.clear();

    std::cout << "21Game "
              << "(" << state2str_[state_] << ")"
              << " > " << std::flush;

    Rio rio(sfd_);
    rio.RioReadInit(sfd_);

    epfd_ = epoll_create(5);
    addfd(epfd_, STDIN_FILENO);
    addfd(epfd_, sfd_);

    struct epoll_event evlist[MAX_EVENTS];
    while (true) {
        int number = epoll_wait(epfd_, evlist, MAX_EVENTS, -1);
        if (number < 0 && errno != EINTR) {
            std::cout << "epoll failed" << std::endl;
            break;
        }

        for (int i = 0; i < number; ++i) {
            int fd = evlist[i].data.fd;
            if (fd == STDIN_FILENO) {
                ProcessCommand(rio);
            } else if (fd == sfd_) {
                ProcessSocket(rio);
            }
        }
    }
}

int main() {
    Client client;
    client.Run();

    return 0;
}