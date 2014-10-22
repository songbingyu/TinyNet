/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef  _TCPACCEPTOR_H_
#define  _TCPACCEPTOR_H_


#include "CallBackDelegate.h"
#include "IConnection.h"

class SocketHelper;
class TcpServer;
class Connection;
class IEvent;
class EvetnIo;

typedef  CallBackDelegate<TcpServer, int, struct sockaddr_in >  NewConnectionCallBack;

class TcpAcceptor : public IConnection
{
public:
    TcpAcceptor( int fd, EventLoop* loop, struct sockaddr_in&  addr  );
    ~TcpAcceptor();
public:
    int     onRead( );
    void    setNewConnectionCallBack( NewConnectionCallBack* cb ) { newConnCallBack_ = cb; }
    int     bindAndListen( );
    bool    isListen() {  return isListening_ == true;  }
public:
    static void onEvents( EventLoop* loop, IEvent* ev, int revents );
private:

    bool                          isListening_;
    NewConnectionCallBack*        newConnCallBack_;
    struct sockaddr_in            localAddr_;
    EventIo                       ev_;
};



#endif //_TCPACCEPTOR_H_












