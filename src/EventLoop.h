/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef   _EVENTLOOP_H_
#define   _EVENTLOOP_H_

#include<vector>
#include "EventEpoll.h"

class IConnections;

typedef std::vector<IConnection*>  ConnectionVec;

class  EventLoop
{
public :
     EventLoop();
     ~EventLoop();

public:
    void update( IConnection* conn ) {  eventEpoll_.addEvent( conn ); }
    void stop() {  isRuning_ = false; }
public:
     int run();

private:
     bool               isRuning_;
     EventEpoll         eventEpoll_;
     ConnectionVec      activeConnections_;
};


#endif // _EVENTLOOP_H_




