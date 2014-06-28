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
    enableRead();
}


TcpAcceptor::~TcpAcceptor()
{

    delete  newConnCallBack_;
    newConnCallBack_ = NULL;

}


int TcpAcceptor::bindAndListen( )
{

    //Fixme: add  socket option

    if ( socketHelper_->bind( sockfd_,  &localAddr_ )  < 0 )
    {
        exit(1);
    }

    LOG_INFO(" bind socket success ");


    if( socketHelper_->listen( sockfd_ ) < 0 )
    {
        exit(1);
    }

    LOG_INFO(" listen  socket success ");

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

