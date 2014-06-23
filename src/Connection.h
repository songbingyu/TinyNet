/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include"TcpServer.h"
#include"EventDelegate.h"

class Connection;


//typedef  EventDelegate<TcpServer, Connection>     NewConnectionCallBack;
typedef  EventDelegate<TcpServer, Connection>     ReadCallBack;
typedef  EventDelegate<TcpServer, Connection>     WriteCallBack;
typedef  EventDelegate<TcpServer, Connection>     CloseCallBack;

class EventLoop;


class Connection
{
public:
    Connection( int fd,  EventLoop* loop );
    ~Connection();
public:
    int  onRead( );
    int  onWrite( );
    int  onClose( );

public:

private:
    //NewConnectionCallBack
    ReadCallBack    readCallback _;
    WriteCallBack   writeCallback_;
    CloseCallback   closeCallBack_;

    int         sockfd_;
    EventLoop*  loop_;
};

#endif // _CONNECTION_H_







