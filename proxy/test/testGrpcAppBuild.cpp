#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "asyncProxyServer.h"

std::string serverAddr = "127.0.0.1:12345";
std::unordered_map<int64_t, Net::EventsHandler*> g_map;
std::mutex g_lock;

int main()
{
    ProxyServerImpl server(serverAddr, &g_map, &g_lock);
    server.Run();

    return 0;
}