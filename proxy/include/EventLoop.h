#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <sys/types.h>
#include <sys/epoll.h>
#include <unordered_map>
#include "common.h"

#define DEFAULT_MAX_EVENTS  20000

namespace Net {

typedef int Event;
const Event EV_NULL = 0x00;
const Event EV_IN = 0x01;
const Event EV_OUT = 0x02;
const Event EV_TIMEOUT = 0x04;
const Event EV_ERR = 0x08;
const Event EV_ET = 0x10;

inline int toEpollEvent(Event events)
{
    int epEv = 0;
    epEv |= events & EV_IN ? EPOLLIN : 0x0;
    epEv |= events & EV_OUT ? EPOLLOUT : 0x0;
    epEv |= events & EV_ERR ? EPOLLERR : 0x0;
    epEv |= events & EV_ET ? EPOLLET : 0x0;
    return epEv;
}

inline Event toNetEvent(int epEv)
{
    int events = 0;
    events |= epEv & EPOLLIN ? EV_IN : EV_NULL;
    events |= epEv & EPOLLOUT ? EV_OUT : EV_NULL;
    events |= epEv & EPOLLERR ? EV_ERR : EV_NULL;
    events |= epEv & EPOLLET ? EV_ET : EV_NULL;
    return events;
}

class EventLoop;

class EventsSource
{
public:
    EventsSource(FileDesc fd, EventLoop *loop,
                const std::function<int()> &inEventCallBack,
                const std::function<int()> &outEventCallBack,
                const std::function<int()> &errEventCallBack);
public:
    int HandleEvents(Net::Event events);
    
    int Update(Net::Event events);

    int Close();

    FileDesc fd() const;
    
    friend class EventLoop;
private:
    FileDesc fd_;
    Net::Event events_ = 0;
    Net::Event out_events_;
    EventLoop *loop_;
    std::function<int()> inEventCallBack_;
    std::function<int()> outEventCallBack_;
    std::function<int()> errEventCallBack_;
};

class EventLoop
{
public:
    EventLoop(int max_events = DEFAULT_MAX_EVENTS);

    ~EventLoop();

public:
    int add(EventsSource *evsSource);

    int mod(EventsSource *evsSource);

    int del(EventsSource *evsSource);

    int loopOnce(int timeout = 0);

private:
    int epollfd_ = -1;
    int eventsCnt_ = 0;
    const int maxEvents_;
    struct epoll_event *events_ = NULL;
    std::unordered_map<FileDesc, EventsSource*> fdToEventsSource_;
};

};  // end of namespace tcp
#endif