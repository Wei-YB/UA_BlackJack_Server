#include "ClientForDatebase.h"
#include <chrono>
#include "spdlog/spdlog.h"
#include <sstream>
#define STAMP_GET_NICKNAME 123456
void ua_blackjack::Game::ClientForDatebase::matchEnd(const std::list<ua_blackjack::Game::Player::ptr> playerList)
{
#ifdef LOG_ON
    spdlog::info("Start database match end request");
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

        int score = player->finalResult == FinalResultOfGame::WIN ? player->bettingMoney : -player->bettingMoney;
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

void ua_blackjack::Game::ClientForDatebase::askPlayerNickName(const BlackJackUID uid)
{
#ifdef LOG_ON
    spdlog::info("Start database nickname request");
#endif
    //
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()); //获取当前时间点
    std::time_t timestamp = tp.time_since_epoch().count();                                                                                                                        //计算距离1970-1-1,00:00的时间长度
    std::stringstream ss;
    ss << timestamp;
    std::string tmpStr = ss.str();
    Request request;
    request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_GET_NAME); //requestType
    request.set_uid(uid);
    request.set_stamp(STAMP_GET_NICKNAME);

    // Call object to store rpc data
    AsyncClientCall *call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncNotify(&call->context, request, &cq_);

    // StartCall initiates the RPC call
    call->response_reader->StartCall();

    call->response_reader->Finish(&call->reply, &call->status, (void *)call);
}

void ua_blackjack::Game::ClientForDatebase::AsyncCompleteRpc() //开一个线程告知协程发送的消息得到了相应
{
    void *got_tag;
    bool ok = false;

    // Block until the next result is available in the completion queue "cq".
    while (cq_.Next(&got_tag, &ok))
    {
        AsyncClientCall *call = static_cast<AsyncClientCall *>(got_tag);

        GPR_ASSERT(ok);

        if (call->status.ok())
        {
            //this->printResponce(call->reply); //收到信号

            auto replyuid = call->reply.uid();
            auto replyargs = call->reply.args();

            if (call->reply.stamp() == STAMP_GET_NICKNAME)
            {
                if (replyargs.size() != 1)
                {
                    spdlog::error("get nick name argsize error uid {0:d}", replyuid);
                    exit(1);
                }
                if (auto player = playerHashMap[replyuid].lock())
                {
                    player->nickName = replyargs[0];
                }
                else
                {
                    spdlog::error("get nick name error uid {0:d}", replyuid);
                }
            }
        }
        else
        {
            spdlog::error("{1}", call->status.error_message());
            //this->askBettingMoney(uid); //再call一次
        }
        delete call;
    }
}