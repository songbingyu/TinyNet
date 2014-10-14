/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include "EventLoop.h"
#include <sys/epoll.h>
#include "Log.h"
#include "ActiveEvent.h"
//#include "IConnection.h"

EventLoop::EventLoop(): isRuning_( true ), curPid_(0)
{
    poller_ = getRecommendedPoller();

    timers_.push_back( NULL );
}

EventLoop::~EventLoop()
{
    delete  poller_;
}

int  EventLoop::run()
{
    do {
        if( expect_false(curPid_) ) {
            curPid_ = getpid();
        }

        fdReify();

        //time
        {
            Timestamp   waitTime = 0.
            Timestamp   prevNowTime =  curTime_;

            updateTime( 1e100 );

            if( expect_true( getTimerCount() > 0 ) ) {
                waitEvent = MAX_BLOCKTIME;

                if( !activeFdEvents_.empty() )
                {
                    Timestamp to = timers_[kHeap0]->getAt() -  curTime_;
                    if( waitTime > to )  waitTime = to;
                }

                if( expect_false( waitTime < 0. ) ){
                    waitTime = 0.
                }

                if( expect_false( waitTime < poller_->getMinWaitTime() ) ){
                    waitTime = poller_->getMinWaitTime();
                }

                if( expect_false( ioBlockTime_ ) ) {
                    Timestamp sleepTime= ioBlockTime_ -( curTime_ - prevNowTime );
                    if( sleepTime > waitTime - poller_->getMinWaitTime() ){
                        sleepTime = waitTime = poller_->getMinWaitTime();
                    }

                    if( expect_true( sleepTime > 0. ) ){
                       tinySleep(sleepTime);
                       waitTime -= sleepTime;
                    }
                }

            }

            poller_->updateEvent( waitTime );

            updateTime( waitTime + sleepTime );





        }
    }while( isRuning_ );

    while( isRuning_ ){

        activeConnections_.clear();
        //Fixme: what time ?
        eventEpoll_.waitEvent( 10, &activeConnections_ );

        ConnectionVec::iterator it = activeConnections_.begin();

        for( ; it != activeConnections_.end(); ++it )
        {
            IConnection* conn = *it;
            if( NULL != conn )
            {
                // begin
                int revents = conn->getReadEvents();
                if( ( revents & EPOLLHUP) && !( revents& EPOLLIN )  )
                {
                     conn->onClose();
                }

                if( revents & ( EPOLLERR ) )
                {
                    conn->onError();
                }

                if( revents & ( EPOLLIN | EPOLLPRI | EPOLLRDHUP ))
                {
                    conn->onRead();
                }

                if( revents &  EPOLLOUT )
                {
                    conn->onWrite();
                }

            }
            else
            {
                //TODO should do what?
                LOG_ERROR( " connection is null " );
            }

        }
    }

    return 1;

}

IPoller* EventLoop::getRecommendedPoller()
{
    //select  ......
    return  new EventEpoll(this);
}

void EventLoop::addPendingEvent( IEvent* ev, int evFlag )
{
    if( expect_false( ev->getPending()) ) {
        pendingEvents_[ ev->getPending()-1 ].eventFlag_ | = evFlag;
    } else {
        PendingEvent* pe  = new PendingEvent( ev, evFlag );
        pendingEvents_.push_back( pe )
        ev->setPending( pendingEvents_.size() );
    }

}

void EventLoop::delPendingEvent( IEvent* ev )
{
    if( ev->getPending() ) {
        pendingEvents_[ev->getPending()-1].clear();
        ev->setPending(0);
    }
}

void EventLoop::addActiveFdEvent(EventIo* ev )
{
    int  cnt = ev->getFd() + 1;
    if( cnt > activeFdEvents_.size() ) {
        activeFdEvent_.resize( cnt, new ActiveFdEvent() );
    }

    ActiveFdEvent* activeEv = activeFdEvents_[ ev->getFd() ];

    activeEv->addList( ev );

    addChangeFd( ev->getFd(), ev->getEvents()&EV_IOFDSET );

    ev->setEvents( ev->getEvents()&~EV_IOFDSET );

}

void EventLoop::delActiveFdEvent( EventIo* ev )
{
    ActiveFdEventI activeEv = activeFdEvents_[ ev->getFd() ];
    activeEv->delList( ev );
    addChangeFd( ev->getFd(),EV_NO );
}

void EventLoop::addTimer( EventTimer* ev )
{
   timers_[ ev->getActive() ] = ( TimerEventList* )ev;

}

void  EventLoop::delTimer( EventTimer* ev )
{

}

void EventLoop::addChangetFd( int fd, int flag )
{
    int refiy = activeFdEvents_[fd].refiy_;

    activeFdEvent_[fd].refiy_ |= flag;

    if( expect_true( !refiy ) ) {
        changeFds_.push_back( fd )
    }
}

void EventLoop::fdReify()
{
    int size = changeFds_.size();
    for( int i=0; i < size; ++i ) {
        int fd = changeFds_[i];
        ActiveFdEvent *ev = activeFdEvents_[fd];
        int oEvents = ev->events_;
        int oReify  = ev->refiy_;

        ev->events_ = 0;
        ev->refiy_  = 0;

        for ( EventIo* e = ev->head_; e !=NULL; e = e->next_ ){
            ev->events_ |= e->getEvents();
        }

        if ( oEvents != ev->events_ ) {
            oReify = EV_IOFDSET;
        }

        if( OReify & EV_IOFDSET ) {
            poller_->updateEvent( fd, oEvents, ev->events_ );
        }

    }
    changeFds_.clear();
}

ActiveFdEvent* EventLoop::getActiveFdEventByFd( int fd ) {
    return activeFdEvents_[fd];
}

void  EventLoop::updateTime( Timestamp maxBlockTime )
{
    nowTime = tinyGetTime();
    if( expect_false( curTime_ > nowTime  || nowTime > curTime_ + maxBlockTime + MIN_TIMEJUMP ) ){

    }
    curTime_ = tinyGetTime();
}

void EventLoop::timerReSchedule( Timestamp adjust )
{
    int timerCnt = getTimerCount();
    for( int i=0; i < timerCnt; ++i ) {
        TimerHeap* he = timers_[ i + kHeap0 ];
        he->setAt( he->getAt() + adjust );
    }
}







