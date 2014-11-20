/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _SOCKET_HELPER_H_
#define _SOCKET_HELPER_H_

#include "TinyDefine.h"

struct sockaddr_in;

//socket op wraper.
class   SocketHelper
{
public:
    static  int		createNonBlockingSocket();
    static int		bind(int fd, const struct sockaddr_in*  addr);
    static int		listen(int fd);
    //TODO: accept4 ?
    static int		accept(int fd,struct  sockaddr_in* addr);
    static int		connect(int fd, const struct sockaddr_in* addr);
    int				read(int fd, char* buf, int len);
    tiny_hot int    write(int fd, char* buf, int len);
    //TODO  readv writev
    tiny_hot int    readv(int fd, const struct iovec* iov, int iovcnt);

    int				close(int fd);
    static int		shutdown(int fd);

    //set socket
    static  int     setTcpNoDelay(int fd,bool isOpen);
    static  int     setReuseAddr (int fd,bool isOPen);
    static  int     setReusePort (int fd,bool isOPen);
    static  int     setKeepAlive (int fd,bool isOpen);

    static int      getSocketError(int fd);
    static struct sockaddr_in   getLocalAddr(int fd);
    static struct sockaddr_in   getPeerAddr(int fd);
    static bool     isSelfConnect(int fd);
private:
    static int      setSocketOpt(int fd,int optname, int& optval);
private:
    int fd_;
};

#endif // _SOCKET_HELPER_H_



