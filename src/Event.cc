/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include "Event.h"
#include "EventLoop.h"

void IEvent:: onEvent( EventLoop* loop, int revents )
{
    if( expect_true( cb_ != NULL ) ) {
        (*cb_)( revents);
    }
}

void IEvent::ev_start( EventLoop* loop, int active )
{
    active_ = active;
    loop->addActiveCnt();

}

void IEvent::ev_stop( EventLoop* loop )
{
    active_ = 0;
    loop->delActiveCnt();
}


void EventIo::start( EventLoop* loop )
{
    if (expect_false( isActive() ) ){
        return;
    }

    tiny_assert( fd_ > 0 );
    tiny_assert( !(events_&~(EV_IOFDSET | EV_READ | EV_WRITE )));

    ev_start( loop, 1 );
    loop->addActiveFdEvent( this );
}

void EventIo::stop(EventLoop* loop )
{
    loop->delPendingEvent( this );

    if ( expect_false( !isActive() ) ) {
        return ;
    }

    tiny_assert( fd_ > 0 );

    loop->delActiveFdEvent( this );

    ev_stop( loop );
}


void EventTimer::start( EventLoop* loop )
{
    if( isActive() ) {
        return;
    }

    at_ += loop->getNowTime();

    tiny_assert( repeat_ >= 0 );

    ev_start( loop, loop->getTimerCount() + 1 );

    loop->addTimer( this );

}

void EventTimer::stop( EventLoop* loop )
{
    loop->delPendingEvent( this );
    if( expect_false( !isActive() )){
        return;
    }
    loop->delTimer( this );
    ev_stop( loop );
}



