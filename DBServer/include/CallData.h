#pragma once
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "UA_BlackJack.grpc.pb.h"
#include "RequestParser.h"


class AsyncService;

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using ua_blackjack::Request;
using ua_blackjack::Response;
using ua_blackjack::DatabaseService;
using ua_blackjack::data_base_server::RequestParser;


class CallData {
public:
    CallData(DatabaseService::AsyncService* service, ServerCompletionQueue* cq, RequestParser& parser);

    void Proceed();

private:
    DatabaseService::AsyncService* service_;
    ServerCompletionQueue*         cq_;
    ServerContext                  ctx_;

    Request                             request_;
    Response                            response_;
    RequestParser&                      parser_;
    ServerAsyncResponseWriter<Response> responder_;

    enum CallStatus { CREATE, PROCESS, FINISH };

    CallStatus status_;
};

