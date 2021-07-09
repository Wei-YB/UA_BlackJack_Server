#include "ProxyRPCClient.h"
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sstream>
#include <iostream>

static const int g_thread_nums = 1000;
static int seconds = 0;
static int total_clients = 0;

static std::vector<int> query_times(g_thread_nums, 0);

static void sig_alarm(int signo){

  auto total_times = 0;
  for(int i = 0; i<g_thread_nums; i++){
    total_times += query_times[i];
  }
  printf("time up.\ntotal_clients is %d.\nquery times is %d.\nQPS is %f.\n", total_clients, total_times, static_cast<double>(total_times) /seconds );
  exit(0);
}

static void QueryFunc(ClientSyn& client, int& query_times) {
  UID a_uid = client.Login("stevejing", "000000");
  RoomID firstRoomID;
  firstRoomID = client.CreateRoom(a_uid);

  while(1){
    client.JoinRoom(a_uid, firstRoomID);
    client.LeaveRoom(a_uid);
    query_times += 2;
  }

}

static void thread_func(int client_nums, int& query_times_per_thread, std::string server_address){
  std::ostringstream oss;
	oss << std::this_thread::get_id();
	std::string stid = oss.str();
	unsigned long long tid = std::stoull(stid);
  SPDLOG_WARN("thread:{0:d} start.", tid);

  //同步client或者异步client
  std::vector<ClientSyn> clients;
  for(int i = 0; i<client_nums; i++){
      auto ptr = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
      if(ptr == nullptr){
        std::cout<< "gRPC create channel failed!"<<std::endl; 
        exit(0);
      }
      clients.emplace_back(ptr);
      
      QueryFunc(clients[i], query_times_per_thread);
    }
}

//TODO. multi clients
void client_asyn_func(std::string server_address){

  auto ptr = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    if(ptr == nullptr){
    std::cout<< "gRPC create channel failed!"<<std::endl; 
    exit(0);
    }
    ClientAsyn client(ptr);

    UID a_uid = client.Login("stevejing", "000000");
    RoomID firstRoomID;
    firstRoomID = client.CreateRoom(a_uid);
    while(1){
      client.JoinRoom(a_uid, firstRoomID);
      client.LeaveRoom(a_uid);

    }
    
}

void client_syn_func(int thread_nums, int client_nums, std::string server_address){

  std::vector<std::thread> client_threads;
  for(int j = 0; j<thread_nums; j++){
    client_threads.emplace_back(thread_func, client_nums, std::ref(query_times[j]), server_address);
    client_threads[j].detach();
  }
}

int main(int argc, char** argv) {
  std::string server_address = "9.135.113.187:50050";

  if(argc != 4){
    std::cout<<"usage: proxy_client <running time> <threads nums> <client nums> "<<std::endl;
    exit(0);
  }
  seconds = atoi(argv[1]);
  int thread_nums = atoi(argv[2]);
  int client_nums = atoi(argv[3]);
  total_clients = client_nums*thread_nums;

  alarm(seconds);
  signal(SIGALRM, sig_alarm);

  spdlog::set_level(spdlog::level::info);
  //spdlog::flush_on(spdlog::level::err);
  
  //client_syn_func(thread_nums, client_nums, server_address);
  client_asyn_func(server_address);

  pause();
  return 0;
}


