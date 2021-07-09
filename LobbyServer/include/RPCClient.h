#pragma once

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <grpcpp/grpcpp.h>
#include "UA_BlackJack.grpc.pb.h"

#include "BlockList.hpp"

namespace ua_blackjack {

class RPCClient {
    using Channel = grpc::Channel;
    using Server = grpc::Server;
    using Status = grpc::Status;
    using ClientContext = grpc::ClientContext;
    using CompletionQueue = grpc::CompletionQueue;
    using DatabaseService = ua_blackjack::DatabaseService;
    using GameService = ua_blackjack::GameService;
    using Request = ua_blackjack::Request;
    using Response = ua_blackjack::Response;
    using BlockList = ua_blackjack::BlockList<Response>;

public:
    //所请求的不同服务对应的channel
    explicit RPCClient(std::shared_ptr<Channel> database_channel,
                       std::shared_ptr<Channel> game_channel,
                       BlockList&               response_list);

    int GetPassword(int64_t uid);

    int GetUid(const std::string& nickname);

    void GameStart(const std::vector<int64_t>& playerList);

    int LeaveRoom(int64_t uid);

    int RestartGameServer();

    int64_t CurStamp() const { return stamp_; }

    //监听来自服务端的响应，通常开启一个新线程执行
    void AsyncCompleteRpc();

private:
    //用于存储rpc服务端返回的响应
    struct AsyncClientCall {
        Response                                                   reply;
        ClientContext                                              context;
        Status                                                     status;
        std::unique_ptr<grpc::ClientAsyncResponseReader<Response>> response_reader;
        int                                                        stamp;
    };

    //stamp取自增计数器的值
    int SetStamp(Request& request) {
        const auto stamp = NextStamp();
        request.set_stamp(stamp);
        return stamp;
    }

    int NextStamp();

    //向stub指定的服务发起请求，请求字段放置在AsyncClientCall对象call->request中。
    template <typename Stub>
    void AsyncCall(Stub& stub, Request&& request) {
        AsyncClientCall* call   = new AsyncClientCall;
        call->stamp             = request.stamp();
        const auto request_type = request.requesttype();
        call->response_reader   = stub->PrepareAsyncNotify(&call->context, request, &cq_);
        call->response_reader->StartCall();
        SPDLOG_TRACE("rpc async call with request type: {0}", Request::RequestType_Name(request_type));
        call->response_reader->Finish(&call->reply, &call->status, call);
    }

    //所请求的不同服务对应的stub_
    std::unique_ptr<DatabaseService::Stub> database_stub_;
    std::unique_ptr<GameService::Stub>     game_stub_;
    //阻塞队列里存放的是后端数据库或者游戏对局服务返回的应答response
    BlockList&                             result_list_;
    int64_t                                stamp_ = 0;
    CompletionQueue                        cq_;
};
}
