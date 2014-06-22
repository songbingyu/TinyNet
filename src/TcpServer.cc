/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include <sys/types.h>
#include <strings.h>
#include <assert.h>
#include <stdlib.h>

#include "TcpServer.h"

TcpServer::TcpServer( int port ): port_( port )
{
    bzero(&addr_, sizeof( addr_ ) );

    listenfd_       = -1;
    isListening_    = 0;
}


TcpServer::~TcpServer()
{


}


int TcpServer::bindAndListen( )
{
    assert( port >= 1024 );
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl( INADDR_ANY );
    addr_.sin_port = htons( port_ );

    listenfd_ = socketHelper_.createNonBlockingSocket( );
    if ( listenfd_ < 0  )
    {
        exit(1);
    }

    if ( socketHelper_.bind( listenfd_,  addr_ )  < 0 )
    {
        exit(1);
    }

    if( socketHelper_.listen( listenfd_ ) < 0 )
    {
        exit(1);
    }

    isListening_ = true;
}

void    TcpServer::run( )
{
   if( !isListening_ )
   {
       LOG_ERROR( "must be listen ... ");
       return;
   }


}
