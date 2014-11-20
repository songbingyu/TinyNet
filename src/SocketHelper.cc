/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>// for TCP_NODELAY
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include "Log.h"
#include "SocketHelper.h"

int SocketHelper::createNonBlockingSocket( )
{
    int fd = ::socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (fd < 0) LOG_ERROR( "create socket fail i. errno:%d ", errno);
    return fd;
}

int SocketHelper::bind(int fd, const struct sockaddr_in* addr)
{
    int r = ::bind(fd, (const struct sockaddr*)addr, (socklen_t)sizeof(*addr));
    if (r < 0) LOG_ERROR( " bind fail errno:%d ", errno);

    return r;
}

int SocketHelper::listen(int fd)
{
    //Fixme: backlog sure?
    int r  = ::listen(fd, 50);
    if (r < 0) LOG_ERROR( "listen  fail ");

    return r;

}


int SocketHelper::accept(int fd, struct  sockaddr_in* addr)
{

    socklen_t  addrlen;
    int  connfd = ::accept4(fd, (struct sockaddr*)(addr),
                                      &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    //Fixme: non blocking accept must be careful
    if (connfd < 0) {
        int lastError  = errno ;
        switch (lastError)
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

int SocketHelper::connect(int fd, const struct sockaddr_in* addr)
{
    return  ::connect(fd, (const struct sockaddr*)addr, (socklen_t)(sizeof(*addr)));
}

int SocketHelper::read(int fd, char* buf, int count)
{

    //TODO: readv ?
    return ::read(fd, buf, count);

}

int SocketHelper::readv(int fd, const struct iovec* iov, int iovcnt)
{
    return ::readv(fd, iov, iovcnt);
}

int SocketHelper::write(int fd, char* buf, int count)
{
    //TODO:writev ?
    return ::write(fd, buf, count);
}

int SocketHelper::close(int fd)
{
    int r = ::close(fd);
    if (r < 0) LOG_ERROR("close socket:%d fail ", fd);

    return r;

}


int SocketHelper::shutdown( int fd )
{

    int r = ::shutdown(fd, SHUT_WR);
    if (r < 0) LOG_ERROR(" shutdown  socket:%d fail ", fd);

    return r;

}

int SocketHelper::setTcpNoDelay(int fd, bool isOpen)
{
    int optval = isOpen ? 1 : 0;
    int ret =  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof( optval ));
    if (ret < 0) {
        LOG_ERROR(" set socket[%d] TCP_NODELAY fail, errno:%d", fd,errno);
    }

    return 1;
}

int SocketHelper::setReuseAddr( int fd, bool isOpen )
{
    int optval = isOpen ? 1 : 0;
    int ret = setSocketOpt(fd, SO_REUSEADDR, optval);
    if (ret < 0) {
        LOG_ERROR(" set sockt[%d] SO_REUSEADDR fail, errno:%d", fd, errno);
    }

    return 1;

}

int SocketHelper::setReusePort(int fd, bool isOpen)
{
    int optval = isOpen ? 1 : 0;
    int ret = setSocketOpt( fd, SO_REUSEPORT, optval );
    if( ret < 0 ) {
        LOG_ERROR(" set socket[%d] SO_REUSEPORT fail, errno:%d", fd,errno );
    }

    return 1;

}

int SocketHelper::setKeepAlive(int fd, bool isOpen)
{
    int optval = isOpen ? 1 : 0;
    int ret = setSocketOpt(fd, SO_KEEPALIVE, optval);
    if (ret < 0) {
        LOG_ERROR(" set socket[%d] SO_KEEPALIVE fail, errno:%d", fd,errno);
    }
    return 1;
}

int SocketHelper::getSocketError(int fd )
{
    int val;
    socklen_t len = (socklen_t)sizeof(val);
    if (getsockopt( fd, SOL_SOCKET, SO_ERROR, &val, &len )) {
        return errno;
    } else {
        return  val;
    }
}

struct sockaddr_in SocketHelper::getLocalAddr( int fd )
{
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    socklen_t len = (socklen_t)sizeof(local);

    if (getsockname( fd, ( struct sockaddr* )&local, &len)) {
        LOG_ERROR(" getsocketname fail ");
    }

    return local;
}

struct sockaddr_in SocketHelper::getPeerAddr(int fd)
{
    struct sockaddr_in peer;
    memset(&peer, 0, sizeof(peer));

    socklen_t len = (socklen_t)sizeof( peer );

    if (getpeername(fd, ( struct sockaddr*)&peer, &len)) {
        LOG_ERROR(" getsocketname fail ");
    }

    return peer;

}

bool SocketHelper::isSelfConnect(int fd)
{
    struct sockaddr_in local = getLocalAddr(fd);
    struct sockaddr_in  peer = getPeerAddr(fd);

    return local.sin_port == peer.sin_port &&
        local.sin_addr.s_addr == peer.sin_addr.s_addr;
}

int SocketHelper::setSocketOpt(int fd, int optname, int& optval)
{
    return setsockopt(fd, SOL_SOCKET, optname, &optval, sizeof(optval));
}









