#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <memory>

#include "ProxyServer.h"
#include "Client.h"
#include "TcpServer.h"
#include "EventLoop.h"
#include "asyncServiceClient.h"
#include "asyncProxyServer.h"
#include "serverAddresses.h"
#include "common.h"

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
    if (argc < 3)
    {
        std::cout << "usage: " << std::string(argv[0]) << " ip port." << std::endl;
        exit(0);
    }

    signal(SIGINT, stop_server);

    Net::EventLoop loop;
    std::shared_ptr<ServiceClient> lobbyClient = 
            std::make_shared<ConcreteServiceClient<LobbyService>>(std::string(lobbyAddress));
    std::shared_ptr<ServiceClient> roomClient = 
            std::make_shared<ConcreteServiceClient<GameService>>(std::string(roomAddress));
    std::shared_ptr<ServiceClient> socialClient = 
            std::make_shared<ConcreteServiceClient<SocialService>>(std::string(socialAddress));

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
   
    // start service clients as threads
    std::thread lobbyClientThread = std::thread(&ConcreteServiceClient<LobbyService>::AsyncCompleteRpc, lobbyClient.get());
    std::thread roomClientThread = std::thread(&ConcreteServiceClient<GameService>::AsyncCompleteRpc, roomClient.get());
    std::thread socialClientThread = std::thread(&ConcreteServiceClient<SocialService>::AsyncCompleteRpc, socialClient.get());
    
    while (!flag)
    {
        if (loop.loopOnce(1000) < 0)
            stop_server(0);
    }

    // lobbyClient->StopClient();
    // roomClient->StopClient();
    // socialClient->StopClient();

    // lobbyClientThread.join();
    // roomClientThread.join();
    // socialClientThread.join();

    return 0;
}
