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

typedef  EventDelegate<TcpServer, Connection, int >     ReadCallBack;
typedef  EventDelegate<TcpServer, Connection, int >     WriteCallBack;
typedef  EventDelegate<TcpServer, Connection, int >     CloseCallBack;

enum ConnectionState
{
    CS_No,
    //CS_Connecing,
    CS_Connected,
    //CS_DisConnecting,
    CS_DisConnected,
};


class Connection : public IConnection
{
public:
    Connection( int fd,  EventLoop* loop, struct sockaddr_in& addr  );
    ~Connection();
public:
    virtual int  onRead ( );
    virtual int  onWrite( );
    virtual int  onClose( );
    virtual int  onError( );
    int          onConnFinish( );
public:
    void         setReadCallBack ( ReadCallBack*  cb ) { readCallback_  = cb; }
    void         setWriteCallBack( WriteCallBack* cb ) { writeCallback_ = cb; }
    void         setCloseCallBack( CloseCallBack* cb ) { closeCallBack_ = cb; }
private:
    ConnectionState  state_;
    //NewConnectionCallBack
    ReadCallBack*    readCallback_;
    WriteCallBack*   writeCallback_;
    CloseCallBack*   closeCallBack_;
};

#endif // _CONNECTION_H_







