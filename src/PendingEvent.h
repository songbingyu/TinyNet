/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _PENDINGEVENT_H_
#define _PENDINGEVENT_H_

#include "Event.h"

class PendingEvent
{
public:
    PendingEvent( IEvent* ev , int evFlag ): event_(ev),eventFlag_( evFlag )
    {

    }

    ~PendingEvent()
    {

    }
public:
    void clear()
    {
        event_ = NULL:
        eventFlag_ = 0;
    }
public:
    IEvent*     event_;
    int         eventFlag_;

};



#endif

