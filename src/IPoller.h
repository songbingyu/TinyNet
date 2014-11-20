/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _POLLER_H_
#define _POLLER_H_

#include "nocopyable.h"
#include "TinyDefine.h"


class IEvent;
class EventLoop;
class ActiveFdEvent;


class IPoller : nocopyable
{
public :
    IPoller(EventLoop* loop);
    virtual ~IPoller();
public :
    virtual void updateEvent(int fd,int oev, int nev) = 0;
    virtual void waitEvent(Timestamp ts) = 0;
public:
    tiny_forceinline Timestamp getMinWaitTime() const { return minWaitTime_; }
protected:

protected:
    Timestamp   minWaitTime_;
    EventLoop*  loop_;

};




#endif





