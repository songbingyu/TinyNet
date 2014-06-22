/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef  _TCP_SERVER_H_
#define  _TCP_SERVER_H_
#include <sys/socket.h>
#include <SocketHelper.h>

class  TcpServer
{
public:
     TcpServer( int port = 4622 );
     ~TcpServer( );

public:

    int     bindAndListen( );

    void    run();

private:
    int                     port_;
    struct  sockaddr_in     addr_;
    int                     listenfd_;
    SocketHelper            socketHelper_;
    bool                    isListening_;
};

#endif // _TCP_SERVER_H_











