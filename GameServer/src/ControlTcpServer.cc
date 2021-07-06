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
                        spdlog::info("restarting...");
                        std::stringstream ss;
                        ss << "restart receive ";
                        int ret = send(fd, ss.str().c_str(), ss.str().size(), 0);
                        serviceStatus = ServiceStatus::HANDEL_GRPC_BY_PARENT_START_FORWARD;
                        int pid = fork();
                        if (pid == 0)
                        {
                            std::string songLogPath = logFilePath + ".son.log";
                            execl("./ GameService", "RESTART", "-lf", songLogPath.c_str(), "-cf", configFilePath.c_str());
                        }
                        else
                        {
                            while (true)
                            {
                                if (playerHashMap.size() == 0) //所有对局都结束
                                {
                                    struct msg_st data;
                                    strcpy(data.text, "Complete");
                                    //发送request
                                    auto msgid = msgget((key_t) static_cast<int>(MSG_KEY_E::MSG_KEY_COMPLETE_ALL_OPE), 0666 | IPC_CREAT);
                                    if (msgid == -1)
                                    {
                                        spdlog::error("msgget failed with error");
                                        exit(EXIT_FAILURE);
                                    }
                                    if (msgsnd(msgid, (void *)&data, MAX_TEXT, 0) == -1)
                                    {
                                        spdlog::error("msgsnd failed");
                                        exit(EXIT_FAILURE);
                                    }
                                }
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