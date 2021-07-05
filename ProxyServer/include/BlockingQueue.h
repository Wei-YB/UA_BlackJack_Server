#ifndef _BLOCKINGQUEUE_H_
#define _BLOCKINGQUEUE_H_

#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <functional>

typedef std::chrono::milliseconds MilliSeconds;

template<typename T>
class BlockingQueue
{
private:
    std::condition_variable con_;
    std::mutex lock_;
    std::queue<T> queue_;
    std::function<bool()> pred_;
public:
    BlockingQueue()
    {
        pred_ = std::bind(&BlockingQueue<T>::NotEmpty, this);
    }

    bool NotEmpty() const {return !queue_.empty();}

    bool Pop(T &res, int timeout)
    {
        std::unique_lock<std::mutex> lock(lock_);
        con_.wait_for(lock, MilliSeconds(timeout), pred_);
        if (queue_.empty())
        {
            return false;
        }
        res = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool Push(T &item)
    {
        std::lock_guard<std::mutex> guard(lock_);
        queue_.push(item);   
        con_.notify_one();
        return true;
    }

};

#endif
