#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <memory>
#include <unordered_map>
#include <memory>
#include "common.h"

#define DEFAULT_MAX_EVENTS  25000

namespace Net {

typedef int Event;

static const Event EV_NULL = 0x00;
static const Event EV_IN = 0x01;
static const Event EV_OUT = 0x02;
static const Event EV_ET = 0x04;
static const Event EV_RDHUP = 0x08;
static const Event EV_HUP = 0x10;
static const Event EV_ERR = 0x20;

inline int toEpollEvent(Event events)
{
    int epEv = 0;
    epEv |= events & EV_IN ? EPOLLIN : 0x0;
    epEv |= events & EV_OUT ? EPOLLOUT : 0x0;
    epEv |= events & EV_ERR ? EPOLLERR : 0x0;
    epEv |= events & EV_HUP ? EPOLLHUP : 0x0;
    epEv |= events & EV_RDHUP ? EPOLLRDHUP : 0x0;
    epEv |= events & EV_ET ? EPOLLET : 0x0;
    return epEv;
}

inline Event toNetEvent(int epEv)
{
    int events = EV_NULL;
    events |= epEv & EPOLLIN ? EV_IN : EV_NULL;
    events |= epEv & EPOLLOUT ? EV_OUT : EV_NULL;
    events |= epEv & EPOLLERR ? EV_ERR : EV_NULL;
    events |= epEv & EPOLLHUP ? EV_HUP : EV_NULL;
    events |= epEv & EPOLLRDHUP ? EV_RDHUP : EV_NULL;
    events |= epEv & EPOLLET ? EV_ET : EV_NULL;
    return events;
}

class EventLoop;

class EventsSource : public std::enable_shared_from_this<EventsSource>
{
public:
    EventsSource(FileDesc fd, EventLoop *loop,
                const std::function<int()> &inEventCallBack,
                const std::function<int()> &outEventCallBack,
                const std::function<int()> &errEventCallBack,
                const std::function<int()> &closeEventCallBack);
    
    ~EventsSource() {::close(fd_);}

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
    int HandleEvents(Net::Event events);

private:
    FileDesc fd_;
    Event events_ = 0;
    EventLoop *loop_;
    std::function<int()> inEventCallBack_;
    std::function<int()> outEventCallBack_;
    std::function<int()> errEventCallBack_;
    std::function<int()> closeEventCallBack_;
};

class EventLoop
{
public:
    EventLoop(int max_events = DEFAULT_MAX_EVENTS);

    ~EventLoop();

public:
    int add(std::shared_ptr<EventsSource> evsSource);

    int mod(std::shared_ptr<EventsSource> evsSource);

    int del(std::shared_ptr<EventsSource> evsSource);

    int loopOnce(int timeout = 0);

private:
    int epollfd_ = -1;
    int eventsCnt_ = 0;
    const int maxEvents_;
    struct epoll_event *events_ = NULL;
    std::unordered_map<FileDesc, std::shared_ptr<EventsSource>> fdToEventsSource_;
};

};  // end of namespace tcp
#endif