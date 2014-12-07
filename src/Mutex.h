/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _MUTEX_H_
#define _MUTEX_H_


#include <pthread.h>
#include "nocopyable.h"

class MutexLock : public nocopyable {
public:
    MutexLock()  { pthread_mutex_init(&mutex_, NULL); }
    ~MutexLock() { pthread_mutex_destory(&mutex_);    }
    void lock() {
        pthread_mutex_lock(&mutex_);
    }
    void unlock() {
        pthread_mutex_unlock(&mutex_);
    }
    pthread_mutex_t* getMutex() { return &mutex_; }
private:
    pthread_mutex_t mutex_;
    //pid_t           holder_;
};


class MutexLockGuard : public nocopyable {
public:
    explicit MutexLockGuard(MutexLock& mutex ) : mutex_(mutex) {
        mutex_.lock();
    }
    ~MutexLockGuard() {
        mutex_.unlock();
    }
private:
    MutexLock& mutex_;
};

#define MutexLockGuard(x) static_assert(false,"missing mutex var name");

#endif //_MUTEX__H_


