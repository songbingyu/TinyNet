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
#include "Connection.h"
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

    acceptor_->setNewConnectionCallBack( new NewConnectionCallBack( this, &TcpServer::onNewConnection ));

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

void  TcpServer::onNewConnection( int*  fd, struct sockaddr_in* addr )
{
#ifdef  _DEBUG_
    assert( NULL == fd );
    assert( NULL == addr );
#endif

    Connection* conn  = new Connection( *fd, loop_ ,*addr );

    conn->setReadCallBack( new ReadCallBack( this, &TcpServer::onRead ) );
    conn->setWriteCallBack( new WriteCallBack( this, &TcpServer::onWrite ));
    conn->setCloseCallBack( new CloseCallBack( this, &TcpServer::onClose ) );
    connectionList_.push_back( conn );

    onConnection( conn );

    return ;
}

int TcpServer::onConnection( Connection* conn )
{
    LOG_INFO(" receive new connection ");
    return 1;
}


void TcpServer::onRead( Connection* conn, int* arg  )
{
    return ;
}

void TcpServer::onWrite( Connection* conn, int* arg )
{
    return ;

}

void TcpServer::onClose( Connection* conn, int* arg )
{

    return ;
}


