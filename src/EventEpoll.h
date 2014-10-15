/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef     _EVENTEPOLL_H_
#define     _EVENTEPOLL_H_

#include <map>
#include <vector>
#include "nocopyable.h"
#include "IPoller.h"

struct      epoll_event;
//class       IConnection;


// epoll wraper
class  EventEpoll : public IPoller
{
public :
    EventEpoll( EventLoop* loop );
    virtual ~EventEpoll();
public:
    // see libevent ,find something is  trick stupid....

    // update?

    virtual void  updateEvent( int fd, int oev, int nev  );

    //int addEvent( IConnection* conn );

    //int delEvent( IConnection* conn );

    virtual void waitEvent( Timestamp ts );

private:
    int  epollCreate();
private:
    int epollfd_;
    int     epollEventMax_;
    typedef  std::vector< struct epoll_event >  EventVec;
    EventVec            events_;
    typedef std::vector<int>            EpermsFdArr;
    EpermsFdArr         epermFds_;

};


#endif //_EVENTEPOLL_H_




