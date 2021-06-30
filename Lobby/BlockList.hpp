#pragma once

#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <list>
#include <spdlog/spdlog.h>


namespace ua_blackjack {

template <typename T>
class BlockList {
public:
    BlockList() : event_fd_(::eventfd(0, EFD_SEMAPHORE)), data_list_(), mutex_() {
        if (event_fd_ < 0) {
            spdlog::error("event fd create error, errno is {0}", errno);
        }
    }

    ~BlockList() {
        ::close(event_fd_);
    }

    [[nodiscard]] int fd() const { return event_fd_; }

    epoll_event GetEpollEvent() const {
        epoll_event event;
        bzero(&event, sizeof event);
        event.events  = EPOLLIN;
        event.data.fd = fd();
        return event;
    }

    //PushBack将激活在event_fd_上等待的epoll_wait
    void PushBack(T&& call_data) {
        SPDLOG_TRACE("new element push into block list");
        {
            std::lock_guard<std::mutex> lock_guard(mutex_);
            data_list_.emplace_back(std::move(call_data));
        }
        int64_t val = 1;
        write(event_fd_, &val, sizeof val);
    }

    void PushBack(const T& call_data) {
        SPDLOG_TRACE("new element push into block list");
        {
            std::lock_guard<std::mutex> lock_guard(mutex_);
            data_list_.emplace_back(call_data);
        }
        int64_t val = 1;
        write(event_fd_, &val, sizeof val);
    }

    T PopFront() {
        T cur{};
        {
            std::lock_guard<std::mutex> lock_guard(mutex_);
            if (data_list_.empty()) {
                spdlog::error("try to read empty list");
                return cur;
            }
            cur = data_list_.front();
            data_list_.pop_front();
        }
        return cur;
    }

private:
    int          event_fd_;
    std::list<T> data_list_;
    std::mutex   mutex_;
};
}
