/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _TINY_DEFINE_H_
#define _TINY_DEFINE_H_

#include<sys/time.h>
#include<sys/select.h>
#include<vector>


#define expect_true(x)      __builtin_expect((x),1)
#define expect_false(x)     __builtin_expect((x),0)

#define tiny_attribute(attrlist)      __attribute__(attrlist)


#define tiny_forceinline    tiny_attribute((__alwayes_inline__))
#define tiny_noinline       tiny_attribute((__noinline__))

#define tiny_pure           tiny_attribute((__pure__))
#define tiny_unused         tiny_attribute((__unused__))

#define tiny_hot            tiny_attribute((__hot__))
#define tiny_cold           tiny_attribute((__cold__))

typedef     double          Timestamp;

typedef     TimerEventList  TimerHeap;
typedef     std::vector< TimerHeap* >  HeapVec;

typedef tiny_assert( expr )     assert( expr )

#define MIN_TIMEJUMP    1.
#define MAX_BLOCKTIME  59.743



enum EventState
{
    ES_New,
    ES_Added,
    ES_Del,
};

const static int kHeap0 = 1;

Timestamp  tinyGetTime()
{
    struct timeval tv;
    gettimeofday( &tv, 0 );
    return tv.tvsec + tv.tvusec*1e-9;
}

void tinySleep( Timestamp delay )
{
    struct timeval tv;
    tv.tv_sec = (long)t;
    tv.tv_usec=(long)( ( t - tv.tv_sec ) * 1e6 );
    select( 0, 0, 0, 0, &tv );
}

#endif








