/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef     _EVENTEPOLL_H_
#define     _EVENTEPOLL_H_

#include<map>
#include<vector>


struct      epoll_event;


// epoll wraper
class  EventEpoll
{
 public :
    EventEpoll();
    ~EventEpoll();
 public:

    int epollCreate();

    int addEvent();

    int delEvent();

    int modEvent();

    int waitEvent( int timeout, ConnectionVec*  activeConns  );

 public:
 private:

    int epollfd_;
    typedef  std::vector< struct epoll_event >  EventVec;
    EventVec            events_;
    typedef  std::map< int, IConnections* > ConnectionMap;
    ConnectionMap       connectionMap;

};


#endif //_EVENTEPOLL_H_




