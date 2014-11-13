/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef  _TCP_SERVER_H_
#define  _TCP_SERVER_H_


#include <list>
#include "Connection.h"

class  TcpAcceptor;
class  EventLoop;

class  TcpServer
{
public:
     TcpServer( int port = 8888 );
     ~TcpServer( );

public:
    void    run( int flag );
private:
    int     init();
public:
    void    onNewConnection( int*  fd, struct sockaddr_in*  addr  );
    void    onRemoveConnection( Connection* conn );
public:
    void    setReadCallBack( const ReadCallBack& cb ) { readCallBack_ = cb; }
    void    setWriteCallBack( const WriteCallBack& cb ) { writeCallback_ = cb; }
    void    setCloseCallBack( const CloseCallBack& cb ) { closeCallBack_ = cb; }
    void    setConnCallBack( const ConnCallBack& cb ) { connCallBack_ = cb; }
private:
    int                     port_;
    TcpAcceptor*            acceptor_;
    EventLoop*              loop_;
    typedef   std::list<Connection*> ConnectionList;
    ConnectionList          connectionList_;

    ReadCallBack    readCallBack_;
    WriteCallBack   writeCallback_;
    CloseCallBack   closeCallBack_;
    ConnCallBack    connCallBack_;
};

#endif // _TCP_SERVER_H_













