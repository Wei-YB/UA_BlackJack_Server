#include <assert.h>
#include <grpcpp/grpcpp.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "UA_BlackJack.grpc.pb.h"
#include "global.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// Service dependent
/*************************/
using ua_blackjack::PlayerService;
using ua_blackjack::Request;
using ua_blackjack::Response;
/*************************/

typedef std::chrono::milliseconds MilliSeconds;
typedef std::chrono::steady_clock SteadyClock;
typedef std::chrono::time_point<SteadyClock> TimePoint;

class StressClient;

std::string addr = "9.135.113.138:50052";
std::shared_ptr<spdlog::logger> logger = nullptr;
std::vector<StressClient*> clients;
int num_thread;
int num_query;

class StressClient {
public:
    StressClient(std::shared_ptr<Channel> channel) : stub_(PlayerService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    Response RequestPlayer(Request& request);
    void CheckStatus(const Status& status);

private:
    std::unique_ptr<PlayerService::Stub> stub_;
};

void StressClient::CheckStatus(const Status& status) {
    // Act upon its status.
    if (status.ok()) {
        logger->info("gRpc Got Reply");
    } else {
        logger->error("gRpc Request Error {0} : {1}", status.error_code(), status.error_message());
    }
}

Response StressClient::RequestPlayer(Request& request) {
    Response reply;

    ClientContext context;

    CheckStatus(stub_->Notify(&context, request, &reply));

    return reply;
}

void* handler(void* arg) {
    int64_t idx = (int64_t)(arg);

    Request request;
    request.set_requesttype(Request::SIGNUP);
    request.set_uid(-1);
    request.set_stamp(SteadyClock::now().time_since_epoch().count());

    for (int i = 0; i < num_query; ++i) {
        clients[idx]->RequestPlayer(request);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    logger = spdlog::basic_logger_mt("basic_logger", "./StressLog.log");
    logger->flush_on(spdlog::level::trace);
    logger->set_level(spdlog::level::err);

    assert(argc == 3);
    num_thread = atoi(argv[1]);
    num_query = atoi(argv[2]);

    for (int i = 0; i < num_thread; ++i) {
        clients.push_back(new StressClient(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials())));
    }

    std::vector<pthread_t> tids(num_thread);

    auto start = SteadyClock::now();

    for (int i = 0; i < num_thread; ++i) {
        pthread_create(&tids[i], nullptr, handler, (void*)(i));
    }

    for (int i = 0; i < num_thread; ++i) {
        pthread_join(tids[i], nullptr);
    }

    auto end = SteadyClock::now();

    auto time = std::chrono::duration_cast<MilliSeconds>(end - start).count();

    std::cout << "QPS: " << num_query * num_thread * 1000 / time << std::endl;
}