#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <memory>

#include <exception>
#include <unordered_map>
#include <list>

#include "common.h"

#define DEFAULT_MAX_EVENTS  1024

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
                const std::function<int()> &errEventCallBack)
                : fd_(fd), loop_(loop),
                inEventCallBack_(inEventCallBack),
                outEventCallBack_(outEventCallBack),
                errEventCallBack_(errEventCallBack) {}
public:
    int HandleEvents(Net::Event events)
    {
        int ret = 0;
        if (events & Net::EV_IN)
        {
            ret = inEventCallBack_() == -1 ? -1 : ret; 
        }
        if (events & Net::EV_OUT)
        {
            ret = outEventCallBack_() == -1 ? -1 : ret; 
        }
        if (events & Net::EV_ERR)
        {
            ret = errEventCallBack_() == -1 ? -1 : ret; 
        }
        return ret;
    }
    
    int Update(Net::Event events)
    {
        if (events_ == events)
        {
            return 0;
        }
        events_ = events;
        return loop_->mod(*this);
    }

    FileDesc fd() const {return fd_;}
    friend class EventLoop;
private:
    FileDesc fd_;
    Net::Event events_;
    Net::Event out_events_;
    EventLoop *loop_;
    std::function<int()> inEventCallBack_;
    std::function<int()> outEventCallBack_;
    std::function<int()> errEventCallBack_;
};

class EventsHandler
{
public:
    EventsHandler() {}
    virtual ~EventsHandler() {}
    EventsHandler(const EventsHandler &) = delete;
    EventsHandler &operator=(const EventsHandler &) = delete;

public:
    virtual int addToEventLoop(EventLoop *loop) = 0;
    virtual int handleEvents(int sockfd, Event events) = 0;
    virtual int pushRpcRequest(void *call) {return 0;}
    virtual int pushRpcResponse(void *response) {return 0;}
};

// this template class provide functionalities of recycling expired handlers 
// and query for handlers by key.
// Note: the Key type parameter must be hashtable
template<typename Key>
class HandlerManager 
{
public:
    HandlerManager() {}
    ~HandlerManager() {}
    HandlerManager(const HandlerManager &) = delete;
    HandlerManager &operator=(const HandlerManager &) = delete;

public:
    int registerHandler(const Key &key, EventsHandler *handler)
    {
        if (m_handlers.find(key) == m_handlers.end())
        {
            m_handlers.emplace(key, handler);
            return 0;
        }
        return -1;
    }

    int expireHandler(const Key &key)
    {
        if (m_handlers.find(key) != m_handlers.end())
        {
            m_expiredHandler.push_back(m_handlers[key]);
            return 0;
        }
        return -1;
    }

    int cleanExpired()
    {
        int ret = 0;
        while (!m_expiredHandler.empty())
        {
            delete m_expiredHandler.front();
            m_expiredHandler.pop_front();
            ++ret;
        }
        return ret;
    }

    EventsHandler *find(const Key &key)
    {
        if (m_handlers.find(key) != m_handlers.end())
        {
            return m_handlers[key];
        }
        return NULL;
    }

private:
    std::unordered_map<Key, EventsHandler *> m_handlers;
    std::list<EventsHandler *> m_expiredHandler;
};

class EventLoop
{
public:
    EventLoop(int max_events = DEFAULT_MAX_EVENTS) : maxEvents_(max_events)
    {
        if ((epollfd_ = epoll_create(5)) < 0)
        {
            throw "EventLoop: fail to create epoll.\n";
        }

        if ((events_ = new struct epoll_event[maxEvents_]) == NULL)
        {
            close(epollfd_);
            throw "EventLoop: fail to assign epoll event array.\n";        
        }
    }

    ~EventLoop() 
    {
        delete [] events_;
        close(epollfd_);
    }

public:
    int add(const EventsSource &evsSource)
    {
        if (fdToEventsSource_.find(evsSource.fd_) != fdToEventsSource_.end() 
            || eventsCnt_ >= maxEvents_)
        {
            return -1;
        }

        struct epoll_event ev;
        ev.data.fd = evsSource.fd_;
        ev.events = toEpollEvent(evsSource.events_);
        if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, ev.data.fd, &ev) < 0)
        {
            return -1;
        }

        fdToEventsSource_.emplace(ev.data.fd, evsSource);
        eventsCnt_++;
        return 0;
    }

    int mod(const EventsSource &evsSource)
    {
        if (fdToEventsSource_.find(evsSource.fd_) != fdToEventsSource_.end())
        {
            return -1;
        }

        struct epoll_event ev;
        ev.data.fd = evsSource.fd_;
        ev.events = toEpollEvent(evsSource.events_);

        if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, ev.data.fd, &ev) < 0)
        {
            return -1;
        }

        return 0;
    }

    int del(const EventsSource &evsSource)
    {
        if (fdToEventsSource_.find(evsSource.fd_) != fdToEventsSource_.end())
        {
            epoll_ctl(epollfd_, EPOLL_CTL_DEL, evsSource.fd_, NULL);
            fdToEventsSource_.erase(evsSource.fd_);
            eventsCnt_--;
            return 0;
        }
        
        return -1;
    }

    int loopOnce(int timeout = 0)
    {
        if (!eventsCnt_)
        {
            return -1;
        }
        // memset(m_events, 0, sizeof(struct epoll_event) * m_max_events);
        int nfds = epoll_wait(epollfd_, events_, maxEvents_, timeout);
        if (nfds < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
            {
                return 0;
            }
            return -1;
        }

        for (int i = 0; i < nfds; ++i)
        {
            int sockfd = events_[i].data.fd;
            int events = events_[i].events;   
            if (fdToEventsSource_.find(sockfd) == fdToEventsSource_.end())
            {
                continue;
            }

            // if the return val is -1, it means we should remove this sockfd from poller
            if (0 > fdToEventsSource_[sockfd].HandleEvents(toNetEvent(events)))
            {
                del(fdToEventsSource_[sockfd]);
            }
        }

        return nfds;
    }

private:
    int epollfd_ = -1;
    int eventsCnt_ = 0;
    const int maxEvents_;
    struct epoll_event *events_ = NULL;
    std::unordered_map<FileDesc, EventsSource> fdToEventsSource_;
};

};  // end of namespace tcp
#endif