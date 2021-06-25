#include "AskForDatabaseRequest.h"
#include <chrono>
#include <sstream>
void ClientForDatebase::matchEnd(const std::list<Player::ptr> playerList)
{
#ifdef PRINT_LOG
    std::cout << "Start database match end request" << std::endl;
#endif
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()); //获取当前时间点
    std::time_t timestamp = tp.time_since_epoch().count();                                                                                                                        //计算距离1970-1-1,00:00的时间长度
    std::stringstream ss;
    ss << timestamp;
    std::string tmpStr = ss.str();
    Request request;
    request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_MATCH_END); //requestType
    request.add_args("time");                                                                  //start”：游戏开始，请求 Bet
    request.add_args(tmpStr);                                                                  //时间戳                                                                  //start”：游戏开始，请求 Bet
    for (auto &player : playerList)
    {
        ss.clear();
        ss.str(std::string());
        ss << player->uid;
        request.add_args(ss.str());

        int score = player->finalResult == WIN ? player->bettingMoney : -player->bettingMoney;
        ss.clear();
        ss.str(std::string());
        ss << score;
        request.add_args(ss.str());
    }

    // Call object to store rpc data
    AsyncClientCall *call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncNotify(&call->context, request, &cq_);

    // StartCall initiates the RPC call
    call->response_reader->StartCall();

    call->response_reader->Finish(&call->reply, &call->status, (void *)call);
}