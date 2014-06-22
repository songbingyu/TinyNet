/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */


#include <strings.h>
#include "TcpServer.h"

TcpServer::TcpServer( int port ): port_( port )
{
    bzero(&addr_, sizeof( addr_ ) );

    listenfd_ = -1;
}


TcpServer::~TcpServer()
{


}


int TcpServer::bindAndListen( )
{


}


