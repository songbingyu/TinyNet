/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */



struct sockaddr_in;


//socket op wraper.
class   SocketHelper
{
public:
    int     createNonBlockingSocket( );
    int     bind( int fd, sockaddr_in*  addr );
    int     listen( int fd );
    //TODO: accept4 ?
    int     accept( int fd, sockaddr_in* addr );
    int     connect int fd, sockaddr_in* addr );
    int     read( int fd, char* buf, int len );
    int     write( int fd, char* buf, int len );
    int     shutdown( int fd );
//TODO readv writev

}



