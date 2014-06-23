/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef     _EVENTEPOLL_H_
#define     _EVENTEPOLL_H_

#include <map>
#include <vector>

struct      epoll_event;
class       IConnection;


// epoll wraper
class  EventEpoll {
public :
    EventEpoll();
    ~EventEpoll();
public:

    int epollCreate();

    int addEvent( IConnection* conn );

    int delEvent( IConnection* conn );

    int waitEvent( int timeout, std::vector<IConnection*>*  activeConns  );

private:
    int  epollCtl( int oper, IConnection* conn );
private:

    int epollfd_;
    typedef  std::vector< struct epoll_event >  EventVec;
    EventVec            events_;
    typedef  std::map< int, IConnection* >     ConnectionMap;
    ConnectionMap       connectionMap;

};


#endif //_EVENTEPOLL_H_




