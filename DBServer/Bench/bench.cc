#include "UA_BlackJack.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <signal.h>
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
std::vector<int> counter;
static int continue_time = 0;
static int thread_count = 0;

class AsyncClient
{
public:
  explicit AsyncClient(std::shared_ptr<Channel> channel) : stub_(DatabaseService::NewStub(channel))
  {
    request_.set_requesttype(Request::GET_UID);
    request_.add_args("Owen");
  }
  int64_t GetUid()
  {
    AsyncClientCall *call = new AsyncClientCall;
    call->response_reader = stub_->PrepareAsyncNotify(&call->context, request_, &cq_);

    call->response_reader->StartCall();
    call->response_reader->Finish(&call->reply, &call->status, (void *)call);
  }

  void AsyncCompleteRpc(int thread_id)
  {
    void *got_tag;
    bool ok = false;
    // Block until the next result is available in the completion queue "cq".
    while (cq_.Next(&got_tag, &ok))
    {
      AsyncClientCall *call = static_cast<AsyncClientCall *>(got_tag);

      GPR_ASSERT(ok);

      if (call->status.ok())
      {
        ++counter[thread_id];
      }
      else
        ++fail_count;
      //std::cout << "RPC failed" << std::endl;
      delete call;
    }
  }

private:
  struct AsyncClientCall
  {
    Response reply;

    ClientContext context;

    Status status;

    std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
  };

  Request request_;
  std::unique_ptr<DatabaseService::Stub> stub_;
  CompletionQueue cq_;
};

void timeout(int signal)
{
  auto queries = 0;
  for (auto val : counter)
  {
    queries += val;
  }
  auto microSecond = continue_time * 1000;
  cout << "total request: " << queries << endl;
  cout << "faild request: " << fail_count << endl;
  cout << "   total cost: " << microSecond << " ms" << endl;
  cout << "          QPS: " << static_cast<int>((queries) / (microSecond / 1000.0)) << " req/s" << endl;
  exit(0);
}

void BenchFunc(int thread_id)
{
  signal(SIGALRM, timeout);
  AsyncClient client(grpc::CreateChannel("9.134.69.87:50051", grpc::InsecureChannelCredentials()));
  std::thread thread_ = std::thread(&AsyncClient::AsyncCompleteRpc, &client, thread_id);
  while (true)
  {
    client.GetUid();
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cout << "usage ./databaseBench <threads> <times>\n";
    return 0;
  }
  signal(SIGALRM, timeout);
  ::thread_count = stoi(argv[1]);
  ::continue_time = stoi(argv[2]);

  counter.resize(thread_count, 0);
  alarm(continue_time);
  vector<thread> threads;

  for (int i = 0; i < thread_count; ++i)
  {
    threads.emplace_back(BenchFunc, i);
  }
  for (auto &t : threads)
  {
    t.join();
  }
}