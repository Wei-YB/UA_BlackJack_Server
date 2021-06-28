#include "SocialServer-asyn-alpha.h"

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <pthread.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "SocialClientDB-syn.h"
#include "UA_BlackJack.grpc.pb.h"

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

Client* client = nullptr;

// There is no shutdown handling in this code.
void ServerImpl::Run() {
    std::string server_address("0.0.0.0:50051");

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    HandleRpcs();
}

void ServerImpl::CallData::Proceed() {
    if (status_ == CREATE) {
        status_ = PROCESS;
        service_->RequestNotify(&ctx_, &request_, &responder_, cq_, cq_, this);
    } else if (status_ == PROCESS) {
        new CallData(service_, cq_);

        // reply_ = client->RequestDB(request_);

        // And we are done! Let the gRPC runtime know we've finished
        status_ = FINISH;

        responder_.Finish(reply_, Status::OK, this);
    } else {
        GPR_ASSERT(status_ == FINISH);

        delete this;
    }
}

// This can be run in multiple threads if needed.
void ServerImpl::HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service_, cq_.get());
    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
        GPR_ASSERT(cq_->Next(&tag, &ok));
        GPR_ASSERT(ok);
        // static_cast<CallData*>(tag)->Proceed();
        CallData* calldata = static_cast<CallData*>(tag);
        if (calldata->request_.requesttype() == ua_blackjack::Request::CHAT) {
        } else {
        }
    }
}

int main(int argc, char** argv) {
    std::string addr = "9.134.69.87:50051";
    client = new Client(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials()));

    ServerImpl server;
    server.Run();

    return 0;
}
