#include <iostream>
#include <string>

#include "RpcServer.h"
#include "lobby.grpc.pb.h"
#include "room.grpc.pb.h"
#include "social.grpc.pb.h"
#include "common.pb.h"
#include "serverAddresses.h"
using common::Request;
using common::Response;

void trivialRpcHandler(const Request &request, Response &response)
{
    std::cout << "get one request" << std::endl;
    int64_t uid = request.uid();
    // handle login situation
    if (request.requesttype() == Request::LOGIN || request.requesttype() == Request::SIGNUP)
    {
        // assign a random number for uid
        uid = (int64_t)&request;
    }
    response.set_status(0);
    response.set_stamp(response.stamp());
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
        AsyncServer<lobby::Lobby::AsyncService> server(std::string(lobbyAddress), trivialRpcHandler);
        server.Run();
    }
    else if (moduleName == "room")
    {
        AsyncServer<room::Room::AsyncService> server(std::string(roomAddress), trivialRpcHandler);
        server.Run();
    }
    else if (moduleName == "social")
    {
        AsyncServer<social::Social::AsyncService> server(std::string(socialAddress), trivialRpcHandler);
        server.Run();
    }
    else
    {
        std::cout << "unknown back-end module name." << std::endl;
    }

    return 0;
}