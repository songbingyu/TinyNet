/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _ICONNECTION_H_
#define _ICONNECTION_H_

#include"SocketHelper.h"


class   EventLoop;

class IConnection
{

public:
    IConnection( int fd, EventLoop* loop, struct sockaddr_in&  localaddr ): sockfd_( fd ),
                                                                            loop_( loop ),
                                                                            localAddr_( localaddr )
    {

        socketHelper_ = new SocketHelper();

    }
    ~IConnection( )
    {
        //Fixme: should close socket ?

        sockfd_ = -1;
        loop_ = NULL;

        delete socketHelper_;
        socketHelper_ =  NULL;

    }

public:
    virtual int  onRead( ) {  return 1; }
    virtual int  onWrite( ){  return 1; }
    virtual int  onClose( ){  return 1; }

public:
    int     getSockFd( ) const { return sockfd_ ; }
protected:
    int                 sockfd_;
    EventLoop*          loop_;
    struct sockaddr_in  localAddr_;
    SocketHelper*       socketHelper_;
    struct sockaddr_in  peerAddr_;
};

#endif // _ICONNECTION_H_




