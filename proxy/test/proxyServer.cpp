#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include "../include/proxy.h"
#include "../include/net/EventLoop.h"
#include "../include/net/TcpListener.h"

#include "serverAddresses.h"
#include "asyncServiceClient.h"

std::unordered_map<int64_t, Net::EventsHandler*> g_uidToClient;
std::mutex g_lock;

bool flag = false;

void stop_server(int sig)
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
    Net::EventLoop loop;
    

    signal(SIGINT, stop_server);

    RpcClient<lobby::Lobby> lobbyClient(std::string(lobbyAddress), &g_uidToClient, &g_lock);
    RpcClient<room::Room> roomClient(std::string(roomAddress), &g_uidToClient, &g_lock);
    RpcClient<social::Social> socialClient(std::string(socialAddress), &g_uidToClient, &g_lock);

    Net::TcpListener<ClientHandler> *listener = new Net::TcpListener<ClientHandler>(argv[1], atoi(argv[2]), 
                                                                                    &g_uidToClient, &g_lock,
                                                                                    &lobbyClient, 
                                                                                    &roomClient,
                                                                                    &socialClient);
    if (0 > listener->addToEventLoop(&loop))
    {
        std::cout << "fail to add listener to event loop" << std::endl;
        delete listener;
        exit(0);
    }

    std::thread lobbyClientThread = std::thread(&RpcClient<lobby::Lobby>::AsyncCompleteRpc, &lobbyClient, std::ref(flag));
    std::thread roomClientThread = std::thread(&RpcClient<room::Room>::AsyncCompleteRpc, &roomClient, std::ref(flag));
    std::thread socialClientThread = std::thread(&RpcClient<social::Social>::AsyncCompleteRpc, &socialClient, std::ref(flag));

    while (!flag)
    {
        if (0 > loop.loopOnce(1000))
        {
            break;
        }
    }
    std::cout << "exit the main loop." << std::endl;
    
    lobbyClientThread.join();
    roomClientThread.join();
    socialClientThread.join();
    
    return 0;
}
