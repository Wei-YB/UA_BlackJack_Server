#pragma once

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
    explicit RPCClient(std::shared_ptr<Channel> database_channel,
                       std::shared_ptr<Channel> game_channel,
                       BlockList&               response_list);

    int GetPassword(int64_t uid);

    int GetUid(const std::string& nickname);

    void GameStart(const std::vector<int64_t>& playerList);

    int LeaveRoom(int64_t uid);

    void AsyncCompleteRpc();

    int64_t CurStamp() const { return stamp_; }

private:
    struct AsyncClientCall {
        Response                                                   reply;
        ClientContext                                              context;
        Status                                                     status;
        std::unique_ptr<grpc::ClientAsyncResponseReader<Response>> response_reader;
        int                                                        stamp;
    };

    int SetStamp(Request& request) {
        const auto stamp = NextStamp();
        request.set_stamp(stamp);
        return stamp;
    }

    int NextStamp();

    template <typename Stub>
    void AsyncCall(Stub& stub, Request&& request) {
        AsyncClientCall* call   = new AsyncClientCall;
        call->stamp             = request.stamp();
        const auto request_type = request.requesttype();
        call->response_reader   = stub->PrepareAsyncNotify(&call->context, request, &cq_);
        call->response_reader->StartCall();
        spdlog::trace("rpc async call with request type: {0}", static_cast<int>(request_type));
        call->response_reader->Finish(&call->reply, &call->status, call);
    }

    std::unique_ptr<DatabaseService::Stub> database_stub_;
    std::unique_ptr<GameService::Stub>     game_stub_;
    BlockList&                             result_list_;
    int64_t                                stamp_ = 0;
    CompletionQueue                        cq_;
};
}
