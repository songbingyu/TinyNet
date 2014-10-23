/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "TinyDefine.h"
#include "IConnection.h"
#include "TcpServer.h"
#include "CallBackDelegate.h"
#include "CircularBuffer.h"

class EventLoop;

typedef  CallBackDelegate<TcpServer, Connection, int >     ReadCallBack;
typedef  CallBackDelegate<TcpServer, Connection, int >     WriteCallBack;
typedef  CallBackDelegate<TcpServer, Connection, int >     CloseCallBack;

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
    Connection( int fd,  EventLoop* loop );
    ~Connection();
public:
    int     onRead ();
    int     onWrite();
    int     onClose();
    int     onError();
    int     onConnFinish();
    int     onConnDestory();
public:
    void    send( char* data, int len );
    void    close();
public:
    static void onEvents( EventLoop* loop, IEvent* ev, int revents );
public:
    void         setReadCallBack ( ReadCallBack*  cb ) { readCallback_  = cb; }
    void         setWriteCallBack( WriteCallBack* cb ) { writeCallback_ = cb; }
    void         setCloseCallBack( CloseCallBack* cb ) { closeCallBack_ = cb; }
private:
    ConnectionState  state_;
    ReadCallBack*           readCallback_;
    WriteCallBack*          writeCallback_;
    CloseCallBack*          closeCallBack_;
    EventIo                 ev_;

    static const size_t  c_BufSize = 1024*4;
    CircularBuffer<char, c_BufSize>    readBuf_;
    CircularBuffer<char, c_BufSize>    writeBuf_;
};

#endif // _CONNECTION_H_








