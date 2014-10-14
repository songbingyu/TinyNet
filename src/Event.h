/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef  _EVENT_
#define  _EVENT_

#include "TinyDefine.h"
#include "CallBackDelegate.h"
#include "EventLoop.h"
#inclide "Log.h"

typedef     void (*EV_CB)( EventLoop* loop,int revents );

typedef     double      Timestamp;

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
}

// all event base

typedef  void (*EVENT_CB)(int revents_);

class IEvent
{
public:
    IEvent(EVENT_CB cb ): cb_(cb){ }
    ~IEvent() { }
protected:
    void  init()
    {
        active_     = 0;
        pending_    = 0;
        data_       = NULL;
    }
public:
    tiny_forceinline void  onEvent( EventLoop* loop, int revents )
    {
        if( expect_true( cb_ != NULL ) ) {
            (*cb_)( revents);
        }
    }

    tiny_forceinline void  ev_start( EventLoop* loop, int active )
    {
            active_ = active;
            loop->addActiveCnt();

    }

    tiny_forceinline void  ev_stop( EventLoop* loop )
    {
        active_ = 0;
        loop->delActiveCnt();
    }

    tiny_forceinline bool   isActive() const { return active_ > 0 ; }
    tiny_forceinline int    getActive() const { return active_; }
    tiny_forceinline void   setActive( int active ) { active_ =  active; }
    tiny_forceinline void   setPending( int pending )  { pending_ = pending; }
    tiny_forceinline int    getPending() const { return pending_; }

protected:
    int         active_;
    int         pending_;
    void*       data_
    EVENT_CB    cb_;

};

class  EventList  : public IEvent
{
protected:
    EventList*  next_;
};

class TimerEventList : public IEvent
{
public:
    TimerEventList( EVENT_CB cb , Timestamp after ) : IEvent( cb ), at_( after )
public:
    Timestamp   getAt() const  { return at_; }
    TimerEventList** getNext() { return &next_; }
    tiny_forceinline void setAt( Timestamp at ) { at_ = at; }
protected:
    Timestamp   at_;
    TimerEventList*  next_;
};

class EventIo : public EventList
{

public:
    EventIo( EVENT_CB cb, int fd , int events ):IEvent( cb ), fd_( fd ),events_( events | EV_IOFDSET )
    {

    }
    ~EventIo()
    {

    }

    tiny_forceinline    int getFd()       const { return fd_;     }
    tiny_forceinline    void setEvents( int flag ) { eventd_ = flag; }
    tiny_forceinline    int getEvents()   const { return events_; }
    tiny_forceinline    EventIo**  getNext() const  { return &this->next_;  }

public:
    void start( EventLoop* loop_ )
    {
        if (expect_false( isActive() ) ){
            return;
        }

        assert( fg_ > 0 );
        assert( !(events_&~(EV_IOFDSET | EV_READ | EV_WRITE )))

        ev_start( loop, 1 );

        loop_->addActiveFdEvent( this );
    }

    void stop(EventLoop* loop )
    {
        loop_->delPendingEvent( this );

        if ( expect_false( !isActive() ) ) {
            return false ;
        }

        assert( fd > 0 );

        loop_->delActiveFdEvent();

        ev_stop( loop );
    }
private:
    int     fd_;
    int     events_;

};

class EventTimer : public   TimerEventList
{
public:
    EventTimer( EVENT_CB cb, Timestamp after, int repeat ):TimerEventList( cb, after ),repeat_( repeat )
    {

    }
public:
    int  getRepeat() const { return repeat_; }
public:
    void start( EventLoop* loop )
    {
        if( isActive() ) {
            return;
        }

        at_ += loop->getNowTime();

        tiny_assert( repeat_ >= 0 );

        ev_start( loop, loop->getTimerCount() + 1 );

        loop->addTimer( this );

    }

    void stop( EventLoop* loop )
    {
        if( expect_false( !isActive() )){
            return;
        }

        int active = getActive();
        if( expect_true( active < loop->getTimerCount() +kHeap0 -1 ) ){
            timers_[active] = timers_[ loop->getTimerCount() ]
        }
    }
private:
    int    repeat_;
};


class EventPeriodic : public TimerEventList
{
public:
    Timestamp   offset_;
    Timestamp   interval_;
};

class EventSignal : public EventList
{
public :
    int signum_;
};


class EventPrepare : public EventList
{
public:

};

class EventCheck : public EventList
{

};

class EventClear : public EventList
{

};



#endif



