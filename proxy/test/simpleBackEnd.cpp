#include <iostream>
#include <string>

#include "RpcServer.h"
#include "UA_BlackJack.pb.h"
#include "common.h"
#include "serverAddresses.h"

using ua_blackjack::Request;
using ua_blackjack::Response;
using ua_blackjack::LobbyService;
using ua_blackjack::GameService;
using ua_blackjack::SocialService;


void trivialRpcHandler(const Request &request, Response &response)
{
    std::cout << "get one request" << std::endl;
    print(std::cout, request);
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
    response.add_args("Sucessfully handle the request.");
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "usage: " << std::string(argv[0]) << "back-end-module-name." << std::endl;
        exit(0);
    }
    std::string moduleName = std::string(argv[1]);
    if (moduleName == "lobby")
    {
        AsyncRpcServer<LobbyService::AsyncService> server(std::string(lobbyAddress), trivialRpcHandler);
        server.Run();
    }
    else if (moduleName == "room")
    {
        AsyncRpcServer<GameService::AsyncService> server(std::string(roomAddress), trivialRpcHandler);
        server.Run();
    }
    else if (moduleName == "social")
    {
        AsyncRpcServer<SocialService::AsyncService> server(std::string(socialAddress), trivialRpcHandler);
        server.Run();
    }
    else if (moduleName == "player")
    {
        AsyncRpcServer<SocialService::AsyncService> server(std::string(playerAddress), trivialRpcHandler);
        server.Run();
    }
    else
    {
        std::cout << "unknown back-end module name." << std::endl;
    }

    std::cout << "server down." << std::endl;
    return 0;
}