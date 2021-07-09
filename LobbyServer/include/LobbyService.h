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

    void Run(std::string serv_address);


private:
    //大厅模块为proxy提供服务的类
    class ServerImpl final {
    public:
        ServerImpl(BlockList<CallData*>& request_list);

        void Run(std::string serv_address);

        void HandleRpcs();

    private:
        BlockList<CallData*>&                  request_list_;
        std::unique_ptr<ServerCompletionQueue> cq_;
        LobbyService::AsyncService             service_;
        std::unique_ptr<Server>                server_;
    };

    static std::shared_ptr<Channel> NewChannel(const std::string& host);

    void WorkThread();

    void HandleRequest(int epfd);

    void HandleResponse(int epfd);

    void Addfd(int epfd, int fd, epoll_event events);

    //分别向两个服务发起请求的两个channel
    std::shared_ptr<Channel> database_channel_;
    std::shared_ptr<Channel> game_channel_;

    //存放来自proxy的请求
    BlockList<CallData*>                   request_list_;
    //存放来自DBSerice、gameService的响应
    BlockList<Response>                    response_list_;
    std::unordered_map<int32_t, CallData*> rpc_info_;

    //三个成员变量用于分解lobby模块的功能
    RPCClient  rpc_client_;
    Lobby      lobby_;
    ServerImpl server_;
};
}
