/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <assert.h>
#include <stdlib.h>

#include "Log.h"
#include "TcpAcceptor.h"
#include "EventLoop.h"
#include "TcpServer.h"

TcpServer::TcpServer( int port ): port_( port )
{
    init();
}


TcpServer::~TcpServer()
{
    port_ = -1;

    delete  acceptor_;
    acceptor_  = NULL;
    delete  loop_;
    loop_ = NULL;

}


int TcpServer::init( )
{

    loop_ = new EventLoop();
    assert( loop_ != NULL );
    assert( port_  >= 1024 );

    struct sockaddr_in addr;
    bzero( &addr, sizeof(addr) );
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl( INADDR_ANY );
    addr.sin_port = htons( port_ );

    //Fixme: should beautiful exit ?
    int listenfd  = SocketHelper::createNonBlockingSocket( );
    if ( listenfd < 0  )
    {
        exit(1);
    }

    acceptor_ = new TcpAcceptor( listenfd, loop_ , addr );

    if ( NULL == acceptor_  )
    {
        exit(1);
    }

    return 1;

}

void TcpServer::run( )
{
     if( !acceptor_->isListen() )
     {
       //LOG_ERROR("not listen ");
       return;
     }

    loop_-> run();

    return;

}

int TcpServer::onNewConnection( int fd, struct sockaddr_in& addr )
{
    return 1;
}

int TcpServer::onConnection( Connection* conn )
{
    return 1;
}


int TcpServer::onRead( Connection* conn )
{
    return 1;
}

int TcpServer::onWrite( Connection* conn )
{
    return 1;

}

int TcpServer::onClose( Connection* conn )
{

    return 1;
}


