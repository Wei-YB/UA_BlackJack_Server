#include <grpcpp/grpcpp.h>

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#include "UA_BlackJack.grpc.pb.h"
using namespace std;

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

using ua_blackjack::DatabaseService;
using ua_blackjack::Request;
using ua_blackjack::Response;

atomic<int> fail_count;

class DatabaseClient {
public:
    DatabaseClient(std::shared_ptr<Channel> channel) : stub_(DatabaseService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    int64_t GetUid(const std::string& name) {
        Request request;
        request.set_requesttype(Request::GET_UID);
        request.add_args(name);

        Response reply;

        ClientContext context;

        Status status = stub_->Notify(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.uid();
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return -1;
        }
    }

private:
    std::unique_ptr<DatabaseService::Stub> stub_;
};

void stressFunc(int count) {
    DatabaseClient client(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));
    for (int i = 0; i < count; ++i) {
        // if(i % 100 == 0)
        // cout <<"called "<< i <<" times"<< endl;
        auto ret = client.GetUid("Owen");
        if (ret < 0) {
            ++fail_count;
        }
    }
}

class AsyncClient {
public:
    explicit AsyncClient(std::shared_ptr<Channel> channel) : stub_(DatabaseService::NewStub(channel)) {
        request_.set_requesttype(Request::GET_UID);
        request_.add_args("Owen");
    }
    int64_t GetUid() {
        AsyncClientCall* call = new AsyncClientCall;
        call->response_reader = stub_->PrepareAsyncNotify(&call->context, request_, &cq_);

        call->response_reader->StartCall();
        call->response_reader->Finish(&call->reply, &call->status, (void*)call);
    }

    void AsyncCompleteRpc(int count) {
        void* got_tag;
        bool ok = false;
        int acc = 1;
        // Block until the next result is available in the completion queue "cq".
        while (cq_.Next(&got_tag, &ok)) {
            ++acc;
            if (acc == count) break;
            AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);

            GPR_ASSERT(ok);

            if (call->status.ok()) {
            } else
                ++fail_count;
            // std::cout << "RPC failed" << std::endl;
            delete call;
        }
    }

private:
    struct AsyncClientCall {
        Response reply;

        ClientContext context;

        Status status;

        std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
    };

    Request request_;
    std::unique_ptr<DatabaseService::Stub> stub_;
    CompletionQueue cq_;
};

int main() {
    int thread_count = 1;
    int call_count = 100000;
    AsyncClient client(grpc::CreateChannel("9.134.69.87:50051", grpc::InsecureChannelCredentials()));
    std::thread thread_ = std::thread(&AsyncClient::AsyncCompleteRpc, &client, call_count);

    auto start = chrono::system_clock::now();
    for (int i = 0; i < call_count; ++i) client.GetUid();
    thread_.join();
    auto end = chrono::system_clock::now();

    // DatabaseClient client(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));
    // auto ret = client.GetUid("Owen");
    // cout <<"get ret with "<< ret << endl;
    auto diff = end - start;
    auto microSecond = chrono::duration_cast<chrono::milliseconds>(diff);
    cout << "total request: " << thread_count * call_count << endl;
    cout << "faild request: " << fail_count << endl;
    cout << "total cost:    " << microSecond.count() << " ms" << endl;
    cout << "QPS:           " << static_cast<int>((thread_count * call_count) / (microSecond.count() / 1000.0))
         << " req/s" << endl;
}