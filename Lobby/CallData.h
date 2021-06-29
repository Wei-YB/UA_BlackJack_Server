#pragma once

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif


#include "Parser.h"
#include "Lobby.h"
#include "UA_BlackJack.grpc.pb.h"
#include "BlockList.hpp"


using grpc::Status;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::ServerAsyncResponseWriter;


using ua_blackjack::LobbyService;
using ua_blackjack::Response;
using ua_blackjack::Request;
using ua_blackjack::BlockList;

using ua_blackjack::lobby::Lobby;
using ua_blackjack::lobby::Parser;
using ua_blackjack::lobby::ParserFactory;


namespace spd = spdlog;

class CallData {
public:
    enum CallStatus { CREATE, NEW_REQUEST, PROCESS, FINISH };

    CallData(LobbyService::AsyncService* service,
             ServerCompletionQueue*      cq,
             BlockList<CallData*>&       request_list);

    CallData(const CallData&) = delete;
    CallData(CallData&&)      = delete;

    CallData& operator=(const CallData&) = delete;
    CallData& operator=(CallData&&)      = delete;

    ~CallData() = default;

    void Proceed(Response* rpc_result = nullptr);

    int64_t GetStamp() const;

    CallStatus GetCallStatus() const;

    static void SetParserFactory(ParserFactory* parser_factory);

private:
    void ProceedCreate();

    void ProceedNewRequest();

    void ProceedProcess(Response* rpc_result);


    static ParserFactory* parser_factory_;

    LobbyService::AsyncService* service_;
    ServerCompletionQueue*      cq_;
    ServerContext               ctx_;


    std::shared_ptr<Parser> parser_;
    Request                 request_;
    Response                reply_;

    BlockList<CallData*>& request_list_;
    // The means to get back to the client.
    ServerAsyncResponseWriter<Response> responder_;

    CallStatus status_; // The current serving state.
};
