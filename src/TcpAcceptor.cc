/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */


#include <assert.h>
#include <netinet/in.h>
#include "EventLoop.h"
#include "Log.h"
#include "TcpServer.h"
#include "Connection.h"
#include "TcpAcceptor.h"



TcpAcceptor::TcpAcceptor( int fd, EventLoop* loop, struct sockaddr_in&  addr ): IConnection( fd, loop, addr )
{

    bindAndListen();
}


TcpAcceptor::~TcpAcceptor()
{


}


int TcpAcceptor::bindAndListen( )
{

    if ( socketHelper_->bind( sockfd_,  &addr_ )  < 0 )
    {
        exit(1);
    }

    if( socketHelper_->listen( sockfd_ ) < 0 )
    {
        exit(1);
    }

    isListening_ = true;

    return  1;
}



int  TcpAcceptor::onRead( )
{
     assert( socketHelper_ != NULL  );

     struct sockaddr_in addr ;
     int newfd = socketHelper_->accept( sockfd_, &addr );
     if( newfd  >= 0 )
     {
         if( NULL != newConnCallBack_ )
         {
             newConnCallBack_->callback( &newfd, &addr  );
         }
     }
     else
     {
         //LOG_ERROR(" connect socket faial " );
     }

     return 1;
}

