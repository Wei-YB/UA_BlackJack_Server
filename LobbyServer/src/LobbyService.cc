#include "LobbyService.h"

ua_blackjack::LobbyServer::LobbyServer(const std::string& game_server, const std::string& database_server):
    database_channel_(NewChannel(database_server)), game_channel_(NewChannel(game_server)),
    rpc_client_(database_channel_, game_channel_, response_list_), lobby_(rpc_client_),
    server_(request_list_) {
    CallData::SetParserFactory(new ParserFactory(lobby_, rpc_client_));
}

void ua_blackjack::LobbyServer::Run(std::string serv_address) {
    //创建新线程执行WorkThread()
    std::thread work_thread([this]() { this->WorkThread(); });
    //创建新线程执行rpc_client_的AsyncCompleteRpc：监听来自database或gameservice的response
    std::thread response_listen_thread([this]() { this->rpc_client_.AsyncCompleteRpc(); });
    //主线程执行的任务：监听来自proxy的request
    server_.Run(serv_address);
}

ua_blackjack::LobbyServer::ServerImpl::ServerImpl(BlockList<CallData*>& request_list): request_list_(request_list) {
}

void ua_blackjack::LobbyServer::ServerImpl::Run( std::string serv_address) {
    std::string   server_address(serv_address);
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    cq_     = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    SPDLOG_INFO("server listening on {}", server_address);
    HandleRpcs();
}

//主线程执行的任务 one loop：监听来自proxy的request
void ua_blackjack::LobbyServer::ServerImpl::HandleRpcs() {
    new CallData(&service_, cq_.get(), request_list_);
    void* tag = NULL;
    bool  ok;
    int constantlyBreak = -1;

    while (true) {
        //GPR_ASSERT(cq_->Next(&tag, &ok));
        // GPR_ASSERT(ok); // ASSERT 有时会failed
        cq_->Next(&tag, &ok);
        if (!ok){
            if (static_cast<CallData*>(tag)->GetCallStatus() == CallData::FINISH){
                SPDLOG_ERROR("GPR_ASSERT(ok) fail, calldata status finished.");
                if (constantlyBreak == -1)
                    constantlyBreak = 1;
            }
            else{
                SPDLOG_ERROR("GPR_ASSERT(ok) fail, calldata not finished.");
                //abort();
            }

            delete static_cast<CallData*>(tag);
            continue;
        }
        if (constantlyBreak == 1)
        {
            constantlyBreak = -1;
            SPDLOG_ERROR("server recover after the last GPR_ASSERT(ok) fail.");
        }

        static_cast<CallData*>(tag)->Proceed();
        tag = NULL;
    }
}

std::shared_ptr<grpc::Channel> ua_blackjack::LobbyServer::NewChannel(const std::string& host) {
    return CreateChannel(host, grpc::InsecureChannelCredentials());
}

//工作线程用于epoll的eventloop：监听两个事件。
void ua_blackjack::LobbyServer::WorkThread() {
    SPDLOG_INFO("event loop start");
    auto epoll_fd = epoll_create(1);
    if (epoll_fd < 0) {
        SPDLOG_ERROR("create epoll fd error: ", strerror(errno));
    }

    epoll_event                          events[2] = {request_list_.GetEpollEvent(), response_list_.GetEpollEvent()};
    std::map<int, std::function<void()>> handler_map;
    handler_map[request_list_.fd()]  = [this, epoll_fd]() { this->HandleRequest(epoll_fd); };
    handler_map[response_list_.fd()] = [this, epoll_fd]() { this->HandleResponse(epoll_fd); };

    //将两个list中的eventfd添加到epoll监控里
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, request_list_.fd(), events) < 0) {
        SPDLOG_ERROR("error in add request list: ", strerror(errno));
    }
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, response_list_.fd(), events + 1) < 0) {
        SPDLOG_ERROR("error in add response list: ", strerror(errno));
    }

    while (true) {
        SPDLOG_TRACE("start next loop");
        auto ret = epoll_wait(epoll_fd, events, 2, -1);
        if (ret < 0) {
            SPDLOG_ERROR("epoll wait return error: {0}", strerror(errno));
        }
        else
            SPDLOG_TRACE("{0} event active", ret);

        for (int i = 0; i < ret; ++i) {
            handler_map[events[i].data.fd]();
        }
    }
}

//新的rpc request到达时，激活CallData的proceed()，proceed()处理一部分工作后将自身(calldata)PushBack到request_list_
//PushBack激活工作线程的epoll，调用HandleRequest()再次进入该calldata的proceed()
void ua_blackjack::LobbyServer::HandleRequest(int epfd) {
    SPDLOG_TRACE("request list active");
    uint64_t val;
    read(request_list_.fd(), &val, sizeof val);

    auto top = request_list_.PopFront();
    //调用保存此request的calldata的Proceed方法，此时正确的情况是直接进入calldata里的status == PROCESS
    top->Proceed();
    const auto stage = top->GetCallStatus();
    if (stage != CallData::FINISH) {
        rpc_info_[top->GetStamp()] = top;
    }
    //Addfd(epfd, request_list_.fd(), request_list_.GetEpollEvent());
}

void ua_blackjack::LobbyServer::HandleResponse(int epfd) {
    SPDLOG_TRACE("response list active");
    uint64_t val;
    read(response_list_.fd(), &val, sizeof val);

    auto response = response_list_.PopFront();
    auto stamp    = response.stamp();
    SPDLOG_TRACE("get response with stamp {0}", stamp);
    if (rpc_info_.count(stamp)) {
        auto call_data = rpc_info_[stamp];
        call_data->Proceed(&response);
        auto stage = call_data->GetCallStatus();
        if (stage != CallData::FINISH) {
            auto new_stamp = call_data->GetStamp();
            SPDLOG_TRACE("call data with another rpc call stamp {0}", new_stamp);
            rpc_info_[new_stamp] = call_data;
        }
        rpc_info_.erase(stamp);
    }
    else {
        SPDLOG_TRACE("useless stamp, no need to process");
    }
    //Addfd(epfd, response_list_.fd(), response_list_.GetEpollEvent());
}


void ua_blackjack::LobbyServer::Addfd(int epfd, int fd, epoll_event events){
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &events) < 0) {
        SPDLOG_ERROR("error in add request list: ", strerror(errno));
    }
    

}
