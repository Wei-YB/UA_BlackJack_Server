#include "DBServerAsynImpl.h"


ServerAsynImpl* server = nullptr;

void start() {
    server->Run();
}

int main(int argc, char** argv) {

    


    std::string redis_host = "127.0.0.1:6379";
    std::string rpc_host   = "0.0.0.0:50051";
    const int   init_score = 2000;
    spdlog::set_level(spdlog::level::info);
    server = new ServerAsynImpl(rpc_host, redis_host, init_score);

    std::thread t1(start);

    getchar();

    server->Shutdown();

    t1.join();
    std::cout << "server stop" << std::endl;

    return 0;
}
