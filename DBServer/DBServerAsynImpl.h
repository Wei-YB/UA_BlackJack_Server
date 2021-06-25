#pragma once

#include <spdlog/spdlog.h>
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "UA_BlackJack.grpc.pb.h"
#include "RequestParser.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using ua_blackjack::Request;
using ua_blackjack::Response;
using ua_blackjack::DatabaseService;

class ServerAsynImpl final {
public:
    ServerAsynImpl(const std::string& rpc_host   = "0.0.0.0:50051",
                   const std::string& redis_host = "127.0.0.1:6379",
                   int                init_score = 2000);

    ~ServerAsynImpl();

    void Run();

    void Shutdown() {
        server_->Shutdown();
        cq_->Shutdown();
    }

    void HandleRpcs();

    friend class CallData;
private:
    std::string rpc_host_;
    std::string redis_host_;
    ua_black_jack_server::data_base_server::RequestParser parser_;
    std::unique_ptr<ServerCompletionQueue>                       cq_;
    DatabaseService::AsyncService                                service_;
    std::unique_ptr<Server>                                      server_;
};
