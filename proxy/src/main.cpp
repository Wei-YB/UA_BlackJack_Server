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
    std::shared_ptr<ServiceClient> lobbyClient = std::make_shared<ConcreteServiceClient<lobby::Lobby>>(std::string(lobbyAddress));
    std::shared_ptr<ServiceClient> roomClient = std::make_shared<ConcreteServiceClient<room::Room>>(std::string(roomAddress));
    std::shared_ptr<ServiceClient> socialClient = std::make_shared<ConcreteServiceClient<social::Social>>(std::string(socialAddress));

    std::shared_ptr<ProxyServer> proxyServer = std::make_shared<ProxyServer>(argv[1], (unsigned short)(atoi(argv[2])), &loop);

    std::shared_ptr<ProxyRpcServer> rpcServer
}
