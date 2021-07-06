#include "ServerAsync.h"
void ua_blackjack::Game::ServerImpl::Run()
{
    ServerBuilder builder;
    builder.AddListeningPort(gameRpcAddr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    spdlog::info("Server listening on {0:d}", gameRpcAddr);
}
void ua_blackjack::Game::ServerImpl::HandleRpcs()
{
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service_, cq_.get());
    void *tag; // uniquely identifies a request.
    bool ok;
    while (true)
    {

        GPR_ASSERT(cq_->Next(&tag, &ok));
        GPR_ASSERT(ok);

        static_cast<CallData *>(tag)->Proceed();
    }
}
void ua_blackjack::Game::CallData::Proceed()
{
    if (status_ == CREATE)
    {
        status_ = PROCESS;
        service_->RequestNotify(&ctx_, &request_, &responder_, cq_, cq_, this);
    }
    else if (status_ == PROCESS)
    {
        new CallData(service_, cq_);
        auto type = request_.requesttype();
        auto uid = request_.uid();
        auto stamp = request_.stamp();
        auto args = request_.args();
        reply_.set_status(-1);
        reply_.set_uid(uid);
        reply_.set_stamp(stamp);
        spdlog::info("type = {0:d} uid = {1:d} stamp = {2:d}", type, uid, stamp);
        if (type == ua_blackjack::Request_RequestType::Request_RequestType_GAME_START) //真正的创建房间code
        {
            nomalStartGameCallback(request_, reply_);
        }

        else if (type == ua_blackjack::Request_RequestType::Request_RequestType_LEAVE_ROOM) //退出房间
        {
            reply_.set_status(0);
            if (auto playerPtr = playerHashMap[uid].lock())
            {
                spdlog::info("{0:d} quit by itself", uid);
                playerPtr->quit(); //托管

                int roomId = playerPtr->getRoom();
                auto env = roomEnvirHashMap[roomId];
                { //加锁
                    std::lock_guard<std::mutex> lock(env->mutex);
                    if (playerPtr->isFinishBetting == false) //在托管阶段就quit了
                    {
                        auto room = roomHashMap[roomId];
                        env->sizeOfCompleteBetting++;
                        if (env->sizeOfCompleteBetting >= room.lock()->playerList.size())
                        {
                            myConditionSignal(env->cond); //所有玩家都收到筹码了
                        }
                    }
                    if (playerPtr->isWaitingReply == true)
                    {
                        myConditionSignal(env->cond);
                    }
                }
            }
            else
            {
                spdlog::error("Quit error uid {0:d}   not existed in any room", uid);
            }
        }
        else if (type == ua_blackjack::Request_RequestType::Request_RequestType_DOUBLE) //双倍
        {

            if (auto playerPtr = playerHashMap[uid].lock())
            {
                reply_.set_status(0);
                spdlog::info("{0:d} double by itself", uid);
                playerPtr->bettingMoney *= 2;
            }
            else
            {
                reply_.set_status(-1);
                spdlog::error("Double error uid {0:d}   not existed in any room", uid);
            }
        }
        else if (type == ua_blackjack::Request_RequestType::Request_RequestType_SURRENDER) //投降
        {

            if (auto playerPtr = playerHashMap[uid].lock())
            {
                reply_.set_status(0);
                spdlog::info("{0:d} surrond by itself", uid);
                playerPtr->bettingMoney /= 2;
                playerPtr->isStand = true;
                playerPtr->finalResult = FinalResultOfGame::LOSE;
                if (playerPtr->isWaitingReply == true)
                {
                    int roomId = playerPtr->getRoom();
                    auto env = roomEnvirHashMap[roomId];
                    { //加锁
                        std::lock_guard<std::mutex> lock(env->mutex);
                        myConditionSignal(env->cond);
                    }
                }
            }
            else
            {
                reply_.set_status(-1);
                spdlog::error("Surrond error uid {0:d}   not existed in any room", uid);
            }
        }
        else
        {
            reply_.set_status(-1);
            spdlog::error("unexpected command uid {0:d}  ", uid);
        }

        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this); //运行这行，调用方就收到response
    }
    else
    {

        GPR_ASSERT(status_ == FINISH);

        delete this;
    }
}