/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _TINY_DEFINE_H_
#define _TINY_DEFINE_H_

#include<sys/time.h>
#include<sys/select.h>
#include<assert.h>
#include<vector>


#define expect_true(x)      __builtin_expect((x),1)
#define expect_false(x)     __builtin_expect((x),0)

#define tiny_attribute(attrlist)      __attribute__(attrlist)

#ifdef _TINY_OP_

#define tiny_forceinline    tiny_attribute((always_inline))
#define tiny_noinline       tiny_attribute((__noinline__))

#define tiny_pure           tiny_attribute((__pure__))
#define tiny_unused         tiny_attribute((__unused__))

#define tiny_hot            tiny_attribute((__hot__))
#define tiny_cold           tiny_attribute((__cold__))

#else

#define tiny_forceinline
#define tiny_noinline

#define tiny_pure
#define tiny_unused

#define tiny_hot
#define tiny_cold



#endif



typedef     double          Timestamp;

enum  EventIOType
{
    kIOSelect,
    kIOPoll,
    kIOEpoll,
    kIOKQueue,
    // cur just 4 , consider add  devpoll port
};

enum
{
    EV_NO       =   0x00000000,
    EV_READ     =   0x00000001,
    EV_WRITE    =   0x00000002,
    EV_IOFDSET  =   0x00000080, //only interal use
    EV_TIMER    =   0x00000100,
    EV_PERIODIC =   0x00000200,
    EV_SIGNAL   =   0x00000400,
    EV_IDLE     =   0x00001000,
    EV_ERROR    =   0x80000000,
};




#define  tiny_assert( expr )     assert( expr )
#define  TINY_DELETE( v )  { delete v; v = NULL; }

#define MIN_TIMEJUMP    1.
#define MAX_BLOCKTIME  59.743



enum EventState
{
    ES_New,
    ES_Added,
    ES_Del,
};

enum EventRunFlag
{
    EVRUN_ALWAYES,
    EVRUN_NOWAIT,
    EVRUN_ONCE,
};

const static int kHeap0 = 1;

extern Timestamp  tinyGetTime();
extern void tinySleep( Timestamp t );
extern void fdInteral( int fd );
#endif










