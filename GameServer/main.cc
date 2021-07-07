#include <iostream>
#include <queue>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "GameProcess.h"
#include "ClientForDatebase.h"
#include "ClientForLobby.h"
#include "ClientForTestUser.h"
#include "spdlog/spdlog.h"
#include <sstream>
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "ControlTcpServer.h"
#include "ProgramProcess.h"

std::string ProxyServiceAddr;
std::string LobbyServiceAddr;
std::string DatabaseServiceAddr;
std::string controlTcpPort;
std::string gameRpcAddr;
bool start_daemon()
{
    int fd;

    switch (fork())
    {
    case -1:
        spdlog::info("fork() failed\n");
        return false;

    case 0:
        break;

    default:
        exit(0);
    }

    /*
    pid_t setsid(void);
    进程调用setsid()可建立一个新对话期间。
    如果调用此函数的进程不是一个进程组的组长，则此函数创建一个新对话期，结果为：
        1、此进程变成该新对话期的对话期首进程（session leader，对话期首进程是创建该对话期的进程）。
           此进程是该新对话期中的唯一进程。
        2、此进程成为一个新进程组的组长进程。新进程组ID就是调用进程的进程ID。
        3、此进程没有控制终端。如果在调用setsid之前次进程有一个控制终端，那么这种联系也被解除。
    如果调用进程已经是一个进程组的组长，则此函数返回错误。为了保证不处于这种情况，通常先调用fork()，
    然后使其父进程终止，而子进程继续执行。因为子进程继承了父进程的进程组ID，而子进程的进程ID则是新
    分配的，两者不可能相等，所以这就保证了子进程不是一个进程组的组长。
    */
    if (setsid() == -1)
    {
        spdlog::info("setsid() failed\n");
        return false;
    }

    switch (fork())
    {
    case -1:
        spdlog::info("fork() failed\n");
        return false;

    case 0:
        break;

    default:
        exit(0);
    }

    umask(0);

    long maxfd;
    if ((maxfd = sysconf(_SC_OPEN_MAX)) != -1)
    {
        for (fd = 0; fd < maxfd; fd++)
        {
            close(fd);
        }
    }

    fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        spdlog::info("open(\"/dev/null\") failed\n");
        return false;
    }

    /*
    // Standard file descriptors.
    #define STDIN_FILENO    0   // Standard input.
    #define STDOUT_FILENO   1   // Standard output.
    #define STDERR_FILENO   2   // Standard error output.
    */

    /*
    int dup2(int oldfd, int newfd);
    dup2()用来复制参数oldfd所指的文件描述符，并将它拷贝至参数newfd后一块返回。
    如果newfd已经打开，则先将其关闭。
    如果oldfd为非法描述符，dup2()返回错误，并且newfd不会被关闭。
    如果oldfd为合法描述符，并且newfd与oldfd相等，则dup2()不做任何事，直接返回newfd。
    */
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        spdlog::info("dup2(STDIN) failed\n");
        return false;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        spdlog::info("dup2(STDOUT) failed\n");
        return false;
    }

    if (dup2(fd, STDERR_FILENO) == -1)
    {
        spdlog::info("dup2(STDERR) failed\n");
        return false;
    }

    if (fd > STDERR_FILENO)
    {
        if (close(fd) == -1)
        {
            spdlog::info("close() failed\n");
            return false;
        }
    }

    return true;
}
bool isProgramRelase;
ServiceStatus serviceStatus;
std::string configFilePath = "../../GameServer/game.config";
std::string logFilePath = "logs/async_log.log";
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage " << argv[0] << "  [RELEASE TEST] [OPTION -cf(config file path) -lf(log file path)]" << std::endl; //用户只能以RELEASE和TEST启动，RESTART是由fork---->exec 执行的
        return 0;
    }
    if (strcmp(argv[1], "RELEASE") == 0)
    {
        isProgramRelase = true;
        serviceStatus = ServiceStatus::HANDEL_GRPC_BY_ITSELF;
    }
    else if (strcmp(argv[1], "TEST") == 0)
    {
        isProgramRelase = false;
        serviceStatus = ServiceStatus::HANDEL_GRPC_BY_ITSELF;
    }
    else if (strcmp(argv[1], "RESTART") == 0) //重启的程序
    {
        isProgramRelase = true;
        spdlog::info("restarting program running....");
        serviceStatus = ServiceStatus::HANDEL_GRPC_BY_PARENT;
    }
    else
    {
        std::cout << "Usage " << argv[0] << " [RELEASE TEST] [OPTION -cf(config file path) -lf(log file path)]" << std::endl;
        return 0;
    }

    if (argc == 4 || argc == 6)
    {
        for (int i = 2; i < argc; i += 2)
        {
            if (strcmp(argv[i], "-cf") == 0)
            {
                configFilePath = argv[i + 1];
                std::cout << "read congfile path.." << std::endl;
            }
            else if (strcmp(argv[i], "-lf") == 0)
            {
                logFilePath = argv[i + 1];
                std::cout << "read logfile path.." << std::endl;
            }
            else
            {
                std::cout << "Usage " << argv[0] << " [RELEASE TEST] [OPTION -cf(config file path) -lf(log file path)]" << std::endl;
                return 0;
            }
        }
    }
    else if (argc > 2)
    {
        std::cout << "Usage " << argv[0] << " [RELEASE TEST] [OPTION -cf(config file path) -lf(log file path)]" << std::endl;
        return 0;
    }

    readConfigFile(configFilePath); //读取filepath

    std::cout << "ProxyServiceAddr = " << ProxyServiceAddr << std::endl;
    std::cout << "LobbyServiceAddr = " << LobbyServiceAddr << std::endl;
    std::cout << "DatabaseServiceAddr = " << DatabaseServiceAddr << std::endl;
    std::cout << "controlTcpPort = " << controlTcpPort << std::endl;
    std::cout << "gameRpcAddr = " << gameRpcAddr << std::endl;
    std::cout << "configFilePath = " << configFilePath << std::endl;
    std::cout << "logFilePath = " << logFilePath << std::endl;

    // std::cout << "start_daemon..." << std::endl;
    // start_daemon(); //守护

    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", logFilePath);
    spdlog::set_default_logger(async_file);
    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_ITSELF) //正常的通过命令行启动
    {
        std::cout << "Run!!!!!!!!!!!!!!!!!!!" << std::endl;
        std::thread thread_ = std::thread(&ua_blackjack::Game::ClientForTestUser::AsyncCompleteRpc, &ua_blackjack::Game::ClientForTestUser::getInstance());
        std::thread thread2_ = std::thread(&ua_blackjack::Game::ClientForDatebase::AsyncCompleteRpc, &ua_blackjack::Game::ClientForDatebase::getInstance());
        std::thread threadReceiveRestartCommand = std::thread(&ua_blackjack::Game::createServiece);

        ua_blackjack::Game::ServerImpl::getInstance().Run();

        spdlog::info("grpc async begin...");

        //创建等待RPC的协程
        co_create(&receiveSignalFromRPC, NULL, waitingSignalFromOtherModule, &ua_blackjack::Game::ServerImpl::getInstance());
        co_resume(receiveSignalFromRPC);

        //回收协程的协程
        co_create(&recoverystCo, NULL, recoveryUnusedCo, NULL);
        co_resume(recoverystCo);

        //开启协程
        co_eventloop(co_get_epoll_ct(), NULL, NULL);

        thread_.join();
        thread2_.join();
        threadReceiveRestartCommand.join();
        return 0;
    }
    else if (serviceStatus == ServiceStatus::HANDEL_GRPC_BY_PARENT) //hot reload
    {
        spdlog::info("Hello I am a new program !!!!!!!");
        //对于子进程而言，客户端可以正常的使用自己的client去接入proxy
        std::thread thread_ = std::thread(&ua_blackjack::Game::ClientForTestUser::AsyncCompleteRpc, &ua_blackjack::Game::ClientForTestUser::getInstance());
        std::thread thread2_ = std::thread(&ua_blackjack::Game::ClientForDatebase::AsyncCompleteRpc, &ua_blackjack::Game::ClientForDatebase::getInstance());
        std::thread thread3_ = std::thread(&ua_blackjack::Game::connectToParent::run, &ua_blackjack::Game::connectToParent::getInstance());
        //创建等待RPC的协程
        co_create(&receiveSignalFromRPC, NULL, waitingSignalFromOtherModule, NULL);
        co_resume(receiveSignalFromRPC);

        //回收协程的协程
        co_create(&recoverystCo, NULL, recoveryUnusedCo, NULL);
        co_resume(recoverystCo);

        //开启协程
        co_eventloop(co_get_epoll_ct(), NULL, NULL);

        thread_.join();
        thread2_.join();
        return 0;
    }
}
