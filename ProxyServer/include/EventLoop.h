#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <memory>
#include <unordered_map>
#include "common.h"

#define DEFAULT_MAX_EVENTS  65535
#define DEFAULT_HEALTH_REPORT_PERIOD    10   // 5s

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

class EventsSource;
class Timer;

class EventLoop
{
public:
    EventLoop(int max_events = DEFAULT_MAX_EVENTS, int healthReportPeriod = DEFAULT_HEALTH_REPORT_PERIOD);

    ~EventLoop();

public:
    int add(std::shared_ptr<EventsSource> evsSource);

    int mod(std::shared_ptr<EventsSource> evsSource);

    int del(std::shared_ptr<EventsSource> evsSource);

    int loopOnce(int timeout = 0);

private:
    void OnHealthReport();

private:
    int epollfd_ = -1;
    int eventsCnt_ = 0;
    const int maxEvents_;
    struct epoll_event *events_ = NULL;
    std::unordered_map<FileDesc, std::shared_ptr<EventsSource>> fdToEventsSource_;
    std::shared_ptr<Timer> timer_;
};

};  // end of namespace tcp
#endif