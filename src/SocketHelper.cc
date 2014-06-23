/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include "Log.h"
#include "SocketHelper.h"

int SocketHelper::createNonBlockingSocket( )
{
    int fd = ::socket( AF_INET, SOCK_STREAM|SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP );
    if( fd < 0 ) LOG_ERROR( "create socket fail ", 1);
    return fd;
}

int SocketHelper::bind( int fd, const struct sockaddr_in* addr )
{
    int r = ::bind( fd, (const struct sockaddr*)addr, (socklen_t)sizeof(addr) );
    if( r < 0 ) LOG_ERROR( " bind fail ", 1);

    return r;
}

int SocketHelper::listen( int fd )
{
    //Fixme: backlog sure?
    int r  = ::listen( fd, 50 );
    if( r < 0 ) LOG_ERROR( "listen  fail ", 1);

    return r;

}


int SocketHelper::accept( int fd, struct  sockaddr_in* addr )
{

    socklen_t  addrlen;
    int  connfd = ::accept4( fd, ( struct sockaddr*)(addr),
                                      &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    //Fixme: non blocking accept must be careful
    if ( connfd < 0 )
    {
        int lastError  = errno ;
        switch ( lastError )
        {
            case EAGAIN:
            case ECONNABORTED:
            case EPROTO:
            case EPERM:
            case EMFILE:
                errno = lastError;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                LOG_ERROR( "unexpected error :%d ", lastError );
                break;
            default:
                LOG_ERROR(" oh, my, god, This is what? %d ", lastError );
        }
    }

    return connfd;
}

int SocketHelper::connect( int fd, const struct sockaddr_in* addr )
{
    return  ::connect( fd, (const struct sockaddr*)addr, (socklen_t )( sizeof(addr) )  );
}

int SocketHelper::read( int fd, char* buf, int count )
{

    return ::read( fd, buf, count );

}

int SocketHelper::write( int fd, char* buf, int count )
{
    return ::write( fd, buf, count );
}

int SocketHelper::close( int fd )
{
    int r = ::close( fd );
    if( r < 0 ) LOG_ERROR("close socket:%d fail ", fd );

    return r;

}


int SocketHelper::shutdown( int fd )
{

    int r = ::shutdown( fd, SHUT_WR  );
    if( r < 0 ) LOG_ERROR(" shutdown  socket:%d fail ", fd );

    return r;

}












