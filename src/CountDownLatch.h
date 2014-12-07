/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _COUNT_DOWN_LATCH_H_
#define _COUNT_DOWN_LATCH_H_

#include "Mutex.h""
#include "Condition.h"

class CountDownLatch : nocopyable {
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
    int  getCount();
private:
    mutable MutexLock mutex_;
    Condition   cond_;
    int count_;

};

#endif //_COUNT_DOWN_LATCH_H_

