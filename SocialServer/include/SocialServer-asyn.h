#ifndef SOCIAL_SERVER_ASYN_H
#define SOCIAL_SERVER_ASYN_H

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "SocialClientDB-syn.h"
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
using ua_blackjack::Request;
using ua_blackjack::Response;
using ua_blackjack::SocialService;
/*************************/

namespace ua_blackjack {
namespace social_server {
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
        CallData(SocialService::AsyncService* service, ServerCompletionQueue* cq)
            : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
            Proceed();
        }

        void Proceed();

    private:
        SocialService::AsyncService* service_;
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
    SocialService::AsyncService service_;
    std::unique_ptr<Server> server_;
};
}  // namespace social_server
}  // namespace ua_blackjack

#endif