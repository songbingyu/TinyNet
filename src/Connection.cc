/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include <stddef.h>
#include <netinet/in.h>
#include "Log.h"
#include "EventLoop.h"
#include "Connection.h"


Connection::Connection( int fd, EventLoop* loop, struct sockaddr_in& addr  ): IConnection( fd, loop ),
                                                                        ev_(Connection::onEvents, fd, EV_READ )
{

}

Connection::~Connection()
{

    delete readCallback_;
    readCallback_  = NULL;

    delete  writeCallback_;
    writeCallback_ = NULL;

    delete  closeCallBack_;
    closeCallBack_ = NULL;

}


int  Connection::onRead( )
{

    //read shoule do what?
    int ret = readBuf_.push(socketHelper_, sockfd_ );
    if( ret < 0  )
    {
        onClose();
    }

    if( readCallback_ )
    {
        readCallback_->callback( this, NULL );
    }
    return 1;
}

int  Connection::onWrite( )
{
    if( writeCallback_ )
    {
         writeCallback_->callback( this, NULL );
    }
    return 1;
}


int  Connection::onClose( )
{
    state_ = CS_DisConnected;
    removeEvent();
#ifdef _DEBUG_
    assert( NULL != closeCallBack_ );
#endif

    if( NULL != closeCallBack_ )
    {
        closeCallBack_->callback( this, NULL );
    }

    return 1;
}

int   Connection::onError( )
{
    //TODO: see nginx what ?
    return 1;
}

int  Connection::onConnFinish()
{
    state_ = CS_Connected;
    ev_.setUserData( (void*)this );
    ev_.start( loop_ );
    return 1;
}

void Connection::onEvents( EventLoop* loop, IEvent* ev, int revents )
{
    Connection* conn = (Connection*)ev->getUserData();
    if( NULL != conn ){
        if( revents&EV_READ  ){

            conn->onRead();

        }else if( revents&EV_WRITE ){

            conn->onWrite();

        }else if( revents&EV_ERROR ){

            conn->onError();
        }else {

            LOG_ERROR("conn :%d unknow event:%d ", conn->getSockFd(),revents );
        }
    }
}
