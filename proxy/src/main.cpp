#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <thread>

#include "ProxyServer.h"
#include "Client.h"
#include "EventLoop.h"
#include "TcpServer.h"
#include "asyncServiceClient.h"
#include "asyncProxyServer.h"
#include "common.h"
#include "log.h"

#include "UA_BlackJack.grpc.pb.h"
#include "UA_BlackJack.pb.h"

using ua_blackjack::LobbyService;
using ua_blackjack::GameService;
using ua_blackjack::SocialService;
using ua_blackjack::PlayerService;

bool flag = false;

void stop_server(int)
{
    flag = true;
}

const char *path_to_pid_file = "/tmp/ua_blackjack/proxy.pid";

static void ReadConfig(const char *config_path, 
                       std::unordered_map<std::string, std::string> &config)
{
    std::ifstream fin(config_path, std::ifstream::in);
    if (!fin.good())
    {
        std::cerr << "In ReadConfig: fail to read config file." << std::endl;
        exit(EXIT_FAILURE);
    }
    while (fin.good())
    {
        std::string line, key, value;
        getline(fin, line);
        std::istringstream iss(line);
        iss >> key >> value;
        config.emplace(key, value);
    }
}

static void Deamonize(const std::unordered_map<std::string, std::string> &config)
{
    pid_t pid;

    /* fork to let the child process detach from the terminal */
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "In deamonizing: fail to fork." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
        exit(EXIT_SUCCESS);
  
    if (setsid() < 0)
    {
        std::cerr << "In deamonizing: fail to setsid()." << std::endl;
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* fork twice to avoid process obtain a terminal */
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "In deamonizing: fail to fork the second time." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    
    logger_flush_on();
    set_logger_name(config.find("logger_name")->second);
    set_log_path(config.find("log_path")->second);
    create_logger();

    /* create a temporal file storing the pid of the current process */
    int pid_fd = open(path_to_pid_file, O_RDWR | O_CREAT, S_IRWXU);
    if (pid_fd < 0)
    {
        logger_ptr->error("In deamonizing: fail to create pid file, exit now.");
        exit(EXIT_FAILURE);
    }
    pid = getpid();
    
    char buf[64];
    int len = sprintf(buf, "%d", pid);
    write(pid_fd, buf, len);
    close(pid_fd);

    logger_ptr->info("In deamonizing: successfullt deamonize service (pid = {}).", pid);

    /* set new file permission mask */
    umask(0);

    //chdir(new_dir);

    // int fd = sysconf(_SC_OPEN_MAX);
    // while (fd >= 0)
    // {
    //     close(fd--);
    // }
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "usage: " << std::string(argv[0]) << " config_path [start | stop]." << std::endl;
        exit(0);
    }

    // 
    if (std::string(argv[2]) == "start")
    {   // check whether the service is already startted
        int fd = open(path_to_pid_file, O_RDWR, S_IRWXU);
        char buf[64];
        memset(buf, '\0', sizeof(buf));
        if (fd > 0)
        {
            read(fd, buf, sizeof(buf));
            int pid = atoi(buf);
            std::cout << "Service already startted, pid = " << pid << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if (std::string(argv[2]) == "stop")
    {
        // check whether the service is already startted
        int fd = open(path_to_pid_file, O_RDWR, S_IRWXU);
        char buf[64];
        memset(buf, '\0', sizeof(buf));
        if (fd < 0)
        {
            std::cout << "Service not start." << std::endl;
            exit(EXIT_FAILURE);
        }
        read(fd, buf, sizeof(buf));
        int pid = atoi(buf);
        kill((pid_t)pid, SIGINT);
        remove(path_to_pid_file);
        exit(EXIT_SUCCESS);
    }
    else
    {
        std::cout << "unknown arg: " << std::string(argv[2]) << std::endl;
        exit(0);
    }

    std::unordered_map<std::string, std::string> config;

    ReadConfig(argv[1], config);

    Deamonize(config);

    logger_ptr->info("In main thread: Successfully create an async logger.");

    signal(SIGINT, stop_server);

    Net::EventLoop loop;

    std::shared_ptr<ServiceClient> lobbyClient = std::make_shared<ConcreteServiceClient<LobbyService>>("lobby", config["lobbyAddress"]);
    std::shared_ptr<ServiceClient> roomClient = std::make_shared<ConcreteServiceClient<GameService>>("room", config["roomAddress"]);
    std::shared_ptr<ServiceClient> socialClient = std::make_shared<ConcreteServiceClient<SocialService>>("social", config["socialAddress"]);
    std::shared_ptr<ServiceClient> playerClient = std::make_shared<ConcreteServiceClient<PlayerService>>("player", config["playerAddress"]);
  
    logger_ptr->info("In main thread: Successfully create four rpc clients.");

    std::shared_ptr<ProxyServer> proxyServer = std::make_shared<ProxyServer>(config.find("host")->second.c_str(),
                                                                             (unsigned short)(atoi(config.find("port")->second.c_str())),
                                                                             &loop);
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
            case BackEndModule::Player:
            {
                proxyServer->RegisterServiceClient(iter->first, playerClient);
            }
            default:
                break;
        }
    }

    logger_ptr->info("In main thread: Successfully create proxy server.");

    lobbyClient->SetResponseCallBack(std::bind(&ProxyServer::OnServiceResponse, proxyServer.get(), std::placeholders::_1));
    roomClient->SetResponseCallBack(std::bind(&ProxyServer::OnServiceResponse, proxyServer.get(), std::placeholders::_1));
    socialClient->SetResponseCallBack(std::bind(&ProxyServer::OnServiceResponse, proxyServer.get(), std::placeholders::_1));
    playerClient->SetResponseCallBack(std::bind(&ProxyServer::OnServiceResponse, proxyServer.get(), std::placeholders::_1));

    // start service clients as threads
    std::thread lobbyClientThread = std::thread(&ConcreteServiceClient<LobbyService>::AsyncCompleteRpc, lobbyClient.get());
    std::thread roomClientThread = std::thread(&ConcreteServiceClient<GameService>::AsyncCompleteRpc, roomClient.get());
    std::thread socialClientThread = std::thread(&ConcreteServiceClient<SocialService>::AsyncCompleteRpc, socialClient.get());
    std::thread playerClientThread = std::thread(&ConcreteServiceClient<PlayerService>::AsyncCompleteRpc, playerClient.get());

#if (DEBUG_MODE == 0)
    // start the gRPC service
    ProxyRpcServer gRpcServer(config.find("proxyAddress")->second, proxyServer);
    proxyServer->SetClientResponseCallBack(std::bind(&ProxyRpcServer::OnClientResponse, &gRpcServer, std::placeholders::_1));
    std::thread gRpcServerThread = std::thread(&ProxyRpcServer::Run, &gRpcServer);



    logger_ptr->info("In main thread: Four rpc client threads start.");
#endif
    while (!flag)
    {
        if (loop.loopOnce(1000) < 0)
            stop_server(0);
    }

    logger_ptr->info("In main thread: The event loop stops.");

    lobbyClient->StopClient();
    roomClient->StopClient();
    socialClient->StopClient();
    playerClient->StopClient();
#if (DEBUG_MODE == 0)
    gRpcServer.Stop();
#endif
    lobbyClientThread.join();
    roomClientThread.join();
    socialClientThread.join();
    playerClientThread.join();
#if (DEBUG_MODE == 0)
    gRpcServerThread.join();
#endif

    logger_ptr->info("Exit the program.");
    return 0;
}
