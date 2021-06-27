#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <memory>
#include <thread>

#include "ProxyServer.h"
#include "Client.h"
#include "TcpServer.h"
#include "EventLoop.h"
#include "asyncServiceClient.h"
#include "asyncProxyServer.h"
#include "serverAddresses.h"
#include "common.h"
#include "log.h"

#include "UA_BlackJack.grpc.pb.h"
#include "UA_BlackJack.pb.h"

using ua_blackjack::LobbyService;
using ua_blackjack::GameService;
using ua_blackjack::SocialService;

bool flag = false;

void stop_server(int)
{
    flag = true;
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cout << "usage: " << std::string(argv[0]) << " ip port log_file_path." << std::endl;
        exit(0);
    }

    set_logger_name("async_logger");
    set_log_path(std::string(argv[3]));
    auto logger = create_logger();

    logger_ptr->info("In main thread: Successfully create an async logger.");

    signal(SIGINT, stop_server);

    Net::EventLoop loop;
    std::shared_ptr<ServiceClient> lobbyClient = 
            std::make_shared<ConcreteServiceClient<LobbyService>>("Lobby", std::string(lobbyAddress));
    std::shared_ptr<ServiceClient> roomClient = 
            std::make_shared<ConcreteServiceClient<GameService>>("Room", std::string(roomAddress));
    std::shared_ptr<ServiceClient> socialClient = 
            std::make_shared<ConcreteServiceClient<SocialService>>("Social", std::string(socialAddress));

    logger_ptr->info("In main thread: Successfully create three rpc client.");

    std::shared_ptr<ProxyServer> proxyServer = std::make_shared<ProxyServer>(argv[1], (unsigned short)(atoi(argv[2])), &loop);
    // resgister the service Clients to the proxy server
    for (auto iter = requestTypeToModule.begin(); iter != requestTypeToModule.end(); ++iter)
    {
        switch (iter->second)
        {
            case BackEndModule::Lobby:
            {
                proxyServer->RegisterServiceClient(iter->first, lobbyClient);
                break;
            }
            case BackEndModule::Room:
            {
                proxyServer->RegisterServiceClient(iter->first, roomClient);
                break;
            }
            case BackEndModule::Social:
            {
                proxyServer->RegisterServiceClient(iter->first, socialClient);
                break;
            }
            default:
                break;
        }
    }

    logger_ptr->info("In main thread: Successfully create proxy server.");

    lobbyClient->SetResponseCallBack(std::bind(&ProxyServer::OnServiceResponse, proxyServer.get(), std::placeholders::_1));
    roomClient->SetResponseCallBack(std::bind(&ProxyServer::OnServiceResponse, proxyServer.get(), std::placeholders::_1));
    socialClient->SetResponseCallBack(std::bind(&ProxyServer::OnServiceResponse, proxyServer.get(), std::placeholders::_1));

   
    // start service clients as threads
    std::thread lobbyClientThread = std::thread(&ConcreteServiceClient<LobbyService>::AsyncCompleteRpc, lobbyClient.get());
    std::thread roomClientThread = std::thread(&ConcreteServiceClient<GameService>::AsyncCompleteRpc, roomClient.get());
    std::thread socialClientThread = std::thread(&ConcreteServiceClient<SocialService>::AsyncCompleteRpc, socialClient.get());
    
    logger_ptr->info("In main thread: Three rpc client threads start.");

    while (!flag)
    {
        if (loop.loopOnce(1000) < 0)
            stop_server(0);
    }

    logger_ptr->info("In main thread: The event loop stops.");

    lobbyClient->StopClient();
    roomClient->StopClient();
    socialClient->StopClient();

    lobbyClientThread.join();
    roomClientThread.join();
    socialClientThread.join();

    logger_ptr->info("Exit the program.");
    return 0;
}
