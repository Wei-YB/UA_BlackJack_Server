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
      closeEventCallBack_(closeEventCallBack)
{
    // fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

int EventsSource::HandleEvents(Event events)
{
    if (events & (EV_ERR | EV_HUP))
    {
        // logger_ptr->info("In main thread: On error event from fd: {}", fd_);
        if (errEventCallBack_) errEventCallBack_();
        return -1;
    }
    if ((events & EV_IN) && (events & EV_RDHUP))
    {
        // logger_ptr->info("In main thread: On input event from fd: {}", fd_);
        if (inEventCallBack_) inEventCallBack_();
        return -1;
    }
    if (events & EV_IN)
    {
        // logger_ptr->info("In main thread: On input event from fd: {}", fd_);
        if (inEventCallBack_) 
            if (inEventCallBack_() < 0) 
            {
                // logger_ptr->info("In main thread: just to prove evsSource (fd: {}) is not dead.", fd_);
                return -1;
            }
    }
    if (events & EV_OUT)
    {
        // logger_ptr->info("In main thread: On onput event from fd: {}", fd_);
        if (outEventCallBack_) 
            if (outEventCallBack_() < 0)
                return -1;
    }
    
    return 0;
}

int EventsSource::EnableET()
{
    events_ |= EV_ET;
    return loop_->mod(shared_from_this());
}

int EventsSource::EnableWrite()
{
    events_ |= EV_OUT;
    return loop_->mod(shared_from_this());
}

int EventsSource::EnableRead()
{
    events_ |= EV_IN;
    return loop_->mod(shared_from_this());
}

int EventsSource::DisableWrite()
{
    events_ &= ~EV_OUT;
    return loop_->mod(shared_from_this());
}

int EventsSource::DisableRead()
{
    events_ &= ~EV_IN;
    return loop_->mod(shared_from_this());
}

int EventsSource::RemoveFromLoop()
{
    return loop_->del(shared_from_this());
}

EventLoop::EventLoop(int max_events) : maxEvents_(max_events)
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

EventLoop::~EventLoop()
{
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
    // logger_ptr->info("In main thread: Successfully add event source (fd: {}).", evsSource->fd_);
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
    // logger_ptr->info("In main thread: Successfully modify eventsSource (fd: {}).", evsSource->fd_);
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

int Net::EventLoop::loopOnce(int timeout)
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
