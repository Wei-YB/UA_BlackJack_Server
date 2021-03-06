#include "ControlTcpServer.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include "combat_typedef.h"
#include "GameProcess.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
void ua_blackjack::Game::createServiece(void)
{
    const char *ip = "0.0.0.0";
    int port = atoi(controlTcpPort.c_str());

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, (const void *)&reuse, sizeof(int)); //当服务端出现timewait状态的链接时，确保server能够重启成功。
    if (listenfd < 0)
    {
        spdlog::error("create socket error");
        exit(1);
    }

    int ret = bind(listenfd, (struct sockaddr *)&address, sizeof address);
    if (ret < 0)
    {
        spdlog::error("bind error {0}", controlTcpPort);
        exit(1);
    }

    ret = listen(listenfd, 5);
    if (ret < 0)
    {
        spdlog::error("bind error");
        exit(1);
    }
    spdlog::info("===========control port{0}===========", port);
    int epollfd = epoll_create(5);
    epoll_event event;
    event.events = EPOLLIN; //可读，EDGE TIRGGER
    event.data.fd = listenfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

    epoll_event events[5];
    while (true)
    {
        int ret = epoll_wait(epollfd, events, 5, -1);
        if (ret < 0)
        {
            spdlog::error("EPOLL ERROR");
            exit(1);
        }
        for (int i = 0; i < ret; i++) //处理每一个epoll事件
        {

            int fd = events[i].data.fd;
            if (fd == listenfd && events[i].events & EPOLLIN) //listnfd收到epoll   这里有个坑，要用自定义的ptr
            {
                struct sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocketfd = accept(listenfd, (sockaddr *)(&clientAddr), &clientAddrLen);
                epoll_event event;
                event.events = EPOLLIN | EPOLLET; //可读，EDGE TIRGGER
                event.data.fd = clientSocketfd;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, clientSocketfd, &event);
            }
            else if (events[i].events & EPOLLIN) //非listenfd且可读
            {
                spdlog::info("accept suuccess");
                while (true)
                {
                    char buffer[128];
                    memset(buffer, '\0', 128);
                    int ret = recv(fd, buffer, 127, 0);
                    if (ret <= 0) //客户端断开
                        break;
                    spdlog::info("get buffer {0}", buffer);
                    if (strcmp(buffer, "restart") == 0)
                    {
                        spdlog::info("receive restart addr...");
                        memset(buffer, '\0', 128);
                        int ret = recv(fd, buffer, 127, 0);
                        if (ret <= 0) //客户端断开
                            break;
                        spdlog::info("restart addr {0}", buffer);
                        if (roomEnvirHashMap.size() != 0)
                        {
                            int pid = fork();
                            if (pid == 0)
                            {
                                //子进程直接开始新的程序
                                std::string songLogPath = logFilePath + ".son.log";
                                int ret = execl(buffer, "GameService",
                                                "RESTART", "-lf", songLogPath.c_str(), "-cf", configFilePath.c_str(),
                                                (char *)NULL);
                                perror("execl");
                            }
                            else
                            {
                                serviceStatus = ServiceStatus::HANDEL_GRPC_BY_PARENT_START_FORWARD;
                                ua_blackjack::Game::connectToson::getInstance().run();
                            }
                        }
                        else
                        {
                            close(listenfd);
                            close(fd);
                            int pid = fork();
                            if (pid == 0)
                            {
                                //子进程直接开始新的程序
                                std::string songLogPath = logFilePath + ".son.log";
                                int ret = execl(buffer, "GameService",
                                                "RELEASE", "-lf", songLogPath.c_str(), "-cf", configFilePath.c_str(),
                                                (char *)NULL);
                                perror("execl");
                            }
                            else
                            {
                                exit(0);
                            }
                        }
                    }
                    else if (strcmp(buffer, "quit") == 0)
                    {
                        spdlog::info("quiting...");
                        std::stringstream ss;
                        ss << "quiting receive ";
                        int ret = send(fd, ss.str().c_str(), ss.str().size(), 0);
                        close(fd);
                        close(listenfd);
                        exit(0);
                    }
                    else if (strcmp(buffer, "ask") == 0)
                    {
                        std::stringstream ss;
                        ss << "now room num " << roomEnvirHashMap.size();
                        ss << " sum room num " << sumOfRoom << std::endl;

                        for (int i = 0; i < 5; i++)
                        {
                            ss << "room pool " << i + 2 << " size: " << roomPool[i].size() << std::endl;
                        }
                        int ret = send(fd, ss.str().c_str(), ss.str().size(), 0);
                        if (ret <= 0) //客户端断开
                            break;
                    }
                }
            }
        }
    }

    return;
}

void ua_blackjack::Game::connectToParent::run(void)
{
    const char *ip = "0.0.0.0";
    int port = atoi(controlTcpPort.c_str()) + 10;

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sock, (struct sockaddr *)&address, sizeof address) < 0)
    {
        spdlog::error("son - connect to parent");
        exit(1);
    }
    spdlog::info("son - connect to parent success");
    while (true)
    {
        //读取protobuf，这里暂时先不考虑沾包，decode这一大堆东西
        char buffer[1024];
        memset(buffer, '\0', 1024);
        int ret = recv(sock, buffer, 1023, 0);
        spdlog::info("Son receive proto from dad {0}", buffer);
        {
            std::unique_lock<std::mutex> lock(this->mtx);
            //这里将protobuf压入队列
            std::string str(buffer);
            if (str == "Goodbye") //爹死了
            {
                spdlog::info("father go to the hell");
                close(sock);
                std::thread threadReceiveRestartCommand = std::thread(&ua_blackjack::Game::createServiece);
                serviceStatus = ServiceStatus::HANDEL_GRPC_BY_ITSELF;
                threadReceiveRestartCommand.join();
            }

            this->forwardRequestQueue.push(std::move(str));
        }

        { //加锁
            std::unique_lock<std::mutex> lock(this->mtx2);
            while (this->forwardResponceQueue.empty())
            {
                this->cond2.wait(lock);
            }
            auto str = this->forwardResponceQueue.front(); //no copy
            send(sock, str.c_str(), str.size() + 1, 0);    //发送请求
            spdlog::info("son send proto reply to dad{0}", str);
            this->forwardResponceQueue.pop();
        }
    }
}

void ua_blackjack::Game::connectToson::run(void)
{

    const char *ip = "0.0.0.0";
    int port = atoi(controlTcpPort.c_str()) + 10;

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&reuse, sizeof(int)); //当服务端出现timewait状态的链接时，确保server能够重启成功。
    if (listenfd < 0)
    {
        spdlog::error("create socket error");
        exit(1);
    }

    int ret = bind(listenfd, (struct sockaddr *)&address, sizeof address);
    if (ret < 0)
    {
        spdlog::error("bind error");
        exit(1);
    }

    ret = listen(listenfd, 5);
    if (ret < 0)
    {
        spdlog::error("bind error");
        exit(1);
    }
    spdlog::info("===========father process listen {0}===========", port);
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocketfd = accept(listenfd, (sockaddr *)(&clientAddr), &clientAddrLen);
    if (clientSocketfd < 0)
    {
        spdlog::error("Father accept");
        exit(1);
    }
    spdlog::info("Father accept");
    while (true)
    {
        spdlog::info("wait forward request");
        { //加锁
            std::unique_lock<std::mutex> lock(this->mtx);
            while (this->forwardRequestQueue.empty())
            {
                this->cond.wait(lock);
            }

            spdlog::info("receive forward request");
            auto str = this->forwardRequestQueue.front();
            if (str == "Goodbye")
            {
                close(listenfd);
            }
            send(clientSocketfd, str.c_str(), str.size() + 1, 0); //发送请求
            if (str == "Goodbye")
            {
                close(clientSocketfd);
            }
            spdlog::info("Father send protobuf to son  {0}", str);

            this->forwardRequestQueue.pop();
            if (str == "Goodbye")
            {
                if (roomEnvirHashMap.size() == 0) //以防又有rpc接入
                    exit(0);                      //go to the hell
            }
        }
        /***************这里补充protobuf的编码->发送->解码************/
        char buffer[1024];
        memset(buffer, '\0', 1024);
        int ret = recv(clientSocketfd, buffer, 1023, 0);
        spdlog::info("father receive protobuf from son {0}", buffer);
        std::string rep(buffer);
        {
            //加锁
            std::unique_lock<std::mutex> lock(this->mtx2);
            this->forwardResponceQueue.push(rep);
            this->cond2.notify_one();
        }
    }
}