/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef  _TCP_SERVER_H_
#define  _TCP_SERVER_H_

class Connection;

#include <SocketHelper.h>

struct sockaddr_in;

class  TcpServer
{
public:
     TcpServer( int port = 4622 );
     ~TcpServer( );

public:
    void    run();
private:
    int     bindAndListen( );
    int     onNewConnection( );
public:
    int     onConnection( Connection* conn );
    int     onRead ( Connection* conn );
    int     onWrite( Connection* conn );
    int     onClose( Connection* conn );
private:
    int                     port_;
    struct  sockaddr_in     addr_;
    int                     listenfd_;
    SocketHelper            socketHelper_;
    bool                    isListening_;
};

#endif // _TCP_SERVER_H_











