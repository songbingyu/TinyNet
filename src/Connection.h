/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <functional>
#include "TinyDefine.h"
#include "IConnection.h"
#include "CallBackDelegate.h"
#include "CircularBuffer.h"

class EventLoop;
class Connection;

typedef std::function< void  ( Connection* ) > ReadCallBack;
typedef std::function< void  ( Connection* ) > WriteCallBack;
typedef std::function< void  ( Connection* ) > CloseCallBack;
typedef std::function< void  ( Connection* ) > ConnCallBack;

enum ConnectionState
{
    CS_No,
    CS_Connecing,
    CS_Connected,
    CS_DisConnecting,
    CS_DisConnected,
};

class Connection : public IConnection
{
public:
    Connection( int fd,  EventLoop* loop, struct sockaddr_in& addr  );
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

    void    setReadCallBack ( const  ReadCallBack&  cb ) { readCallback_  = cb; }
    void    setWriteCallBack( const  WriteCallBack& cb ) { writeCallback_ = cb; }
    void    setCloseCallBack( const CloseCallBack& cb ) { closeCallBack_ = cb; }
    void    setConnCallback(const ConnCallBack* cb ) { connCallBack_ = cb; }

    bool    isConnected() const { return state_ == CS_Connected;  }

private:
    ConnectionState  state_;
    ReadCallBack            readCallback_;
    WriteCallBack           writeCallback_;
    CloseCallBack           closeCallBack_;
    ConnCallBack            connCallBack_;
    EventIo                 ev_;

    static const size_t kBufSize = 1024*4;
    CircularBuffer<kBufSize>    readBuf_;
    CircularBuffer<kBufSize>    writeBuf_;
};

#endif // _CONNECTION_H_








