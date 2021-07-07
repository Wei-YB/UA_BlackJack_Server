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
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <queue>
namespace ua_blackjack::Game
{
    void createServiece(void);
    class connectToParent //子进程调用，用于接受父进程发送的rpc请求
    {
    public:
        void run(void);

        std::condition_variable cond;  //forwardRequestQueue的条件
        std::mutex mtx;                //forwardRequestQueue的锁
        std::condition_variable cond2; //forwardResponceQueue的条件
        std::mutex mtx2;               //forwardResponceQueue的锁
        std::queue<std::string> forwardRequestQueue;
        std::queue<std::string> forwardResponceQueue;
        static connectToParent &getInstance()
        {
            static connectToParent instance;
            return instance;
        }

    private:
        connectToParent() {}
        ~connectToParent(){};
        connectToParent(const connectToParent &);
    };

    class connectToson //父进程调用，向子进程发送rpc请求
    {
    public:
        void run(void);

        std::condition_variable cond;  //forwardRequestQueue的条件
        std::mutex mtx;                //forwardRequestQueue的锁
        std::condition_variable cond2; //forwardResponceQueue的条件
        std::mutex mtx2;               //forwardResponceQueue的锁
        std::queue<std::string> forwardRequestQueue;
        std::queue<std::string> forwardResponceQueue;
        static connectToson &getInstance()
        {
            static connectToson instance;
            return instance;
        }

    private:
        connectToson() {}
        ~connectToson(){};
        connectToson(const connectToson &);
    };
}