/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef  _EVENT_
#define  _EVENT_

#include "TinyDefine.h"
#include "CallBackDelegate.h"


// all event base
class       IEvent;
class       EventLoop;
class       TimerEventList;
typedef     TimerEventList  TimerHeap;
typedef     std::vector< TimerHeap* >  HeapVec;
typedef     void (*EVENT_CB)(EventLoop* loop, IEvent* ev,  int revents_);

class IEvent
{
public:
    IEvent( EventLoop* loop, EVENT_CB cb ):active_(0),pending_(0),data_(NULL),cb_(cb), loop_(loop){ }
    ~IEvent() { }
protected:
    void  init()
    {
        active_     = 0;
        pending_    = 0;
        data_       = NULL;
    }
public:
    tiny_forceinline void   onEvent(int revents );
    tiny_forceinline void   ev_start( int active );
    tiny_forceinline void   ev_stop();
    tiny_forceinline bool   isActive() const { return active_ > 0 ; }
    tiny_forceinline int    getActive() const { return active_; }
    tiny_forceinline void   setActive( int active ) { active_ =  active; }
    tiny_forceinline void   setPending( int pending )  { pending_ = pending; }
    tiny_forceinline int    getPending() const { return pending_; }
    tiny_forceinline void*  getUserData() const { return data_; }
    tiny_forceinline void   setUserData( void* data ){ data_ = data; }
protected:
    int         active_;
    int         pending_;
    void*       data_;
    EVENT_CB    cb_;
    EventLoop*  loop_;

};

class  EventList  : public IEvent
{
public:
    EventList( EventLoop* loop, EVENT_CB cb ) :IEvent( loop, cb ),next_(NULL)
    {

    }
    ~EventList()
    {

    }
    tiny_forceinline    EventList**  getNext()   { return &this->next_;  }
protected:
    EventList*  next_;
};

class TimerEventList : public IEvent
{
public:
    TimerEventList( EventLoop* loop, EVENT_CB cb, Timestamp after ) : IEvent( loop, cb ), at_(after)
    {

    }
    ~TimerEventList()
    {

    }
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
    EventIo( EventLoop* loop, EVENT_CB cb, int fd , int events ):EventList( loop, cb ), fd_(fd),events_( events )
    {

    }
    ~EventIo()
    {

    }

    tiny_forceinline    int getFd()       const { return fd_;     }
    //tiny_forceinline    void setEvents( int flag ) { events_ = flag; }
    tiny_forceinline    int getEvents()   const { return events_; }
    tiny_forceinline    bool isReading() const { return events_&EV_READ;   }
    tiny_forceinline    bool isWriting() const { return events_&EV_WRITE; }
    tiny_forceinline    bool changeEvents( int events );


public:
    void start();
    void stop();
private:
    int     fd_;
    int     events_;

};

class EventTimer : public   TimerEventList
{
public:
    EventTimer( EventLoop* loop, EVENT_CB cb, Timestamp after, int repeat ):TimerEventList( loop, cb, after ),repeat_( repeat )
    {

    }
public:
    int  getRepeat() const { return repeat_; }
public:
    void start();
    void stop();
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
public:
    EventSignal( EventLoop* loop, EVENT_CB cb, int sigNum ):EventList( loop, cb ),sigNum_(sigNum)
    {

    }
    ~EventSignal()
    {

    }
public:
    tiny_forceinline    void setSigNum( int sigNum ) { sigNum_ = sigNum; }
    tiny_forceinline    int  getSigNum() const       { return sigNum_;   }
    void start();
    void stop ();

private:
    int sigNum_;
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



