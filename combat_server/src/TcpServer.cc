#include "TcpServer.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>

void ua_blackjack::Game::createServiece(void)
{
    const char *ip = "0.0.0.0";
    int port = 50050;

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    int ret = bind(listenfd, (struct sockaddr *)&address, sizeof address);
    assert(ret >= 0);

    ret = listen(listenfd, 5);
    assert(ret >= 0);

    int epollfd = epoll_create(5);
    epoll_event event;
    event.events = EPOLLIN | EPOLLET; //可读，EDGE TIRGGER
    event.data.fd = listenfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

    epoll_event events[5];
    while (true)
    {
        int ret = epoll_wait(epollfd, events, 5, -1);
        if (ret < 0)
        {
            spdlog::error("EPOLL ERROR");
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
                        auto pid = fork();
                        if (pid == 0) //子进程
                        {
                        }
                        else if (pid > 0) //父进程
                        {
                        }
                        else //出错了!!!!!!!!!
                        {
                            spdlog::error("restarting failure");
                        }
                    }
                }
            }
        }
    }

    return;
}