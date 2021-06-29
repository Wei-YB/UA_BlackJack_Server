#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <exception>
#include <unordered_map>
#include <functional>

#include "EventLoop.h"

using namespace Net;

EventsSource::EventsSource(FileDesc fd, EventLoop *loop,
                        const std::function<int()> &inEventCallBack,
                        const std::function<int()> &outEventCallBack,
                        const std::function<int()> &errEventCallBack,
                        const std::function<int()> &closeEventCallBack)
                        : fd_(fd), loop_(loop),
                        inEventCallBack_(inEventCallBack),
                        outEventCallBack_(outEventCallBack),
                        errEventCallBack_(errEventCallBack),
                        closeEventCallBack_(closeEventCallBack) {}

int EventsSource::HandleEvents(Event events)
{
    if (events & EV_HUP || events & EV_ERR)
    {   // logger_ptr->warn("fatal error happens in event source (fd: {}), close it now.", fd_);
        if (closeEventCallBack_) 
        {
            closeEventCallBack_();
        }
        return -1;
    }
    if (events & EV_IN)
    {   // logger_ptr->info("input event from event source (fd: {}).", fd_);
        if (inEventCallBack_ && inEventCallBack_() < 0)
        {
            return -1;
        }
    }
    if (events & EV_RDHUP)
    {   // logger_ptr->info("peer-shut-down event from event source (fd: {}).", fd_);
        if (closeEventCallBack_)
        {
            closeEventCallBack_()
        }
        return -1;
    }
    if (events & EV_OUT)
    {   // logger_ptr->info("output event from event source (fd: {}).", fd_);
        if (outEventCallBack_ && outEventCallBack_() < 0)
        {
            return -1;
        } 
    }
    
    return 0;
}

int EventsSource::EnableWrite()
{
    if (events_ & EV_OUT)
    {
        return 0;
    }
    events_ |= EV_OUT | EV_ET;
    return loop->mod(share_from_this());
}

int EventsSource::EnableRead()
{
    if (events_ & EV_IN)
    {
        return 0;
    }
    events_ |= EV_IN | EV_ET;
    return loop->mod(share_from_this());
}

int EventsSource::DisableWrite()
{
    if (events_ & EV_OUT)
    {
        events_ &= ~EV_OUT;
        return loop->mod(share_from_this());
    }
    return 0;
}

int EventsSource::DisableRead()
{
    if (events_ & EV_IN)
    {
        events_ &= ~EV_IN;
        return loop->mod(share_from_this());
    }
    return 0;
}

int EventsSource::SetNonBlocking(bool flag)
{
    if (flag)
        return fcntl(fd_, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    else
        return fcntl(fd_, F_SETFL, fcntl(fd, F_GETFL) & (~O_NONBLOCK));
}

int EventsSource::Close()
{
    ::close(fd_);
    return loop_->del(share_from_this());
}

FileDesc EventsSource::fd() const 
{
    return fd_;
}

EventLoop::EventLoop(int max_events) : maxEvents_(max_events)
{
    if ((epollfd_ = epoll_create(5)) < 0)
    {
        // logger_ptr->error("fail to create epollfd.");
    }
    if ((events_ = new struct epoll_event[maxEvents_]) == NULL)
    {
        close(epollfd_);
        // logger_ptr->error("fail to assign epoll event array.");
        throw "EventLoop: fail to assign epoll event array.\n";        
    }
}

Net::EventLoop::~EventLoop() 
{
    delete [] events_;
    close(epollfd_);
}

int Net::EventLoop::add(EventsSource *evsSource)
{
    if (fdToEventsSource_.find(evsSource->fd_) != fdToEventsSource_.end() 
        || eventsCnt_ >= maxEvents_)
    {
        return -1;
    }
    struct epoll_event ev;
    ev.data.fd = evsSource->fd_;
    ev.events = toEpollEvent(evsSource->events_);
    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, ev.data.fd, &ev) < 0)
    {
        return -1;
    }
    fdToEventsSource_.emplace(evsSource->fd_, evsSource);
    eventsCnt_++;
    return 0;
}

int Net::EventLoop::mod(EventsSource *evsSource)
{
    if (fdToEventsSource_.find(evsSource->fd_) == fdToEventsSource_.end())
    {
        return add(evsSource);
    }
    struct epoll_event ev;
    ev.data.fd = evsSource->fd_;
    ev.events = toEpollEvent(evsSource->events_);
    if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, ev.data.fd, &ev) < 0)
    {
        return -1;
    }
    return 0;
}

int Net::EventLoop::del(EventsSource *evsSource)
{
    if (fdToEventsSource_.find(evsSource->fd_) != fdToEventsSource_.end())
    {
        epoll_ctl(epollfd_, EPOLL_CTL_DEL, evsSource->fd_, NULL);
        fdToEventsSource_.erase(evsSource->fd_);
        eventsCnt_--;
        return 0;
    }
    
    return -1;
}

int Net::EventLoop::loopOnce(int timeout)
{
    if (fdToEventsSource_.size() == 0)
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
        if (0 > fdToEventsSource_[sockfd]->HandleEvents(toNetEvent(events)))
        {
            del(fdToEventsSource_[sockfd]);
        }
    }
    return nfds;
}
