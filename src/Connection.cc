/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include<stddef.h>
#include"Log.h"
#include"EventLoop.h"
#include"Connection.h"


Connection::Connection( int fd, EventLoop* loop ): sockfd_( fd ),
                                                  loop_( loop)
{

}

Connection::~Connection()
{
    sockfd_  = -1;
    loop_ = NULL ;
}


int  Connection::onRead( )
{


}

int  Connection::onWrite( )
{

}


int  Connection::onClose( )
{

}



