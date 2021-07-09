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
int num_query;
int cnt = 0;
int fail_cnt = 0;
std::atomic<bool> timeout;

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
    void AsyncCompleteRpc() {
        void* got_tag;
        bool ok = false;

        // Block until the next result is available in the completion queue "cq".
        while (cq_.Next(&got_tag, &ok)) {
            ++cnt;
            AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);

            GPR_ASSERT(ok);

            if (!call->status.ok()) {
                ++fail_cnt;
            }

            delete call;

            if (timeout) break;
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

void handler(int sig) { timeout = true; }

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

    assert(argc == 2);
    int time = atoi(argv[1]);
    // num_query = atoi(argv[1]);

    StressClient client(grpc::CreateChannel("9.135.113.138:50052", grpc::InsecureChannelCredentials()));

    Request request;
    request.set_requesttype(Request::SIGNUP);
    request.set_uid(-1);
    request.set_stamp(SteadyClock::now().time_since_epoch().count());

    auto start = SteadyClock::now();

    std::thread thread_ = std::thread(&StressClient::AsyncCompleteRpc, &client);

    timeout = false;
    alarm(time);

    while (!timeout) {
        client.RequestPlayer(request);
    }

    // for (int i = 0; i < num_query; ++i) {
    //     client.RequestPlayer(request);
    // }

    thread_.join();

    auto end = SteadyClock::now();

    auto time_clock = std::chrono::duration_cast<MilliSeconds>(end - start).count();

    std::cout << "total request: " << cnt << std::endl;
    std::cout << "faild request: " << fail_cnt << std::endl;
    std::cout << "total cost:    " << time << " s" << std::endl;
    // std::cout << "total cost:    " << time_clock << " ms" << std::endl;
    std::cout << "QPS: " << cnt / time << std::endl;

    return 0;
}