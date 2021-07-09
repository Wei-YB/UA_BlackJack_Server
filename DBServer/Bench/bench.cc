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
static int client_count = 0;

class StressClient {
public:
    StressClient(std::shared_ptr<Channel> channel) : stub_(DatabaseService::NewStub(channel)) {
      request_.add_args("Owen");
      request_.set_requesttype(Request::GET_UID);
      request_.set_stamp(0);
    }


    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    bool getUID(){
      Response reply;
      ClientContext context;
      return stub_->Notify(&context, request_,&reply).ok();
    }
private:
    Request request_;
    std::unique_ptr<DatabaseService::Stub> stub_;
};


void timeout(int signal)
{
  auto queries = 0;
  for (auto val : counter)
  {
    queries += val;
  }
  auto microSecond = continue_time * 1000;
  cout <<  "total client: " << client_count << endl;
  cout << "total request: " << queries << endl;
  cout << "faild request: " << fail_count << endl;
  cout << "   total cost: " << microSecond << " ms" << endl;
  cout << "          QPS: " << static_cast<int>((queries) / (microSecond / 1000.0)) << " req/s" << endl;
  exit(0);
}

void BenchFunc(int clients,int thread_id)
{
  vector<StressClient> vec_client;
  for(int i = 0;i < clients; ++i){
    vec_client.emplace_back(grpc::CreateChannel("9.134.69.87:50051", grpc::InsecureChannelCredentials()));
  }
  while(true)
    for(auto& c : vec_client){
    counter[thread_id] += c.getUID();
    }
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cout << "usage ./databaseBench <clients> <times>\n";
    return 0;
  }
  signal(SIGALRM, timeout);
  int thread_count = 16;
  ::client_count = stoi(argv[1]);
  ::continue_time = stoi(argv[2]);

  counter.resize(thread_count, 0);
  alarm(continue_time);
  vector<thread> threads;

  int clientPerThread = client_count / 8;

  for (int i = 0; i < thread_count; ++i)
  {
    threads.emplace_back(BenchFunc, client_count, i);
  }
  for (auto &t : threads)
  {
    t.join();
  }
}