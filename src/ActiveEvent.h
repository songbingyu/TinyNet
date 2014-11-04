/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _ACTIVE_EVENT_H_
#define _ACTIVE_EVENT_H_

#include "TinyDefine.h"

class EventIo;
class EventList;
class EventLoop;


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
    void addList( EventIo* ev );
    void delList( EventIo* ev );
    tiny_forceinline  void  tiny_cold  killFd( EventLoop* loop );
    tiny_forceinline void fdEvent( EventLoop* loop, int revents  );
public:
    EventIo*        head_;
    int             events_;
    int             refiy_;
    int             revents_;
    unsigned int    egen_;
};

class ActiveSignalEvent
{
public:
    ActiveSignalEvent(): head_(NULL), loop_(NULL), pending_(0)
    {

    }
    ~ActiveSignalEvent()
    {

    }
public:
    void addList( EventList* el );
    void delList( EventList* el );
    void addFeedEvent();
public:
    EventList*  head_;
    EventLoop*  loop_;
    int         pending_;
};


#endif






