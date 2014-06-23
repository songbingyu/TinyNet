/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _ICONNECTION_H_
#define _ICONNECTION_H_

#include <netinet/in.h>
#include <sys/epoll.h>
#include "EventLoop.h"
#include "SocketHelper.h"


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
    int     getSockFd( ) const      { return sockfd_ ;    }
    void    setEvents( int ev )     { events_ = ev;       }
    int     getEvents( ) const      { return events_;     }
    void    setReadEvents( int ev ) { readEvents_ = ev;   }
    int     getReadEvents() const   { return readEvents_; }
    void    enableRead( )           { events_ |= ( EPOLLIN | EPOLLPRI ); }
    void    enableWrite( )          { events_ |=  EPOLLOUT; update();    }
    void    update( )               { loop_->update( this );             }
protected:
    int                 sockfd_;
    EventLoop*          loop_;
    struct sockaddr_in  localAddr_;
    SocketHelper*       socketHelper_;
    //struct sockaddr_in  peerAddr_;
    int                 events_;
    int                 readEvents_;

};

#endif // _ICONNECTION_H_




