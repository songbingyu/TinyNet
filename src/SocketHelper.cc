/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include <sys/types.h>
#include <sys/socket.h>
#include "Log.h"

int SocketHelper::createNonBlockingSocket( )
{
    int fd = ::socket( AF_INET, SOCK_STREAM|SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP );
    if( fd < 0 ) LOG_ERROR( "create socket fail ");
    return fd;
}

int SocketHelper::bind( int fd, sockaddr_in* addr );
{
    int r = ::bind( fd, (sockaddr*)addr, (socklen_t)sizeof(addr) );
    if( r < 0 ) LOG_ERROR( " bind fail ");

    return r;
}

int SocketHelper::listen( int fd )
{
    //Fixme: backlog sure?
    int r  = ::listen( fd, 50 );
    if( r < 0 ) LOG_ERROR( "listen  fail ");

    return r;

}


int SocketHelper::accept( int fd,  sockaddr_in* addr )
{

    int addrlen = ( socklen_t ) ( sizeof( addr ) );
    int  connfd =  ::accept4( sockfd, (sockaddr*)(addr),
                                      &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC)

    if ( connfd < 0 )
    {

    }
}

int SocketHelper::connect( int fd, socketaddr_in* addr )
{

}

int SocketHelper::read( int fd, char* buf, int count )
{


}

int SocketHelper::write( int fd, char* buf, int count )
{


}

int SocketHelper::shutdown( int fd )
{


}












