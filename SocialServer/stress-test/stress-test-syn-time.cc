#include <assert.h>
#include <grpcpp/grpcpp.h>
#include <signal.h>
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
using ua_blackjack::Request;
using ua_blackjack::Response;
using ua_blackjack::SocialService;
/*************************/

typedef std::chrono::milliseconds MilliSeconds;
typedef std::chrono::steady_clock SteadyClock;
typedef std::chrono::time_point<SteadyClock> TimePoint;

class StressClient;

std::string addr = "9.135.113.138:50051";
std::shared_ptr<spdlog::logger> logger = nullptr;
std::vector<StressClient*> clients;
int num_thread;
int num_time;

std::vector<int> query_cnt;
std::vector<int> fail_cnt;

class StressClient {
public:
    StressClient(std::shared_ptr<Channel> channel) : stub_(SocialService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    Response RequestSocial(Request& request, int idx);
    bool CheckStatus(const Status& status);

private:
    std::unique_ptr<SocialService::Stub> stub_;
};

bool StressClient::CheckStatus(const Status& status) {
    // Act upon its status.
    if (status.ok()) {
        // logger->info("gRpc Got Reply");
        return true;
    } else {
        // logger->error("gRpc Request Error {0} : {1}", status.error_code(), status.error_message());
        return false;
    }
}

Response StressClient::RequestSocial(Request& request, int idx) {
    Response reply;

    ClientContext context;

    if (CheckStatus(stub_->Notify(&context, request, &reply))) {
        query_cnt[idx]++;
    } else {
        fail_cnt[idx]++;
    }

    return reply;
}

void* handler(void* arg) {
    int64_t idx = (int64_t)(arg);

    Request request;
    request.set_requesttype(Request::SIGNUP);
    request.set_uid(-1);
    request.set_stamp(SteadyClock::now().time_since_epoch().count());

    while (true) {
        clients[idx]->RequestSocial(request, idx);
    }
}

void timeout(int sig) {
    int total_query_cnt = 0;
    int total_fail_cnt = 0;
    for (int i = 0; i < query_cnt.size(); ++i) {
        total_query_cnt += query_cnt[i];
        total_fail_cnt += fail_cnt[i];
    }

    std::cout << "total request: " << total_query_cnt << std::endl;
    std::cout << "faild request: " << total_fail_cnt << std::endl;
    std::cout << "total cost:    " << num_time * 1000 << " ms" << std::endl;
    std::cout << "QPS: " << total_query_cnt / num_time << std::endl;

    exit(0);
}

int main(int argc, char* argv[]) {
    logger = spdlog::basic_logger_mt("basic_logger", "./StressLog.log");
    logger->flush_on(spdlog::level::trace);
    logger->set_level(spdlog::level::err);

    assert(argc == 3);
    num_thread = atoi(argv[1]);
    num_time = atoi(argv[2]);

    for (int i = 0; i < num_thread; ++i) {
        clients.push_back(new StressClient(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials())));
    }

    std::vector<pthread_t> tids(num_thread);
    query_cnt.resize(num_thread, 0);
    fail_cnt.resize(num_thread, 0);

    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = timeout;
    if (sigaction(SIGALRM, &sa, nullptr) == -1) {
        logger->error("sigaction error");
        exit(EXIT_FAILURE);
    }

    alarm(num_time);

    for (int i = 0; i < num_thread; ++i) {
        pthread_create(&tids[i], nullptr, handler, (void*)(i));
    }

    for (int i = 0; i < num_thread; ++i) {
        pthread_join(tids[i], nullptr);
    }
}