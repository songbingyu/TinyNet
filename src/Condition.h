/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _CONDITION_H_
#define _CONDITION_H_

#include "nocopyable.h"
#include "Mutex.h"

class Condition {
public:
    explicit Condition(MutexLock& mutex) : mutex_(mutex) {
        pthread_cond_init(&cond_, NULL);
    }

    ~Condition() {
        pthread_cond_destory(&cond_);
    }

    void wait() {
        pthread_cond_wait(&cond_, mutex_.getMutex());
    }

    void notify() {
        pthread_cond_signal(&cond_);
    }

    void notifyAll() {
        pthread_cond_broadcast(&cond_);
    }

private:
    MutexLock&      mutex_;
    pthread_cond_t  cond_;
};

#endif // _CONDITION_H_


