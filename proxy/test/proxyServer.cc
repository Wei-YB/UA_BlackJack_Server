#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include "../include/proxy.h"
#include "../include/net/EventLoop.h"
#include "../include/net/TcpListener.h"

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
    Net::TcpListener<ClientHandler> *listener = new Net::TcpListener<ClientHandler>(argv[1], atoi(argv[2]), &g_uidToClient, &g_lock);
    if (0 > listener->addToEventLoop(&loop))
    {
        std::cout << "fail to add listener to event loop" << std::endl;
        delete listener;
        exit(0);
    }

    signal(SIGINT, stop_server);

    while (!flag)
    {
        if (0 > loop.loopOnce(1000))
        {
            break;
        }
    }
    
    return 0;
}
