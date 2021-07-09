#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <ctime>
#include <cassert>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <grpcpp/grpcpp.h>
#include "../grpc/UA_BlackJack.grpc.pb.h"


using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;


// Service dependent
/*************************/
using ua_blackjack::Request;
using ua_blackjack::Response;
using ua_blackjack::LobbyService;
using ua_blackjack::Request_RequestType;

using UID = int64_t;
using RoomID = int32_t;


class ClientSyn  {
 public:
  ClientSyn(std::shared_ptr<Channel> channel)
      : stub_(LobbyService::NewStub(channel)) {}

    UID Login(std::string username, std::string password);

    bool JoinRoom(UID uid, RoomID rid);

    RoomID CreateRoom(UID uid);

    bool LeaveRoom(UID uid);

    bool PlayerReady(UID uid);

    RoomID QuickMatch(UID uid);

    std::vector<RoomID> GetAvailableRoomList();

    void MatchEnd(RoomID rid);

    void Logout(UID uid);

    // get current local time stamp
    int64_t getCurrentLocalTimeStamp();

private:
  std::unique_ptr<LobbyService::Stub> stub_;
  
};

class ClientAsyn {
 public:
  explicit ClientAsyn(std::shared_ptr<Channel> channel)
      : stub_(LobbyService::NewStub(channel)) {
        //开启一个新线程去执行AsyncCompleteRpc()
        std::thread newThread = std::thread(&ClientAsyn::AsyncCompleteRpc, this);
        newThread.detach();
      }

  UID Login(std::string username, std::string password);

  bool JoinRoom(UID uid, RoomID rid);

  RoomID CreateRoom(UID uid);

  bool LeaveRoom(UID uid);

  void AsynClientCall(Request request);

  // Loop while listening for completed responses.
  // Prints out the response from the server.
  // 异步完成的rpc调用，此函数应该放在一个新线程里开启！它是一直在监听来自服务端的响应，通过判断cq_.Next有没有返回来实现的！
  void AsyncCompleteRpc();
  

 private:
  // struct for keeping state and data information，自行封装的一个struct！
  struct AsyncClientCall {
    Response reply;

    ClientContext context;

    Status status;

    std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
  };

  int64_t getCurrentLocalTimeStamp();

  std::unique_ptr<LobbyService::Stub> stub_;
  CompletionQueue cq_;
};
