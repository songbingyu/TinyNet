/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */


#include "EventLoop.h"

EventLoop::EventLoop()
{
    eventEpoll_.epollCreate();
}

EventLoop::~EventLoop()
{


}

int  EventLoop::run()
{

    return 1;

}


