#pragma once

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <string>
#include "BlockList.hpp"
#include "CallData.h"

#include <spdlog/spdlog.h>

namespace ua_blackjack {

using grpc::Server;
using grpc::ServerBuilder;
using grpc::Channel;

class LobbyServer {

public:
    LobbyServer(const std::string& game_server, const std::string& database_server);

    void Run();


private:
    class ServerImpl final {
    public:
        ServerImpl(BlockList<CallData*>& request_list);

        void Run();

        void HandleRpcs();

    private:
        BlockList<CallData*>&                  request_list_;
        std::unique_ptr<ServerCompletionQueue> cq_;
        LobbyService::AsyncService             service_;
        std::unique_ptr<Server>                server_;
    };

    static std::shared_ptr<Channel> NewChannel(const std::string& host);

    void WorkThread();


    void HandleRequest();

    void HandleResponse();

    //two channel for two rpcclient call
    std::shared_ptr<Channel> database_channel_;
    std::shared_ptr<Channel> game_channel_;

    //存放来自proxy的请求
    BlockList<CallData*>                   request_list_;
    //存放来自DB的响应
    BlockList<Response>                    response_list_;
    std::unordered_map<int32_t, CallData*> rpc_info_;

    RPCClient  rpc_client_;
    Lobby      lobby_;
    ServerImpl server_;
};
}
