#include <assert.h>
#include <grpcpp/grpcpp.h>
#include <signal.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "UA_BlackJack.grpc.pb.h"
#include "global.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
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

std::vector<int> query_cnt;
std::vector<int> fail_cnt;

int thread_num;
int total_time;

class StressClient {
public:
    explicit StressClient(std::shared_ptr<Channel> channel) : stub_(PlayerService::NewStub(channel)) {}

    // Assembles the client's payload and sends it to the server.
    void RequestPlayer(Request& request) {
        // Call object to store rpc data
        AsyncClientCall* call = new AsyncClientCall;

        call->response_reader = stub_->PrepareAsyncNotify(&call->context, request, &cq_);

        // StartCall initiates the RPC call
        call->response_reader->StartCall();

        call->response_reader->Finish(&call->reply, &call->status, (void*)call);
    }

    // Loop while listening for completed responses.
    // Prints out the response from the server.
    void AsyncCompleteRpc(int tid) {
        void* got_tag;
        bool ok = false;

        // Block until the next result is available in the completion queue "cq".
        while (cq_.Next(&got_tag, &ok)) {
            ++query_cnt[tid];

            AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);

            GPR_ASSERT(ok);
            if (call->status.ok()) {
                ++query_cnt[tid];
            } else {
                ++fail_cnt[tid];
            }
            delete call;
        }
    }

private:
    // struct for keeping state and data information
    struct AsyncClientCall {
        Response reply;

        ClientContext context;

        Status status;

        std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
    };

    std::unique_ptr<PlayerService::Stub> stub_;
    CompletionQueue cq_;
};

void handler(int sig) {
    int total_query_cnt = 0;
    int total_fail_cnt = 0;
    for (int i = 0; i < query_cnt.size(); ++i) {
        total_query_cnt += query_cnt[i];
        total_fail_cnt += fail_cnt[i];
    }

    std::cout << "total request: " << total_query_cnt << std::endl;
    std::cout << "faild request: " << total_fail_cnt << std::endl;
    std::cout << "total cost:    " << total_time * 1000 << " ms" << std::endl;
    std::cout << "QPS: " << total_query_cnt / total_time << std::endl;

    exit(0);
}

void Stress(int tid) {
    StressClient client(grpc::CreateChannel("9.135.113.138:50052", grpc::InsecureChannelCredentials()));
    std::thread thread_ = std::thread(&StressClient::AsyncCompleteRpc, &client, tid);

    Request request;
    request.set_requesttype(Request::SIGNUP);
    request.set_uid(-1);
    request.set_stamp(SteadyClock::now().time_since_epoch().count());
    while (true) {
        client.RequestPlayer(request);
    }
}

int main(int argc, char* argv[]) {
    logger = spdlog::basic_logger_mt("basic_logger", "./StressLog-asyn.log");
    logger->flush_on(spdlog::level::trace);
    logger->set_level(spdlog::level::err);

    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGALRM, &sa, nullptr) == -1) {
        logger->error("sigaction error");
        exit(EXIT_FAILURE);
    }

    assert(argc == 3);

    thread_num = atoi(argv[1]);
    total_time = atoi(argv[2]);

    query_cnt.resize(thread_num, 0);
    fail_cnt.resize(thread_num, 0);

    alarm(total_time);
    std::vector<std::thread> threads;
    for (int i = 0; i < thread_num; ++i) {
        threads.emplace_back(Stress, i);
    }

    for (int i = 0; i < thread_num; ++i) {
        threads[i].join();
    }

    return 0;
}