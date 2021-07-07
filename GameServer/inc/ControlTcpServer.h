#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
namespace ua_blackjack::Game
{
    void createServiece(void);
    void connectToParent(void); //子进程调用，用于接受父进程发送的rpc请求
    void connectToson(void);    //父进程调用，向子进程发送rpc请求
}