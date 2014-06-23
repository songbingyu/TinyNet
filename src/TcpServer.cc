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
#include "TcpServer.h"

TcpServer::TcpServer( int port ): port_( port )
{

    bindAndListen();

}


TcpServer::~TcpServer()
{
    port_ = 0;
    bzero( &addr_, sizeof(addr_) );
    socketHelper_.close( listenfd_  );
    listenfd_ = -1;
    isListening_ = false;
}


int TcpServer::bindAndListen( )
{
    assert( port_  >= 1024 );

    bzero( &addr_, sizeof(addr_) );
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl( INADDR_ANY );
    addr_.sin_port = htons( port_ );

    //Fixme: should beautiful exit ?
    listenfd_ = socketHelper_.createNonBlockingSocket( );
    if ( listenfd_ < 0  )
    {
        exit(1);
    }

    if ( socketHelper_.bind( listenfd_,  &addr_ )  < 0 )
    {
        exit(1);
    }

    if( socketHelper_.listen( listenfd_ ) < 0 )
    {
        exit(1);
    }

    isListening_ = true;
}

void TcpServer::run( )
{
     if( !isListening_ )
     {
       //LOG_ERROR("not listen ");
       return;
     }




}

int TcpServer::onNewConnection()
{

}

int TcpServer::onConnection( Connection* conn )
{

}


int TcpServer::onRead( Connection* conn )
{

}

int TcpServer::onWrite( Connection* conn )
{


}

int TcpServer::onClose( Connection* conn )
{

}


