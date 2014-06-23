/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */


#include <assert.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "Log.h"
#include "IConnection.h"
#include "EventEpoll.h"



EventEpoll::EventEpoll( ): epollfd_(-1 ),
                           events_(100)
{
    epollCreate() ;
}

EventEpoll::~EventEpoll()
{

    close( epollfd_ );
}

int EventEpoll::epollCreate( )
{

    epollfd_ = epoll_create1( EPOLL_CLOEXEC );
    if( epollfd_ < 0 )
    {
        LOG_ERROR( " epoll create fail ... " );
        return -1;
    }

    return 0;
}


int EventEpoll::addEvent( IConnection* conn  )
{
    int fd = conn->getSockFd();

    if( conn->getEvents() == 0  )
    {
        LOG_ERROR( " socket %d not set events", fd );
        return 1;
    }


    ConnectionMap::iterator  it = connectionMap.find( fd );
    if ( it != connectionMap.end() )
    {
        connectionMap[ fd ] = conn;
    }

    epollCtl( EPOLL_CTL_ADD, conn );

    return 1;
}

int EventEpoll::delEvent( IConnection* conn )
{

    int fd = conn->getSockFd();
    ConnectionMap::iterator  it = connectionMap.find( fd );
    if ( it == connectionMap.end() )
    {
        LOG_ERROR( " not find socket %d in connectionMap ", fd );
        return 1;
    }

    if( it->second == conn )
    {
        LOG_ERROR( " I don't know what happen ?  ", fd );
        return 1;
    }
    connectionMap.erase( it );

    epollCtl( EPOLL_CTL_DEL, conn );

    return 1;

}



int EventEpoll::epollCtl( int oper, IConnection* conn )
{
    struct epoll_event event;
    bzero( &event, sizeof(event) );
    event.events   = conn->getEvents();
    event.data.ptr = conn;
    int sockfd     = conn->getSockFd();

    int ret = epoll_ctl( epollfd_, oper, sockfd, &event );
    if( ret < 0 )
    {
        LOG_ERROR(" epool_ctl faila...:%d ", oper );
    }

    return 1;
}

int EventEpoll::waitEvent( int timeout, std::vector<IConnection*>*  activeConns )
{
#ifdef _DEBUG_
    assert( epollfd_ != NULL );
#endif
    int numEvents =  epoll_wait( epollfd_, &*events_.begin(), (int)events_.size(), timeout );
    if( numEvents > 0 )
    {

            assert( numEvents < (int)events_.size() );
            for( int i=0; i < numEvents; ++i )
            {
                IConnection* conn = (IConnection*)events_[i].data.ptr;
                conn->setReadEvents( events_[i].events );
                activeConns->push_back( conn );
            }
    }
    else if ( numEvents == 0  )
    {
        LOG_INFO(" no events ");
    }
    else
    {

        LOG_INFO(" no events:%d ", errno );
    }
    return 1;
}




