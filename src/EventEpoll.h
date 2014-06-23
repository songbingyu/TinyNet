/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef     _EVENTEPOLL_H_
#define     _EVENTEPOLL_H_


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

    int modvent();

    int epollWait();

 public:
    typedef  std::vector< struct epoll_event >  EventVec;

    EventVec    events_;
 private:

    int epollfd_;

};


#endif //_EVENTEPOLL_H_




