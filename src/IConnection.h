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
    IConnection( int fd, EventLoop* loop, struct sockaddr_in&  addr ): sockfd_( fd ),
                                                                     loop_( loop ),
                                                                     addr_( addr )
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
    int                 sockfd_;
    EventLoop*          loop_;
    struct sockaddr_in  addr_;
    SocketHelper*       socketHelper_;
};

#endif // _ICONNECTION_H_




