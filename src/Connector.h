/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_


#include "TinyDefine.h"
#include <netinet/in.h>

#include "SocketHelper.h"


class IEvent;
class EventIo;
class EventLoop;


class Connector
{
public:
    Connector( EventLoop* loop, struct sockaddr_in& addr  );
    ~Connector();
public:
    void start();
    void restart();
    void stop();
public:
    static void onEvents( EventLoop* loop, IEvent* ev, int revents );
private:
    enum States
    {
        kDisConnect,
        kConnecting,
        kConnected,
    }

    enum {
        kMaxReTryCnt = 3;
    }

    tiny_forceinline  void setState( States s) { state_ = s; }
    void connect();
    void retry( int fd );
    void delEvent();
    void onWaitConnectFinish( int fd );
    void onWrite();
    void onError();
private:
    EventLoop* loop_;
    struct sockaddr_in localAddr_;
    SocketHelper socketHelper_;
    bool        connect_;
    States      state_;
    EventIo*    ev_;
    int         retryCnt_;

};


#endif //_CONNECTOR_H_



