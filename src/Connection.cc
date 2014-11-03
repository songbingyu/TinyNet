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
                                                                        ev_(loop,Connection::onEvents, fd, EV_READ )
{

}

Connection::~Connection()
{


}

void Connection::send( char* data, int len )
{
    if( state_ != CS_Connected ) return;
    int nwrite = 0;
    int remaining = len;
    int writeError = false;


    if(  !ev_.isWriteing() && writeBuf_.capacity() ==0 ){
        nwrite = socketHelper_->write( sockfd_, data, len );
        if( nwrite >= 0  ){
            remaining = remaining - nwrite;
            if( remaining == 0 ){
                //Fixme: should callback?
            }
        }else {
            nwrite = 0;
            if( errno != EWOULDBLOCK ){
                if( errno == EPIPE || errno == ECONNRESET ){
                    writeError = true;
                }
            }
        }
    }

    if( remaining > 0 && !writeError ){
        int oldLen = writeBuf_.capacity();
        if( oldLen + remaining > writeBuf_.size() ){
            //Fixme : over
            LOG_ERROR("connection socket buff over, fd:%d ,events:%d", sockfd_, ev_.getEvents() );
        }

        writeBuf_.push( data+nwrite, remaining );

        if( !ev_.isWriteing() ){
            ev_.changeEvents( ev_.getEvents()&EV_WRITE );
        }
    }

}

void Connection::close()
{
    if( state_ == CS_Connecing || state_ == CS_Connected ){
        state_ = CS_DisConnecting;
            onClose();
    }
}

int  Connection::onRead( )
{

    //read shoule do what?
    int ret = readBuf_.push(socketHelper_, sockfd_ );
    if( ret < 0  )
    {
        onClose();
    }

    readCallback_( this );
    return 1;
}

int  Connection::onWrite( )
{
    if( ev_.isWriteing() ){

    }
    //writeCallback_(this);
    return 1;
}


int  Connection::onClose( )
{
    state_ = CS_DisConnected;
    ev_.stop();

#ifdef _DEBUG_
    assert( NULL != closeCallBack_ );
#endif

    closeCallBack_( this );
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
    ev_.start();
    return 1;
}

int Connection::onConnDestory()
{
    if( state_ == CS_Connected ){
        state_ = CS_DisConnected;
        ev_.stop();
    }
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




