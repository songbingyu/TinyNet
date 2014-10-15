/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _ACTIVE_EVENT_H_
#define _ACTIVE_EVENT_H_

#include "TinyDefine.h"
#include "Event.h"
#include "EventLoop.h"


class ActiveFdEvent
{
public:
    ActiveFdEvent(): head_(NULL), events_(0), refiy_(0), revents_(0), egen_(0)
    {

    }
    ~ActiveFdEvent()
    {

    }
public:
    void addList( EventIo* ev )
    {
        head_ = ev;
        *(ev->getNext()) = ev;
    }

    void delList( EventIo* ev )
    {
        EventList** head = ( EventList**)&head_;
        while( *head ) {

            if( expect_true( *head == ev ) ){
                *head = *ev->getNext();
            }

            head =(*head)->getNext();
        }
    }

public:

    tiny_forceinline  void  tiny_cold  killFd( EventLoop* loop )
    {
        EventIo* ev = NULL;
        while( (ev = head_ ) ){
            ev->stop( loop );
            loop->addPendingEvent( (IEvent*)ev, EV_ERROR | EV_WRITE | EV_READ );
        }

    }

    tiny_forceinline void fdEvent( EventLoop* loop, int revents  )
    {

    }
public:
    EventIo*        head_;
    int             events_;
    int             refiy_;
    int             revents_;
    unsigned int    egen_;
};


#endif






