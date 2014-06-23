/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */


#include <assert.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "Log.h"
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


int EventEpoll::addEvent( )
{
    return 1;
}

int EventEpoll::delEvent()
{

    return 1;

}

int EventEpoll::modEvent()
{

    return 1;
}


int EventEpoll::waitEvent( int timeout, ConnectionVec*  activeConns )
{
#ifdef _DEBUG_
    assert( epollfd_ != NULL );
#endif
    int numEvents =  epoll_wait( epollfd_, &*events.begin(), (int)events.size(), timeout );
    if( numEvents > 0 )
    {

            assert( numEvents < events.size() );
            for( int i=0; i < numEvents; ++i )
            {
               IConnections* conn = (IConnections*)eventd_[i].data.ptr;

            }
    }
    else if ( numEvents == 0  )
    {
        LOG_INFO(" no events ");
    }
    else
    {

        LOG_INFO(" no events:%d "i, errno );
    }
    return 1;
}




