#include <iostream>
#include <signal.h>
#include "Timer.h"

bool g_flag = false;

void stop_program(int)
{
    g_flag = true;
}

void OnTimeOut()
{
    static int cnt = 0;
    std::cout << "This is the " << ++cnt << "th timeout event." << std::endl;
}

using Net::Timer;
using Net::EventLoop;

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cout << "usage: " << std::string(argv[0]) << " sec milli micro" << std::endl;
        return 0;
    }
    EventLoop loop;
    Timer timer(&loop, OnTimeOut);
    timer.SetPeriod(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

    signal(SIGINT, stop_program);
    while (!g_flag && loop.loopOnce(1000) > -1);

    return 0;
}