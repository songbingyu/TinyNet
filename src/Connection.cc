/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include<stddef.h>
#include <netinet/in.h>
#include"Log.h"
#include"EventLoop.h"
#include"Connection.h"


Connection::Connection( int fd, EventLoop* loop, struct sockaddr_in& addr  ): IConnection( fd, loop, addr ),
                                                                             state_( CS_No )
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

    if( readCallback_ )
    {
        readCallback_->callback( this );
    }
    return 1;
}

int  Connection::onWrite( )
{
    return 1;
}


int  Connection::onClose( )
{
    return 1;
}

int  Connection::onConnFinish()
{
    state_ = CS_Connected;
    enableRead();
    return 1;
}

