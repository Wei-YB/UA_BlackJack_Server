#include <iostream>
#include <string>
#include <thread>

#include "RpcServer.h"
#include "UA_BlackJack.pb.h"
#include "common.h"
#include "log.h"

using ua_blackjack::Request;
using ua_blackjack::Response;
using ua_blackjack::LobbyService;
using ua_blackjack::GameService;
using ua_blackjack::SocialService;

std::string lobbyAddress  = "localhost:50051";
std::string roomAddress   = "localhost:50052";
std::string socialAddress = "localhost:50053";
std::string playerAddress = "localhost:50054";

void trivialRpcHandler(const Request &request, Response &response)
{
    // logger_ptr->info("get one request from client (uid: {})", request.uid());
    // print(std::cout, request);
    int64_t uid = request.uid();
    // handle login situation
    if (request.requesttype() == Request::LOGIN || request.requesttype() == Request::SIGNUP)
    {
        // assign a random number for uid
        uid = (int64_t)&request;
    }
    response.set_status(0);
    response.set_stamp(request.stamp());
    response.set_uid(uid);
    // logger_ptr->info("Sucessfully handle the request.");
}

bool flag = false;

void stop_server(int)
{
    flag = true;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "usage: " << std::string(argv[0]) << " log_path." << std::endl;
        exit(0);
    }

    logger_flush_on();
    set_logger_name("backEndLogger");
    set_log_path("../../log/simpleBackEnd.log");
    create_logger();

    AsyncRpcServer<LobbyService::AsyncService> lobbyServer(lobbyAddress, trivialRpcHandler);
    AsyncRpcServer<GameService::AsyncService> roomServer(roomAddress, trivialRpcHandler);
    AsyncRpcServer<SocialService::AsyncService> socialServer(socialAddress, trivialRpcHandler);
    AsyncRpcServer<PlayerService::AsyncService> playerServer(playerAddress, trivialRpcHandler);
    
    std::thread lobbyServerThread = std::thread(&AsyncRpcServer<LobbyService::AsyncService>::Run, &lobbyServer);
    std::thread roomServerThread = std::thread(&AsyncRpcServer<GameService::AsyncService>::Run, &roomServer);
    std::thread socialServerThread = std::thread(&AsyncRpcServer<SocialService::AsyncService>::Run, &socialServer);
    std::thread playerServerThread = std::thread(&AsyncRpcServer<PlayerService::AsyncService>::Run, &playerServer);

    // a for loop just for listening for the SIGINT
    while (!flag)
    {
        sleep(1);
    }

    lobbyServer.Stop();
    roomServer.Stop();
    socialServer.Stop();
    playerServer.Stop();

    lobbyServerThread.join();
    roomServerThread.join();
    socialServerThread.join();
    playerServerThread.join();

    // logger_ptr->info("In main thread: exit the simpleBackEnd.");

    return 0;
}