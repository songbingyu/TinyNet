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
class IConnection : public nocopyable
{

public:
    IConnection(int fd, EventLoop* loop): sockfd_(fd), loop_(loop)
    {
        socketHelper_ = new SocketHelper();

    }

    ~IConnection()
    {
        //Fixme: should close socket ?
        socketHelper_->close(sockfd_);
        sockfd_ = -1;
        loop_   = NULL;

        delete socketHelper_;
        socketHelper_ =  NULL;
    }

public:
    int     getSockFd() const      {	return sockfd_ ;    }
protected:
    int                 sockfd_;
    EventLoop*          loop_;
    SocketHelper*       socketHelper_;
};

#endif // _ICONNECTION_H_





