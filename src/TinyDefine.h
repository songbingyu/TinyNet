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


#define  tiny_assert( expr )     assert( expr )

#define MIN_TIMEJUMP    1.
#define MAX_BLOCKTIME  59.743



enum EventState
{
    ES_New,
    ES_Added,
    ES_Del,
};

const static int kHeap0 = 1;

extern Timestamp  tinyGetTime();
extern void tinySleep( Timestamp t );

#endif










