#ifndef MUTEX_H
#define MUTEX_H

#include <assert.h>
#include <pthread.h>

class MutexLock {
public:
    MutexLock() : holder_(0) { pthread_mutex_init(&mutex_, nullptr); }
    ~MutexLock() {
        assert(holder_ == 0);
        pthread_mutex_destroy(&mutex_);
    }

    void lock() { pthread_mutex_lock(&mutex_); }

private:
    pthread_mutex_t mutex_;
    pid_t holder_;
}

class MutexLockGuard {
public:
    explicit MutexLockGuard(MutexLock& mutex)
}

#endif