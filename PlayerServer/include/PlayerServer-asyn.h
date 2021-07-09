#ifndef PLAYER_SERVER_ASYN_H
#define PLAYER_SERVER_ASYN_H

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "PlayerClientDB-syn.h"
#include "UA_BlackJack.grpc.pb.h"
#include "global.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

// Service dependent
/*************************/
using ua_blackjack::PlayerService;
using ua_blackjack::Request;
using ua_blackjack::Response;
/*************************/

namespace ua_blackjack {
namespace player_server {
class ServerImpl final {
public:
    ~ServerImpl() {
        server_->Shutdown();
        // Always shutdown the completion queue after the server.
        cq_->Shutdown();
    }

    // There is no shutdown handling in this code.
    void Run(std::string& DBServer_ip, std::string& DBServer_port, std::string& listen_port);

private:
    // Class encompasing the state and logic needed to serve a request.
    class CallData {
    public:
        CallData(PlayerService::AsyncService* service, ServerCompletionQueue* cq)
            : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
            Proceed();
        }

        void Proceed();

    private:
        PlayerService::AsyncService* service_;
        ServerCompletionQueue* cq_;
        ServerContext ctx_;

        Request request_;
        Response reply_;

        // The means to get back to the client.
        ServerAsyncResponseWriter<Response> responder_;

        enum CallStatus { CREATE, PROCESS, FINISH };
        CallStatus status_;  // The current serving state.
    };

    // This can be run in multiple threads if needed.
    void HandleRpcs();

    std::unique_ptr<ServerCompletionQueue> cq_;
    PlayerService::AsyncService service_;
    std::unique_ptr<Server> server_;
};
}  // namespace player_server
}  // namespace ua_blackjack

#endif