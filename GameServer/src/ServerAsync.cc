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
            if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_ITSELF)
                nomalStartGameCallback(request_, reply_);
            else if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT_START_FORWARD)
            {
                rpcForwardCallback(request_, reply_, MSG_KEY_E::MSG_KEY_GAME_START);
            }
        }

        else if (type == ua_blackjack::Request_RequestType::Request_RequestType_LEAVE_ROOM) //退出房间
        {
            if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_ITSELF || playerHashMap.count(uid) != 0)
                nomalLeaveRoomCallback(request_, reply_);
            else if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT_START_FORWARD)
            {
                rpcForwardCallback(request_, reply_, MSG_KEY_E::MSG_KEY_LEAVE_ROOM);
            }
        }
        else if (type == ua_blackjack::Request_RequestType::Request_RequestType_DOUBLE) //双倍
        {
            if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_ITSELF || playerHashMap.count(uid) != 0)
                nomalDoubleCallback(request_, reply_);
            else if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT_START_FORWARD)
            {
                rpcForwardCallback(request_, reply_, MSG_KEY_E::MSG_KEY_DOUBLE);
            }
        }
        else if (type == ua_blackjack::Request_RequestType::Request_RequestType_SURRENDER) //投降
        {
            if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_ITSELF || playerHashMap.count(uid) != 0)
                nomalSurrenderCallback(request_, reply_);
            else if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT_START_FORWARD)
            {
                rpcForwardCallback(request_, reply_, MSG_KEY_E::MSG_KEY_SURRENDER);
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