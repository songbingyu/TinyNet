/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef   _EVENTLOOP_H_
#define   _EVENTLOOP_H_

#include <vector>
#include "nocopyable.h"
#include "EventEpoll.h"

class IConnections;

typedef std::vector<IConnection*>  ConnectionVec;

class  EventLoop: public nocopyable
{
public :
     EventLoop();
     ~EventLoop();

public:
    void updateEvent( IConnection* conn ) {  eventEpoll_.updateEvent( conn ); }
    void delEvent( IConnection* conn )    {  eventEpoll_.delEvent( conn ); }
    void stop() {  isRuning_ = false; }
public:
     int run();

private:
     bool               isRuning_;
     EventEpoll         eventEpoll_;
     ConnectionVec      activeConnections_;
};


#endif // _EVENTLOOP_H_




