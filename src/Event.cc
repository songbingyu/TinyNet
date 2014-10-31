/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include "Event.h"
#include <signal.h>
#include "SignalHelper.h"
#include "EventLoop.h"

void IEvent:: onEvent( int revents )
{
    if( expect_true( cb_ != NULL ) ) {
        (*cb_)( loop_, this,  revents);
    }
}

void IEvent::ev_start( int active )
{
    active_ = active;
    loop_->addActiveCnt();

}

void IEvent::ev_stop()
{
    active_ = 0;
    loop_->delActiveCnt();
}


void EventIo::start()
{
    if (expect_false( isActive() ) ){
        return;
    }

    tiny_assert( fd_ > 0 );
    tiny_assert( !(events_&~(EV_IOFDSET | EV_READ | EV_WRITE )));

    ev_start(1);
    loop_->addActiveFdEvent( this );
}

void EventIo::stop()
{
    loop_->delPendingEvent( this );

    if ( expect_false( !isActive() ) ) {
        return ;
    }

    tiny_assert( fd_ > 0 );

    loop_->delActiveFdEvent( this );

    ev_stop();
}


void EventTimer::start()
{
    if( isActive() ) {
        return;
    }

    at_ += loop_->getNowTime();

    tiny_assert( repeat_ >= 0 );

    ev_start( loop_->getTimerCount() + 1 );

    loop_->addTimer( this );

}

void EventTimer::stop()
{
    loop_->delPendingEvent( this );
    if( expect_false( !isActive() )){
        return;
    }
    loop_->delTimer( this );
    ev_stop();
}

void EventSignal::start()
{
    if( isActive() ){
        return;
    }

    tiny_assert( sigNum_ > 0 );
    ev_start(1);

    loop_->addSignalEvent( this );

    struct sigaction sa;

    sa.sa_handler = Tiny::sigHandle;
    sigfillset( &sa.sa_mask );

    sa.sa_flags = SA_RESTART;

    sigaction( sigNum_, &sa, 0 );

}

void EventSignal::stop()
{
    loop_->delPendingEvent( (IEvent*)this );
    if( expect_false( !isActive() ) ){
        return;
    }

    loop_->delSignalEvent( this );

    ev_stop();

    signal( sigNum_, SIG_DFL );
}


