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
#include "log.h"

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
    int ret;
    if (events & EV_HUP || events & EV_ERR)
    {   // logger_ptr->warn("fatal error happens in event source (fd: {}), close it now.", fd_);
        if (closeEventCallBack_) 
        {
            closeEventCallBack_();
        }
        return -1;
    }
    if (events & EV_IN)
    {
        logger_ptr->info("In main thread: On input event from fd: {}", fd_);
        ret = inEventCallBack_() == -1 ? -1 : ret; 
    }
    if (events & EV_OUT)
    {
        logger_ptr->info("In main thread: On onput event from fd: {}", fd_);
        ret = outEventCallBack_() == -1 ? -1 : ret; 
    }
    
    return 0;
}

int EventsSource::EnableWrite()
{
    int ret;
    if (events_ & EV_OUT)
    {
        logger_ptr->info("In main thread: On error event from fd: {}", fd_);
        ret = errEventCallBack_() == -1 ? -1 : ret; 
    }
    events_ |= EV_OUT | EV_ET;
    return loop_->mod(share_from_this());
}

int EventsSource::EnableRead()
{
    if (events_ & EV_IN)
    {
        return 0;
    }
    events_ |= EV_IN | EV_ET;
    return loop_->mod(share_from_this());
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

int Net::EventLoop::add(std::shared_ptr<EventsSource> evsSource)
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
    logger_ptr->info("In main thread: Successfully add event source.");
    return 0;
}

int Net::EventLoop::mod(std::shared_ptr<EventsSource> evsSource)
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
    logger_ptr->info("In main thread: Successfully modify events.");
    return 0;
}

int Net::EventLoop::del(std::shared_ptr<EventsSource> evsSource)
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
