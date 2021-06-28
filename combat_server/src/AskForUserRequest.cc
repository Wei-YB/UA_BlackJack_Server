#include "AskForUserRequest.h"
#include "Player.h"
#include "GameProcess.h"
#include "spdlog/spdlog.h"
#include <sstream>
#define STAMP_ASK_BETTING 0x1
#define STAMP_ASK_HIT 0X2
#define STAMP_ASK_UPDATE 0X3
#define STAMP_ASK_END 0X4
void ClientForTestUser::askBettingMoney(const BlackJackUID uid)
{

    spdlog::info("Start betting rpc request");
    Request request;
    request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_NOTIFY_USER); //requestType
    request.set_uid(uid);
    request.set_stamp(STAMP_ASK_BETTING);
    request.add_args("start"); //start”：游戏开始，请求 Bet
    // Call object to store rpc data
    AsyncClientCall *call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncNotify(&call->context, request, &cq_);

    // StartCall initiates the RPC call
    call->response_reader->StartCall();

    call->response_reader->Finish(&call->reply, &call->status, (void *)call);
}

void ClientForTestUser::askHitOrStand(const BlackJackUID uid)
{
    spdlog::info("Start ask hit");
    Request request;
    request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_NOTIFY_USER); //requestType
    request.set_uid(uid);
    request.set_stamp(STAMP_ASK_HIT);
    request.add_args("hit"); //"hit"：游戏中，请求拿牌或者停牌

    // Call object to store rpc data
    AsyncClientCall *call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncNotify(&call->context, request, &cq_);

    // StartCall initiates the RPC call
    call->response_reader->StartCall();

    call->response_reader->Finish(&call->reply, &call->status, (void *)call);
}
void ClientForTestUser::askUpdate(const BlackJackUID uid, const BlackJackUID notifyUser) //第一个uid是哪个用户的信息更新了，第二个参数是发送给哪个用户
{
    spdlog::info("Start update");

    Request request;
    request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_NOTIFY_USER); //requestType
    request.set_uid(notifyUser);
    request.set_stamp(STAMP_ASK_UPDATE);
    request.add_args("update"); //"upate"：用户牌更新，任意一个用户的牌更新都需要通知Client
    auto player = playerHashMap[uid];
    auto playerForClient = playerHashMap[notifyUser];
    if (auto ptr = player.lock())
    {
        std::stringstream tmp;
        tmp << ptr->uid;
        std::string updateArg = tmp.str();
        for (auto &poker : ptr->pokerList)
        {
            if (auto ptrForThisClient = playerForClient.lock())
            {
                if (poker->isHide() == true && ptrForThisClient->isDealer == false) //普通玩家是看不到隐藏牌的
                {
                    updateArg += " 0 0";
                    continue;
                }
            }
            else
            {
                spdlog::error("Player not exist!!! {0}", notifyUser);
                exit(1);
            }
            int color = poker->getValue() / 13 + 1;
            updateArg += " ";
            tmp.clear();
            tmp.str(std::string());
            tmp << color;
            updateArg += tmp.str();
            int num = poker->getValue() % 13 + 1;
            updateArg += " ";
            tmp.clear();
            tmp.str(std::string());
            tmp << num;
            updateArg += tmp.str();
        }
        request.add_args(updateArg);
    }
    else
    {
        spdlog::error("Player not exist!!! {0}", uid);
        exit(1);
    }
    // Call object to store rpc data
    AsyncClientCall *call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncNotify(&call->context, request, &cq_);

    // StartCall initiates the RPC call
    call->response_reader->StartCall();

    call->response_reader->Finish(&call->reply, &call->status, (void *)call);
}
void ClientForTestUser::askEnd(const BlackJackUID uid, FinalResultOfGame isWin)
{

    spdlog::info("Start End request");

    Request request;
    request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_NOTIFY_USER); //requestType
    request.set_uid(uid);
    request.set_stamp(STAMP_ASK_END);
    request.add_args("end"); //start”：游戏开始，请求 Bet

    switch (isWin)
    {
    case FinalResultOfGame::WIN:
        request.add_args("win"); //start”：游戏开始，请求 Bet
        break;
    case FinalResultOfGame::LOSE:
        request.add_args("lose"); //start”：游戏开始，请求 Bet
        break;
    case FinalResultOfGame::DRAW:
        request.add_args("draw"); //start”：游戏开始，请求 Bet
        break;
    }

    // Call object to store rpc data
    AsyncClientCall *call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncNotify(&call->context, request, &cq_);

    // StartCall initiates the RPC call
    call->response_reader->StartCall();

    call->response_reader->Finish(&call->reply, &call->status, (void *)call);
}

void ClientForTestUser::AsyncCompleteRpc() //开一个线程告知协程发送的消息得到了相应
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
            this->printResponce(call->reply); //收到信号

            auto replyuid = call->reply.uid();
            auto replyargs = call->reply.args();
            auto player = playerHashMap[replyuid];
            if (auto ptr = player.lock())
            {

                if (call->reply.stamp() == STAMP_ASK_BETTING) //ask betting有响应了
                {
                    if (ptr->isQuit == true) //已退出玩家的相应不处理
                    {
                        spdlog::warn("uid {0:d}user quit but reply later", replyuid);
                        delete call;
                        return;
                    }
                    int roomId = ptr->getRoom();
                    auto env = roomEnvirHashMap[roomId];
                    env->operateId = OPERATE_BETMONEY;
                    ((BetMoneyArgument *)env->arg)->uid = replyuid;
                    for (auto &money : replyargs)
                    {
                        if (money == "Bet")
                            continue;
                        ((BetMoneyArgument *)env->arg)->money = atoi(money.c_str());
                    }

                    myConditionSignal(env->cond);
                }
                else if (call->reply.stamp() == STAMP_ASK_HIT) //ask hit有响应了
                {
                    if (ptr->isQuit == true) //已退出玩家的相应不处理
                    {
                        spdlog::warn("uid {0:d}user quit but reply later", replyuid);
                        delete call;
                        return;
                    }

                    int roomId = ptr->getRoom();
                    auto env = roomEnvirHashMap[roomId];
                    ((HitArgument *)env->arg)->uid = replyuid;
                    for (auto &s : replyargs)
                    {
                        if (s == "Hit") // hit
                        {
                            env->operateId = OPERATE_HIT;
                        }
                        else if (s == "Stand")
                        {
                            env->operateId = OPERATE_STAND;
                        }
                        else
                        {
                            spdlog::error("{0} HIT OR STAND ERROR", call->reply.uid());
                            exit(1);
                        }
                    }

                    myConditionSignal(env->cond);
                }
                else if (call->reply.stamp() == STAMP_ASK_UPDATE) //ask update有响应了
                {
                    /*
                不检客户端有没有收到update
                
                */
                }
                else if (call->reply.stamp() == STAMP_ASK_END)
                {
                    /*
                不检客户端有没有收到end
                
                */
                }
            }
            else
            {
                spdlog::warn("unexpected reply uid {0:d}", replyuid);
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