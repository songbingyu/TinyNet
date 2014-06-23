/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef  _TCP_SERVER_H_
#define  _TCP_SERVER_H_

class Connection;

class  TcpAcceptor;
class  EventLoop;


class  TcpServer
{
public:
     TcpServer( int port = 4622 );
     ~TcpServer( );

public:
    void    run();
private:
    int     init();
public:
    int     onNewConnection( int fd, struct sockaddr_in& addr  );
    int     onConnection( Connection* conn );
    int     onRead ( Connection* conn );
    int     onWrite( Connection* conn );
    int     onClose( Connection* conn );
private:
    int                     port_;
    TcpAcceptor*            acceptor_;
    EventLoop*              loop_;
};

#endif // _TCP_SERVER_H_











