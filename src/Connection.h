/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include"IConnection.h"
#include"TcpServer.h"
#include"EventDelegate.h"


class EventLoop;


class Connection : public IConnection
{
public:
    Connection( int fd,  EventLoop* loop, struct sockaddr_in& addr  );
    ~Connection();
public:
    virtual int  onRead( );
    virtual int  onWrite( );
    virtual int  onClose( );

public:

private:

    typedef  EventDelegate<TcpServer, Connection, int >     ReadCallBack;
    typedef  EventDelegate<TcpServer, Connection, int >     WriteCallBack;
    typedef  EventDelegate<TcpServer, Connection, int >     CloseCallBack;
    //NewConnectionCallBack
    ReadCallBack*    readCallback_;
    WriteCallBack*   writeCallback_;
    CloseCallBack*   closeCallBack_;

};

#endif // _CONNECTION_H_







