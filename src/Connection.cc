/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include<stddef.h>
#include <netinet/in.h>
#include"Log.h"
#include"EventLoop.h"
#include"Connection.h"


Connection::Connection( int fd, EventLoop* loop, struct sockaddr_in& addr  ): IConnection( fd, loop, addr )
{

}

Connection::~Connection()
{
    sockfd_  = -1;
    loop_ = NULL ;
}


int  Connection::onRead( )
{
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



