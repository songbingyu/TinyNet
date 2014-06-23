/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef  _TCPACCEPTOR_H_
#define  _TCPACCEPTOR_H_


#include"EventDelegate.h"
#include"IConnection.h"

class SocketHelper;

class TcpServer;
class Connection;

class TcpAcceptor : public IConnection
{
public:
    TcpAcceptor( int fd, EventLoop* loop, struct sockaddr_in&  addr  );
    ~TcpAcceptor();
public:
    typedef  EventDelegate<TcpServer, int, struct sockaddr_in  >     NewConnectionCallBack;
    virtual int  onRead( );
    void    setNewConnectionCallBack( NewConnectionCallBack* cb ) { newConnCallBack_ = cb; }
    int     bindAndListen( );
    bool    isListen() {  return isListening_ == true;  }
private:
    bool                          isListening_;
    NewConnectionCallBack*        newConnCallBack_;

};


#endif //_TCPACCEPTOR_H_










