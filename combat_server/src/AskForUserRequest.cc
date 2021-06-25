#include "AskForUserRequest.h"
#include "Player.h"
#include "GameProcess.h"
#define STAMP_ASK_BETTING 0x1
#define STAMP_ASK_HIT 0X2
#define STAMP_ASK_UPDATE 0X3
#define STAMP_ASK_END 0X4
void Client::askBettingMoney(const BlackJackUID uid)
{
#ifdef PRINT_LOG
    std::cout << "Start betting rpc request" << std::endl;
#endif
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

void Client::askHitOrStand(const BlackJackUID uid)
{
#ifdef PRINT_LOG
    std::cout << "Start ask hit" << std::endl;
#endif
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
void Client::askUpdate(const BlackJackUID uid)
{
#ifdef PRINT_LOG
    std::cout << "Start update" << std::endl;
#endif
    Request request;
    request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_NOTIFY_USER); //requestType
    request.set_uid(this->uid);
    request.set_stamp(STAMP_ASK_UPDATE);
    request.add_args("update"); //"upate"：用户牌更新，任意一个用户的牌更新都需要通知Client
    auto player = playerHashMap[uid];
    auto playerForClient = playerHashMap[this->uid];
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
        std::cout << "Player not exist!!!" << std::endl;
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
void Client::askEnd(const BlackJackUID uid, FinalResultOfGame isWin)
{
#ifdef PRINT_LOG
    std::cout << "Start End request" << std::endl;
#endif
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

void Client::AsyncCompleteRpc() //开一个线程告知协程发送的消息得到了相应
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
            this->printResponce(call->reply);             //收到信号
            if (call->reply.stamp() == STAMP_ASK_BETTING) //ask betting有响应了
            {
                auto replyuid = call->reply.uid();
                auto replyargs = call->reply.args();
                auto player = playerHashMap[uid];
                if (auto ptr = player.lock())
                {
                    int roomId = ptr->getRoom();
                    auto env = roomEnvirHashMap[roomId];
                    env->operateId = OPERATE_BETMONEY;
                    ((BetMoneyArgument *)env->arg)->uid = uid;
                    for (auto &money : replyargs)
                    {
                        if (money == "Bet")
                            continue;
                        ((BetMoneyArgument *)env->arg)->money = atoi(money.c_str());
                    }

                    myConditionSignal(env->cond);
                }
            }
            else if (call->reply.stamp() == STAMP_ASK_HIT) //ask hit有响应了
            {
                auto replyuid = call->reply.uid();
                auto replyargs = call->reply.args();
                auto player = playerHashMap[uid];
                if (auto ptr = player.lock())
                {
                    int roomId = ptr->getRoom();
                    auto env = roomEnvirHashMap[roomId];
                    ((HitArgument *)env->arg)->uid = uid;
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
                            std::cout << "HIT OR STAND ERROR" << std::endl;
                            exit(1);
                        }
                    }

                    myConditionSignal(env->cond);
                }
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
            std::cout << this->uid << call->status.error_message() << std::endl;
            this->askBettingMoney(uid); //再call一次
        }
        delete call;
    }
}