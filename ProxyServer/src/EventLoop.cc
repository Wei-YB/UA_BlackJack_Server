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
#include "EventsSource.h"
#include "Timer.h"
#include "log.h"

using namespace Net;

EventLoop::EventLoop(int max_events, int healthReportPeriod) 
                    : maxEvents_(max_events)
{
    if ((epollfd_ = epoll_create(5)) < 0)
    {
        logger_ptr->error("In creating eventloop: fail to create epollfd.");
        throw std::exception();
    }
    if ((events_ = new struct epoll_event[maxEvents_]) == NULL)
    {
        logger_ptr->error("In creating eventloop: fail to allocate epoll event array.");
        close(epollfd_);
        throw std::exception();
    }

    timer_ = std::make_shared<Timer>(this, std::bind(&EventLoop::OnHealthReport, this));

    timer_->SetPeriod(healthReportPeriod);
}

EventLoop::~EventLoop()
{
    timer_->SetPeriod(0);
    delete[] events_;
    close(epollfd_);
}

int EventLoop::add(std::shared_ptr<EventsSource> evsSource)
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
    if (!fdToEventsSource_.emplace(evsSource->fd_, evsSource).second)
    {
        epoll_ctl(epollfd_, EPOLL_CTL_DEL, evsSource->fd_, NULL);
        return -1;
    }
    eventsCnt_++;
    logger_ptr->trace("In EventLoop::add(): Successfully add event (events: {0}) to source (fd: {1}).", evsSource->events_, evsSource->fd_);
    return 0;
}

int EventLoop::mod(std::shared_ptr<EventsSource> evsSource)
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
    logger_ptr->trace("In EventLoop::mod(): Successfully modify source (fd: {0}) with events ({1}).", evsSource->fd_, evsSource->events_);
    return 0;
}

int EventLoop::del(std::shared_ptr<EventsSource> evsSource)
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

int EventLoop::loopOnce(int timeout)
{
    if (fdToEventsSource_.size() == 0)
    {
        return -1;
    }
    
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
        std::shared_ptr<EventsSource> evsSource;
        if (fdToEventsSource_.find(sockfd) == fdToEventsSource_.end())
            continue;
        evsSource = fdToEventsSource_[sockfd];
        // if the return val is -1, it means we should remove this sockfd from poller
        if (0 > evsSource->HandleEvents(toNetEvent(events)))
        {
            del(evsSource);
        }
    }
    return nfds;
}

void EventLoop::OnHealthReport()
{
    logger_ptr->info("In EventLoop::OnHealthReport(): current sources in map: {0}, eventsCnt: {1}.", fdToEventsSource_.size(), eventsCnt_);
}