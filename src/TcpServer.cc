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
#include "Connection.h"
#include "EventLoop.h"
#include "TcpServer.h"

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

    delete  acceptor_;
    acceptor_  = NULL;
    delete  loop_;
    loop_ = NULL;

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

    conn->setReadCallBack(  new ReadCallBack( this, &TcpServer::onRead ) );
    conn->setWriteCallBack( new WriteCallBack( this, &TcpServer::onWrite ));
    conn->setCloseCallBack( new CloseCallBack( this, &TcpServer::onClose ) );
    connectionList_.push_back( conn );

    conn->onConnFinish();
    onConnection( conn );

    return ;
}

int TcpServer::onConnection( Connection* conn )
{
    LOG_INFO(" receive new connection %d ", conn->getSockFd() );
    return 1;
}


void TcpServer::onRead( Connection* conn, int* arg  )
{
    LOG_INFO(" read conn ");
    return ;
}

void TcpServer::onWrite( Connection* conn, int* arg )
{
    LOG_INFO(" write conn ");
    return ;
}

void TcpServer::onClose( Connection* conn, int* arg )
{

    //First call user define func

    //delete from list
    //

    //should map?
    LOG_INFO("remove socket from connectionlist :%d ", conn->getSockFd());
    std::remove(connectionList_.begin(),connectionList_.end(),conn );
    delete conn;

    return ;
}


