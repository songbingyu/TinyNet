/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _ICONNECTION_H_
#define _ICONNECTION_H_

#include <netinet/in.h>
#include <sys/epoll.h>
#include "nocopyable.h"
#include "EventLoop.h"
#include "SocketHelper.h"

// you know Connection must be  not copy
class IConnection: public nocopyable
{

public:
    IConnection( int fd, EventLoop* loop ): sockfd_( fd ),loop_( loop )
    {
        socketHelper_ = new SocketHelper();

    }

    ~IConnection( )
    {
        //Fixme: should close socket ?
        socketHelper_->close( sockfd_ );
        sockfd_ = -1;
        loop_   = NULL;

        delete socketHelper_;
        socketHelper_ =  NULL;
    }

public:
    int     getSockFd( ) const      { return sockfd_ ;    }
    void    setEvents( int ev )     { events_ = ev;       }
    int     getEvents( ) const      { return events_;     }
    void    setReadEvents( int ev ) { readEvents_ = ev;   }
    int     getReadEvents( ) const  { return readEvents_; }
    void    enableRead( )           { events_ |= ( EPOLLIN | EPOLLPRI ); updateEvent();  }
    void    enableWrite( )          { events_ |=  EPOLLOUT;              updateEvent();  }
    void    removeEvent( )          { events_ = 0 ; delEvent();             }
    bool    isNoneEvent( )          {  return events_ == 0 ;                        }
private:
    // add mod
    void    updateEvent( )          { /*loop_->updateEvent( this );*/           }
    // del
    void    delEvent( )             { /*loop_->delEvent( this );*/              }
protected:
    int                 sockfd_;
    EventLoop*          loop_;
    SocketHelper*       socketHelper_;
    int                 events_;
    int                 readEvents_;
};

#endif // _ICONNECTION_H_





