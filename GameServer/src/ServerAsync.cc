#include "ServerAsync.h"
#include "ControlTcpServer.h"
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
        if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_ITSELF)
        {
            handelRpcRequest(request_, reply_, false);
        }
        else if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT_START_FORWARD)
        {
            handelRpcRequest(request_, reply_, true);
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

void handelRpcRequest(ua_blackjack::Request &request, ua_blackjack::Response &responce, bool needForward)
{
    auto type = request.requesttype();
    auto uid = request.uid();
    auto stamp = request.stamp();
    auto args = request.args();
    responce.set_status(-1);
    responce.set_uid(uid);
    responce.set_stamp(stamp);
    spdlog::info("type = {0:d} uid = {1:d} stamp = {2:d}", type, uid, stamp);
    if (type == ua_blackjack::Request_RequestType::Request_RequestType_GAME_START) //真正的创建房间code
    {
        if (needForward == false)
            nomalStartGameCallback(request, responce);
        else if (needForward == true)
        {
            rpcForwardCallback(request, responce, MSG_KEY_E::MSG_KEY_GAME_START);
        }
    }
    else if (type == ua_blackjack::Request_RequestType::Request_RequestType_LEAVE_ROOM) //退出房间
    {
        if (playerHashMap.count(uid) != 0)
            nomalLeaveRoomCallback(request, responce);
        else if (needForward == true)
        {
            rpcForwardCallback(request, responce, MSG_KEY_E::MSG_KEY_LEAVE_ROOM);
        }
    }
    else if (type == ua_blackjack::Request_RequestType::Request_RequestType_DOUBLE) //双倍
    {
        if (playerHashMap.count(uid) != 0)
            nomalDoubleCallback(request, responce);
        else if (needForward == true)
        {
            rpcForwardCallback(request, responce, MSG_KEY_E::MSG_KEY_DOUBLE);
        }
    }
    else if (type == ua_blackjack::Request_RequestType::Request_RequestType_SURRENDER) //投降
    {
        if (playerHashMap.count(uid) != 0)
            nomalSurrenderCallback(request, responce);
        else if (needForward == true)
        {
            rpcForwardCallback(request, responce, MSG_KEY_E::MSG_KEY_SURRENDER);
        }
    }
    else if (type == ua_blackjack::Request_RequestType::Request_RequestType_LOGOUT) //重启
    {
        int pid = fork();
        if (pid == 0)
        {
            execl("/home/tong/UA_BlackJack_Server/build/game/GameService",
                  "GameService",
                  "RELEASE", "-lf",
                  "/home/tong/UA_BlackJack_Server/build/game/logs/async_log.log ", "-cf",
                  "/home/tong/UA_BlackJack_Server/GameServer/game.config",
                  (char *)NULL);
        }
        else
        {
            exit(0);
        }
    }
    else
    {
        responce.set_status(-1);
        spdlog::error("unexpected command uid {0:d}  ", uid);
    }
}