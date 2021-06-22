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

#include <exception>
#include <unordered_map>
#include <list>

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

class EventsHandler
{
public:
    EventsHandler() {}
    virtual ~EventsHandler() {}
    EventsHandler(const EventsHandler &) = delete;
    EventsHandler &operator=(const EventsHandler &) = delete;

public:
    virtual int addToEventLoop(EventLoop *loop) = 0;
    virtual int handleEvents(int sockfd, int events) = 0;
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
    EventLoop(int max_events = DEFAULT_MAX_EVENTS) : m_max_events(max_events)
    {
        if ((m_epollfd = epoll_create(5)) < 0)
        {
            throw "EventLoop: fail to create epoll.\n";
        }

        if ((m_events = new struct epoll_event[m_max_events]) == NULL)
        {
            close(m_epollfd);
            throw "EventLoop: fail to assign epoll event array.\n";        
        }
    }

    ~EventLoop() 
    {
        delete [] m_events;
        close(m_epollfd);
    }

public:
    int add(int sockfd, int events, EventsHandler *evshandler)
    {
        if (m_fd_to_handler.find(sockfd) != m_fd_to_handler.end() 
            || m_events_cnt >= m_max_events)
        {
            return -1;
        }

        struct epoll_event ev;
        ev.data.fd = sockfd;
        ev.events = events;
        if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sockfd, &ev) < 0)
        {
            return -1;
        }

        m_fd_to_handler.emplace(sockfd, evshandler);
        m_events_cnt++;
        return 0;
    }

    int mod(int sockfd, int events, EventsHandler *evshandler)
    {
        if (m_fd_to_handler.find(sockfd) == m_fd_to_handler.end() 
            || m_fd_to_handler[sockfd] != evshandler)
        {
            return -1;
        }

        struct epoll_event ev;
        ev.data.fd = sockfd;
        ev.events = events;

        if (epoll_ctl(m_epollfd, EPOLL_CTL_MOD, sockfd, &ev) < 0)
        {
            return -1;
        }

        return 0;
    }

    int del(int sockfd)
    {
        if (m_fd_to_handler.find(sockfd) != m_fd_to_handler.end())
        {
            epoll_ctl(m_epollfd, EPOLL_CTL_DEL, sockfd, NULL);
            m_fd_to_handler.erase(sockfd);
            m_events_cnt--;
            return 0;
        }
        
        return -1;
    }

    int loopOnce(int timeout = 0)
    {
        if (!m_events_cnt)
        {
            return -1;
        }
        // memset(m_events, 0, sizeof(struct epoll_event) * m_max_events);
        int nfds = epoll_wait(m_epollfd, m_events, m_max_events, timeout);
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
            int sockfd = m_events[i].data.fd;
            int events = m_events[i].events;   
            if (m_fd_to_handler.find(sockfd) == m_fd_to_handler.end())
            {
                continue;
            }

            // if the return val is -1, it means we should remove this sockfd from poller
            if (0 > m_fd_to_handler[sockfd]->handleEvents(sockfd, events))
            {
                del(sockfd);
            }
        }

        return nfds;
    }

private:
    int m_epollfd = -1;
    int m_events_cnt = 0;
    const int m_max_events;
    struct epoll_event *m_events = NULL;
    std::unordered_map<int, EventsHandler*> m_fd_to_handler;
};

};  // end of namespace tcp
#endif