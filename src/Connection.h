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
#include "SimpleBuffer.h"

class EventLoop;
class Connection;

typedef std::function< void  ( Connection* ) > ReadCallBack;
typedef std::function< void  ( Connection* ) > WriteCallBack;
typedef std::function< void  ( Connection* ) > CloseCallBack;
typedef std::function< void  ( Connection* ) > ConnCallBack;

const size_t kBufSize = 1024*4;

#ifdef USE_CIRCULAR_BUFFER
typedef CircularBuffer<kBufSize> Buffer;
#else
typedef SimpleBuffer             Buffer;
#endif

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
    tiny_hot tiny_forceinline int     onRead ();
    tiny_forceinline int     onWrite();
    tiny_forceinline int     onClose();
    tiny_forceinline  int     onError();

    int     onConnFinish();
    int     onConnDestory();
public:
    void    send( char* data, int len );
    void    close();
    void    setTcpNoDelay( bool on ) { socketHelper_->setTcpNoDelay( sockfd_, on ); }
public:
    tiny_hot tiny_forceinline  static void onEvents( EventLoop* loop, IEvent* ev, int revents );

public:

    void    setReadCallBack ( const  ReadCallBack&  cb ) { readCallback_  = cb; }
    void    setWriteCallBack( const  WriteCallBack& cb ) { writeCallback_ = cb; }
    void    setCloseCallBack( const CloseCallBack& cb ) { closeCallBack_ = cb; }
    void    setConnCallBack(const ConnCallBack& cb ) { connCallBack_ = cb; }

    Buffer* getReadBuffer() { return &readBuf_; }
    Buffer* getWriteBuffer() { return &writeBuf_; }

    bool    isConnected() const { return state_ == CS_Connected;  }

private:
    ConnectionState  state_;
    ReadCallBack            readCallback_;
    WriteCallBack           writeCallback_;
    CloseCallBack           closeCallBack_;
    ConnCallBack            connCallBack_;
    EventIo                 ev_;

    Buffer  readBuf_;
    Buffer  writeBuf_;

};

#endif // _CONNECTION_H_








