#pragma once
#include "EventLoop.h"
#include <unistd.h>
#include <sys/eventfd.h>

using Net::EventsSource;
using Net::EventLoop;

class Notifyer
{
private:
    std::shared_ptr<EventsSource> eventsSource_;
    std::function<void(uint64_t)> eventCallBack_;
    std::function<void()> errorCallBack_;

public:
    Notifyer(EventLoop *loop,
             const std::function<void(uint64_t)> &eventCallBack,
             const std::function<void()> &errorCallBack) 
             : eventsSource_(std::make_shared<EventsSource>(eventfd(0, EFD_NONBLOCK),
                                                            loop, 
                                                            std::bind(&Notifyer::OnEvents, this),
                                                            nullptr,
                                                            std::bind(&Notifyer::OnError, this),
                                                            nullptr)),
             eventCallBack_(eventCallBack), errorCallBack_(errorCallBack)
    {
        eventsSource_->EnableRead();
        // eventsSource_->EnableWrite();
        // eventsSource_->SetNonBlocking();
    }

    int Notify(uint64_t n)
    {
        return eventfd_write(eventsSource_->fd(), n);
    }

// private:
    int OnEvents()
    {
        uint64_t n;
        int ret = read(eventsSource_->fd(), (void*)n, sizeof(n));
        if (eventfd_read(eventsSource_->fd(), &n) < 0)
        {
            return -1;
        }
        if (eventCallBack_) eventCallBack_(n);
        return 0;
    }

    int OnError() 
    {
        if (errorCallBack_) errorCallBack_();
        return -1;
    }
};