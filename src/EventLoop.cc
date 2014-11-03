/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include "EventLoop.h"
#include <sys/epoll.h>
#include "Log.h"
#include "Heap.h"
#include "ActiveEvent.h"
#include "PendingEvent.h"

EventLoop::EventLoop(): isRuning_( true ), curPid_(0),
                        activeCnt_(0),ioBlockTime_(0),signalHelper_(this)
{
    poller_ = getRecommendedPoller();
    curTime_ = tinyGetTime();
    //占位
    timers_.push_back( NULL );
}

EventLoop::~EventLoop()
{
    isRuning_ = false;

    TINY_DELETE( poller_ );

    PendingArr::iterator it = pendingEvents_.begin();
    for( ; it != pendingEvents_.end(); ++it ){
        delete *it;
    }
    pendingEvents_.clear();

    {
        ActiveFdArr::iterator it = activeFdEvents_.begin();
        for( ; it!= activeFdEvents_.begin(); ++it ){
            delete *it;
        }
        activeFdEvents_.clear();
    }

    changeFds_.clear();

    {
        HeapVec::iterator it = timers_.begin();
        for( ; it != timers_.end(); ++it ){
            delete *it;
        }
        timers_.clear();
    }

    rFeeds_.clear();
}

int  EventLoop::run( int flags )
{
    do {
        if( expect_false(curPid_) ) {
            curPid_ = getpid();
        }

        invokePending();
        fdReify();

        //time
        {
            Timestamp   waitTime = 0.;
            Timestamp   prevNowTime =  curTime_;
            Timestamp   sleepTime=0.;
            updateTime( 1e100 );

            if( expect_true( !( flags&EVRUN_NOWAIT  || !activeCnt_ ) ) ) {
                waitTime = MAX_BLOCKTIME;

                if( getTimerCount() > 0  )
                {
                    Timestamp to = timers_[kHeap0]->getAt() -  curTime_;
                    if( waitTime > to )  waitTime = to;
                }

                if( expect_false( waitTime < 0. ) ){
                    waitTime = 0.;
                }

                if( expect_false( waitTime < poller_->getMinWaitTime() ) ){
                    waitTime = poller_->getMinWaitTime();
                }

                if( expect_false( ioBlockTime_ ) ) {
                    sleepTime= ioBlockTime_ -( curTime_ - prevNowTime );
                    if( sleepTime > waitTime - poller_->getMinWaitTime() ){
                        sleepTime = waitTime = poller_->getMinWaitTime();
                    }

                    if( expect_true( sleepTime > 0. ) ){
                       tinySleep(sleepTime);
                       waitTime -= sleepTime;
                    }
                }

            }

            poller_->waitEvent( waitTime );

            updateTime( waitTime + sleepTime );

            timersReify();

            invokePending();


        }
    }while( expect_true( activeCnt_ &&
                !( flags & ( EVRUN_ONCE|EVRUN_NOWAIT )) ));

   return  activeCnt_;

}

IPoller* EventLoop::getRecommendedPoller()
{
    //select  ......
    return  new EventEpoll(this);
}

void EventLoop::addPendingEvent( IEvent* ev, int evFlag )
{
    if( expect_false( ev->getPending()) ) {
        pendingEvents_[ ev->getPending()-1 ]->eventFlag_ |= evFlag;
    } else {
        PendingEvent* pe  = new PendingEvent( ev, evFlag );
        pendingEvents_.push_back( pe );
        ev->setPending( (int)pendingEvents_.size() );
    }

}

void EventLoop::delPendingEvent( IEvent* ev )
{
    if( ev->getPending() ) {
        pendingEvents_[ev->getPending()-1]->clear();
        ev->setPending(0);
    }
}

void EventLoop::addActiveFdEvent(EventIo* ev )
{
    int  cnt = ev->getFd() + 1;
    if( cnt > (int)activeFdEvents_.size() ) {
        activeFdEvents_.resize( cnt, new ActiveFdEvent() );
    }

    ActiveFdEvent* activeEv = activeFdEvents_[ ev->getFd() ];

    activeEv->addList( ev );

    addChangeFd( ev->getFd(), EV_IOFDSET );

}

void EventLoop::delActiveFdEvent( EventIo* ev )
{
    ActiveFdEvent* activeEv = activeFdEvents_[ ev->getFd() ];
    activeEv->delList( ev );
    addChangeFd( ev->getFd(),EV_NO );
}

void EventLoop::addTimer( EventTimer* ev )
{
    if( ev->getActive()+1 > getTimerCount() ){
        timers_.resize( ev->getActive()+1 , NULL );
    }
    timers_[ ev->getActive() ] = ( TimerEventList* )ev;

    Tiny::upHeap( timers_, ev->getActive() );
}

void  EventLoop::delTimer( EventTimer* ev )
{
    if( expect_true( ev->getActive() < getTimerCount() + kHeap0 -1 ) ){
        timers_[ ev->getActive() ] = timers_[ getTimerCount() + kHeap0 -1 ];
        timers_.pop_back();
        Tiny::adjustHeap( timers_, getTimerCount(), ev->getActive() );
    }else {
        timers_.pop_back();
    }
}

void EventLoop::addSignalEvent( EventSignal* es )
{
    signalHelper_.addSignal( es );
}

void EventLoop::delSignalEvent( EventSignal* es )
{
    signalHelper_.delSignal( es );
}

void EventLoop::addFeedSignal( int sigNum )
{
   ActiveSignalEvent& es = Tiny::sigMaps[ sigNum ];

   es.pending_ = 1;

   signalHelper_.pipeWrite();

}

void EventLoop::onSignalEvent()
{
    signalHelper_.onSignalEvent();
}

void EventLoop::addFeedReverse( IEvent* ev )
{
    rFeeds_.push_back( ev );
}

void EventLoop::feedReverseDone( int revents )
{
    int  size = (int)rFeeds_.size();
    for( int i=0; i < size; ++i ){
        addPendingEvent( rFeeds_[i], revents );
    }
    rFeeds_.clear();
}

bool EventLoop::addChangeFd( int fd, int flags )
{
    int refiy = activeFdEvents_[fd]->refiy_;

    activeFdEvents_[fd]->refiy_ |= flags;

    if( expect_true( !refiy ) ) {
        changeFds_.push_back( fd );
    }

    return true;
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

        for ( EventIo* e = ev->head_; e !=NULL; e = (EventIo*)*(e->getNext()) ){
            ev->events_ |= e->getEvents();
        }

        if ( oEvents != ev->events_ ) {
            oReify = EV_IOFDSET;
        }

        if( oReify & EV_IOFDSET ) {
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
    Timestamp nowTime = tinyGetTime();
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

void EventLoop::timersReify()
{
    if( getTimerCount() && timers_[kHeap0]->getAt() < curTime_ ){
        do
        {
            EventTimer* ev = ( EventTimer* )timers_[kHeap0];

            if( ev->getRepeat() ){
                ev->setAt( ev->getAt() + ev->getRepeat() );
                if( ev->getAt() < curTime_ ){
                    ev->setAt( curTime_ );
                }
                Tiny::downHeap( timers_, getTimerCount(), kHeap0 );
            } else {
                ev->stop();
            }
            addFeedReverse( (IEvent*)ev );
        }while( getTimerCount() && timers_[kHeap0]->getAt() < curTime_ );

        feedReverseDone( EV_TIMER );
    }
}

void EventLoop::invokePending( )
{
    int size = pendingEvents_.size();
    for( int i=0; i < size; ++i ){
        PendingEvent* pe = pendingEvents_[i];
        pe->event_->setPending( 0 );
        pe->event_->onEvent( pe->eventFlag_ );
    }

    pendingEvents_.clear();
}



