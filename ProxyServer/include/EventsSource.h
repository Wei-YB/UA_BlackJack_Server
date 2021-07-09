#ifndef _EVENTSSOURCE_H_
#define _EVENTSSOURCE_H_

#include <memory>
#include <functional>
#include "common.h"

namespace Net {

class EventLoop;
typedef int Event;

class EventsSource : public std::enable_shared_from_this<EventsSource>
{
public:
    EventsSource(FileDesc fd, EventLoop *loop,
                const std::function<int()> &inEventCallBack,
                const std::function<int()> &outEventCallBack,
                const std::function<int()> &errEventCallBack,
                const std::function<int()> &closeEventCallBack);
    
    ~EventsSource();

public:
    int EnableWrite();

    int EnableRead();

    int EnableET();

    int DisableWrite();

    int DisableRead();

    int RemoveFromLoop();

    FileDesc fd() const {return fd_;};

    friend class EventLoop;

// private:
    int HandleEvents(Event events);

private:
    FileDesc fd_;
    Event events_ = 0;
    EventLoop *loop_;
    std::function<int()> inEventCallBack_;
    std::function<int()> outEventCallBack_;
    std::function<int()> errEventCallBack_;
    std::function<int()> closeEventCallBack_;
};

}   // end of namespace Net
#endif