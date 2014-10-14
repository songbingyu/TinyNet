/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include "IPoller.h"
#include "Event.h"
#include "EventLoop.h"

IPoller::IPoller( EventLoop* loop ): loop_(loop)
{

}

IPoller::~IPoller()
{
    loop_ = NULL;
}













