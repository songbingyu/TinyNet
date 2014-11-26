/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */


#include "TcpAcceptor.h"
#include <assert.h>
#include <netinet/in.h>
#include "TinyDefine.h"
#include "Log.h"
#include "Log.h"
#include "EventLoop.h"
#include "TcpServer.h"
#include "Connection.h"
#include "Event.h"



TcpAcceptor::TcpAcceptor(int fd, EventLoop* loop, struct sockaddr_in&  addr): IConnection(fd, loop),
                                                                                localAddr_(addr),
                                                                                ev_(loop,TcpAcceptor::onEvents, fd, EV_READ)
{

    bindAndListen();
}


TcpAcceptor::~TcpAcceptor()
{

    delete  newConnCallBack_;
    newConnCallBack_ = NULL;

}


int TcpAcceptor::bindAndListen()
{

    //Fixme: add  socket option

    if ( socketHelper_->bind(sockfd_,  &localAddr_ ) < 0 ) {
        exit(1);
    }

    LOG_INFO(" bind socket success ");


    if (socketHelper_->listen(sockfd_) < 0) {
        exit(1);
    }

    LOG_INFO(" listen  socket success ");

    isListening_ = true;

    ev_.setUserData((void*)this);
    ev_.start();

    return  1;
}



int  TcpAcceptor::onRead( )
{
     assert(socketHelper_ != NULL );
     struct sockaddr_in addr ;
     int newfd = socketHelper_->accept(sockfd_, &addr);
     if (newfd >= 0) {
         if (NULL != newConnCallBack_) {
             newConnCallBack_->callback(&newfd, &addr);
         }
     } else {
         LOG_ERROR("Acceptor accpet new conn fail:%d ", errno);
     }

     return 1;
}

void TcpAcceptor::onEvents(EventLoop* loop, IEvent* ev, int revents_)
{
    TcpAcceptor* acceptor = (TcpAcceptor*)ev->getUserData();
    if (revents_&EV_READ) {
        acceptor->onRead();
    } else {
        LOG_ERROR("EventIo receieve event:%d not EV_READ ");
    }
}


