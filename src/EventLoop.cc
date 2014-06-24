/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */


#include "EventLoop.h"

EventLoop::EventLoop(): isRuning_( true )
{
    eventEpoll_.epollCreate();
}

EventLoop::~EventLoop()
{

    activeConnections_.clear();

}

int  EventLoop::run()
{

    while( isRuning_  )
    {

        activeConnections_.clear();
    }

    return 1;

}


