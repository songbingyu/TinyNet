/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef   _EVENTLOOP_H_
#define   _EVENTLOOP_H_

#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include "nocopyable.h"
#include "TinyDefine.h"
#include "EventEpoll.h"
#include "Event.h"

class ActiveFdEvent;
class IConnections;
class PendingEvent;
class IPoller;
class PendingEvent;

typedef std::vector<ActiveFdEvent*> ActiveEventVec;

class  EventLoop: public nocopyable
{
public :
     EventLoop();
     ~EventLoop();

public:
    void stop() {  isRuning_ = false; }

public:
     int run();
public:
     void addPendingEvent( IEvent* ev, int evFlag );
     void delPendingEvent(IEvent* ev );
     void addActiveFdEvent ( EventIo* ev );
     void delActiveFdEvent ( EventIo* ev );
     void addTimer( EventTimer* ev );
     void delTimer( EventTimer* ev );
     void addFeedReverse( IEvent* ev );
     void feedReverseDone( int revents );
     ActiveFdEvent*  getActiveFdEventByFd( int fd );
     tiny_forceinline void addActiveCnt() { ++activeCnt_; }
     tiny_forceinline void delActiveCnt() { --activeCnt_; }
     tiny_forceinline Timestamp   getNowTime() const  { return curTime_; }
     tiny_forceinline int         getTimerCount()     { return timers_.size() - 1; }
     tiny_forceinline void  setIoBlockTime( Timestamp interval ) { ioBlockTime_ = interval; }
     void invokePending();
private:
     IPoller* getRecommendedPoller();
     tiny_forceinline   bool addChangeFd( int fd, int flags );
     tiny_forceinline   void fdReify();
     tiny_forceinline   void updateTime( Timestamp maxBlockTime );
     tiny_noinline tiny_cold void timerReSchedule( Timestamp adjust );
     tiny_forceinline   void timersReify();
private:

    typedef  std::vector<PendingEvent*>   PendingArr;
    PendingArr          pendingEvents_;
    bool                isRuning_;
    IPoller*            poller_;
    typedef std::vector<ActiveFdEvent*>  ActiveFdArr;
    ActiveFdArr         activeFdEvents_;
    typedef std::vector<int>            ChangeFdArr;
    ChangeFdArr         changeFds_;
    pid_t               curPid_;
    int                 activeCnt_;
    HeapVec             timers_;
    typedef std::vector<IEvent*>  ReverseArr;
    ReverseArr          rFeeds_;
    Timestamp           curTime_;
    Timestamp           ioBlockTime_;
};


#endif // _EVENTLOOP_H_




