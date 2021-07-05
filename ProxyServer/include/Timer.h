#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/timerfd.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <functional>

#include "EventLoop.h"

namespace Net {


class Timer {
private:
    EventLoop *loop_;
    struct itimerspec setting_;
    std::shared_ptr<EventsSource> eventsSource_; 
    std::function<void()> timeOutCallback_;

public:
    Timer(EventLoop *loop, const std::function<void()> &cb) 
        : eventsSource_(std::make_shared<EventsSource>(::timerfd_create(CLOCK_REALTIME, 0), 
                                                        loop,
                                                        std::bind(&Timer::OnTimeOut, this),
                                                        nullptr,
                                                        std::bind(&Timer::OnError, this),
                                                        nullptr)), 
        timeOutCallback_(cb) 
    {
        eventsSource_->EnableRead();
    }

    ~Timer() 
    {
        eventsSource_->RemoveFromLoop();
    }

    int SetExpired(int sec, int milli = 0, int micro = 0)
    {
        setting_.it_value.tv_sec = sec + (milli / 1000) + (micro / 1000000);
        setting_.it_value.tv_nsec = (milli % 1000) * 1000000 + (micro % 1000000) * 1000;
        setting_.it_interval.tv_sec = setting_.it_interval.tv_nsec = 0;
        return timerfd_settime(eventsSource_->fd(), 0, &setting_, NULL);
    }

    int SetPeriod(int sec, int milli = 0, int micro = 0)
    {
        setting_.it_interval.tv_sec = setting_.it_value.tv_sec = sec + (milli / 1000) + (micro / 1000000);
        setting_.it_interval.tv_nsec = setting_.it_value.tv_nsec = (milli % 1000) * 1000000 + (micro % 1000000) * 1000;
        return timerfd_settime(eventsSource_->fd(), 0, &setting_, NULL);
    }

private:
    int OnTimeOut()
    {
        uint64_t tmp;
        if (sizeof(uint64_t) > ::read(eventsSource_->fd(), (void*)&tmp, sizeof(uint64_t)))
        {
            return -1;
        }
        if (timeOutCallback_)
        {
            timeOutCallback_();
        }
        return 0;
    }

    int OnError()
    {
        return -1;
    }
};

}

#endif
