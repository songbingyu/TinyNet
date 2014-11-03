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
#include <algorithm>
#include "Log.h"
#include "TcpAcceptor.h"
#include "EventLoop.h"
#include "TcpServer.h"

using namespace std::placeholders;


void PrintLog()
{
    printf("--------Welcome to use TinyNet CopyRight by bingyu ----------\n");
    printf("everything is ok, Let go \n");

}



TcpServer::TcpServer( int port ): port_( port )
{
    init();
}


TcpServer::~TcpServer()
{
    port_ = -1;
    TINY_DELETE( acceptor_ );
    TINY_DELETE( loop_ )
}


int TcpServer::init( )
{

    PrintLog();
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

    LOG_INFO("create socket sucess:%d", listenfd );

    acceptor_ = new TcpAcceptor( listenfd, loop_ , addr );

    if ( NULL == acceptor_  )
    {
        exit(1);
    }

    LOG_INFO("create TcpAcceptor sucess ");

    acceptor_->setNewConnectionCallBack( new NewConnectionCallBack( this, &TcpServer::onNewConnection ));

    return 1;

}

void TcpServer::run( )
{
     if( !acceptor_->isListen() )
     {
       LOG_ERROR("not listen, please first bind and listen ");
       return;
     }

    loop_-> run( EVRUN_ALWAYES );

    return;

}

void  TcpServer::onNewConnection( int*  fd, struct sockaddr_in* addr )
{
#ifdef  _DEBUG_
    assert( NULL == fd );
    assert( NULL == addr );
#endif

    Connection* conn  = new Connection( *fd, loop_,*addr );

    conn->setReadCallBack( readCallBack_ );
    conn->setWriteCallBack( writeCallback_ );
    conn->setCloseCallBack( std::bind( &TcpServer::onRemoveConnection, this, _1 ));
    conn->setConnCallback( connCallBack_ );

    connectionList_.push_back( conn );

    conn->onConnFinish();

    return ;
}

void TcpServer::onRemoveConnection( Connection* conn )
{
    closeCallBack_( conn );
    //should map?
    conn->onConnDestory();
    LOG_INFO("remove socket from connectionlist :%d ", conn->getSockFd());
    std::remove(connectionList_.begin(),connectionList_.end(),conn );
    TINY_DELETE( conn );
    return;
}





