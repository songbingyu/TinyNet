/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef   _EVENTLOOP_H_
#define   _EVENTLOOP_H_


#include "EventEpoll.h"


class  EventLoop
{
public :
     EventLoop();
     ~EventLoop();

public:
     int run();

private:
     EventEpoll eventEpoll_;
};


#endif // _EVENTLOOP_H_




