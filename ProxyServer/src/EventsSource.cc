#include <unistd.h>
#include "EventsSource.h"
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

EventsSource::~EventsSource() {::close(fd_);}

int EventsSource::HandleEvents(Event events)
{
    if (events & (EV_ERR | EV_HUP))
    {
        logger_ptr->warn("In EventsSource::HandleEvents(): fd: {} has error", fd_);
        if (errEventCallBack_) errEventCallBack_();
        return -1;
    }
    if ((events & EV_IN) && (events & EV_RDHUP))
    {
        logger_ptr->trace("In EventsSource::HandleEvents(): peer (fd: {}) sent its last data and shutdown", fd_);
        if (inEventCallBack_) inEventCallBack_();
        return -1;
    }
    if (events & EV_IN)
    {
        logger_ptr->trace("In EventsSource::HandleEvents(): input event from fd: {}", fd_);
        if (inEventCallBack_ && inEventCallBack_() < 0)
            return -1;
    }
    if (events & EV_OUT)
    {
        logger_ptr->trace("In EventsSource::HandleEvents(): onput event from fd: {}", fd_);
        if (outEventCallBack_ && outEventCallBack_() < 0)
            return -1;
    }
    
    return 0;
}

int EventsSource::EnableET()
{
    if (events_ & EV_ET)
        return 0;
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
    if ((events_ & EV_OUT) == 0)
        return 0;
    events_ &= ~EV_OUT;
    return loop_->mod(shared_from_this());
}

int EventsSource::DisableRead()
{
    if ((events_ & EV_IN) == 0)
        return 0;
    events_ &= ~EV_IN;
    return loop_->mod(shared_from_this());
}

int EventsSource::RemoveFromLoop()
{
    return loop_->del(shared_from_this());
}