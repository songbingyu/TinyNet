/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef   _EVENTLOOP_H_
#define   _EVENTLOOP_H_

#include<vector>
#include "EventEpoll.h"

class IConnections;

class  EventLoop
{
public :
     EventLoop();
     ~EventLoop();

public:

public:
     int run();

private:
     EventEpoll eventEpoll_;
     typedef std::vector<IConnections*>    ConnectionVec;

     ConnectionVec      activeConnections_;
};


#endif // _EVENTLOOP_H_




