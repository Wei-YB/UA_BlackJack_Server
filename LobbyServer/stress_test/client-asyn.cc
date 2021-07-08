/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <thread>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

// Service dependent
/*************************/
using demo::GetNameService;
using demo::NameReply;
using demo::NameRequest;
/*************************/

class Client {
 public:
  explicit Client(std::shared_ptr<Channel> channel)
      : stub_(GetNameService::NewStub(channel)), cnt(0) {
        //开启一个新线程去执行AsyncCompleteRpc()
        //若同时开启多个线程去执行AsyncCompleteRpc()，那么服务端的请求回来时只会唤醒其中一个线程的cq_.Next()
        std::thread newThread = std::thread(&Client::AsyncCompleteRpc, this);
        newThread.detach();
        std::thread newThread1 = std::thread(&Client::AsyncCompleteRpc, this);
        newThread1.detach();
      }

  // Assembles the client's payload and sends it to the server.
  void GetName(int id) {
    // Data we are sending to the server.
    NameRequest request;
    request.set_id(id);

    // Call object to store rpc data
    AsyncClientCall* call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncGetName(&call->context, request, &cq_);

    // StartCall initiates the RPC call
    
    //StartCall()和Finish()都会立即返回，无需等待服务端的响应！
    call->response_reader->StartCall();
    std::cout << "Startcall() complete " << std::endl;

    call->response_reader->Finish(&call->reply, &call->status, (void*)call);
    std::cout << "Finish() complete " << std::endl;
  }

  






  // Loop while listening for completed responses.
  // Prints out the response from the server.
  // 异步完成的rpc调用，此函数应该放在一个新线程里开启！它是一直在监听来自服务端的响应，通过判断cq_.Next有没有返回来实现的！
  void AsyncCompleteRpc() {
    void* got_tag;
    bool ok = false;
    

    std::cout << "before cq_.Next" << std::endl;

    // Block until the next result is available in the completion queue "cq".

    // 故虽然是异步，但当服务端没有回应时就会阻塞在cq_.Next里，所以此成员函数应该放在新的线程里运行，让此阻塞不会妨碍主线程的运行！
    // 需要循环调用cq_里的Next方法！
    // 这个while是一个无限循环，用于监听来自服务器的响应
    while (cq_.Next(&got_tag, &ok)) {
      std::cout << "cq_.Next has returned." << std::endl;

      AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);

      GPR_ASSERT(ok);

      if (call->status.ok()){
        std::lock_guard<std::mutex> guard(mtx);//此lock_guard对象被创建的时候自动lock(), 对象离开作用域时会自动销毁，并调用unlock()！
        
        std::cout <<"pid:"<<getpid() <<"  threadid:"<< std::this_thread::get_id()<<"  Greeter received: " << call->reply.name() <<"for " << ++cnt <<" times"<< std::endl;

      }
        
      else
        std::cout << "RPC failed" << std::endl;

      delete call;
    }
  }

 private:
  // struct for keeping state and data information，自行封装的一个struct！
  struct AsyncClientCall {
    NameReply reply;

    ClientContext context;

    Status status;

    std::unique_ptr<ClientAsyncResponseReader<NameReply>> response_reader;
  };

  std::unique_ptr<GetNameService::Stub> stub_;
  CompletionQueue cq_;
  int cnt;
  std::mutex mtx;
};

int main(int argc, char** argv) {
  Client client(grpc::CreateChannel(
      "localhost:50050", grpc::InsecureChannelCredentials()));

  // Spawn reader thread that loops indefinitely
  // 类的成员函数可以放在不同的线程里
  // 如这里同一个实例client的getname就是在主线程里运行，二AsyncCompleteRpc就是在thread_运行。
  // std::thread thread_ = std::thread(&Client::AsyncCompleteRpc, &client);


  //这里的for是一次性发送了10次请求，而不需要等待任何一次请求的回复，就可以去做其他事情！
  for (int i = 0; i < 10; i++) {
    int id = i;
    std::cout << "The actual RPC call:getname(id) is starting: " << std::endl;
    client.GetName(id);  // The actual RPC call!
    std::cout << "The actual RPC call:getname(id) complete" << std::endl;

  }

  //join()为主线程等待子线程的阻塞模式
  //detach()为主线程不管子线程的非阻塞模式

  std::cout << "wait in main" << std::endl;
  pause();
  return 0;
}
