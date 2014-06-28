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



EventEpoll::EventEpoll(): epollfd_(-1 ),
                           events_(100)
{
    epollCreate() ;
}

EventEpoll::~EventEpoll()
{

    close( epollfd_ );
}

int EventEpoll::epollCreate()
{

    epollfd_ = epoll_create1( EPOLL_CLOEXEC );
    if( epollfd_ < 0 )
    {
        LOG_ERROR( " epoll create fail ... " );
        return -1;
    }

    return 0;
}

const char* eventStateToString( EventState  es )
{
    switch( es )
    {
        case ES_New:
            return "ES_New";
        case ES_Added:
            return "ES_Added";
        case ES_Del:
            return "ES_Del";
        default:
            return "MyGod";

    }
}


int EventEpoll::updateEvent( IConnection* conn )
{
    EventState st = conn->getEventState();

    LOG_INFO(" updtate event to epoll :%s ", eventStateToString( st ) );
    int fd = conn->getSockFd();

    if( st == ES_New )
    {
#ifdef  _DEBUG_
        ConnectionMap::iterator  it = connectionMap.find( fd );
        if ( it != connectionMap.end() )
        {
            connectionMap[ fd ] = conn;
        }
        LOG_ERROR("fd:%d has addded to map!")
#endif  //_DEBUG_

        //Fixme should check?
        //assert( !conn->isNoneEvent() );

        connectionMap[ fd ] = conn;
        conn->setEventState( ES_Added );
        epollCtl( EPOLL_CTL_ADD, conn );
    }
    else if( st == ES_Added )
    {
#ifdef  _DEBUG_
        ConnectionMap::iterator  it = connectionMap.find( fd );
        assert( it != connectionMap.end() );
        assert( it->second == conn );
#endif //_DEBUG_

        //Fixeme: shoule process events_ == 0 ?
        if( conn->isNoneEvent() )
        {
            conn->setEventState( ES_Del );
            epollCtl( EPOLL_CTL_DEL, conn );
        }
        else
        {
            epollCtl( EPOLL_CTL_MOD, conn );
        }
    }
    else// ES_DEL
    {

#ifdef  _DEBUG_
        ConnectionMap::iterator  it = connectionMap.find( fd );
        assert( it != connectionMap.end() );
        assert( it->second == conn );
#endif //_DEBUG_
        conn->setEventState( ES_Del );
        epollCtl( EPOLL_CTL_DEL, conn );

    }

    return 1;
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

    LOG_INFO("Del event from epoll:%d ", fd );

    ConnectionMap::iterator  it = connectionMap.find( fd );
    if ( it == connectionMap.end() )
    {
        LOG_ERROR( " not find socket %d in connectionMap ", fd );
        return 1;
    }

    if( it->second != conn )
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
    if( ret ==  -1  )
    {
        // consider from libevent2
        if( oper == EPOLL_CTL_MOD && errno == ENOENT )
        {
            if( epoll_ctl( epollfd_ , EPOLL_CTL_ADD, sockfd, &event ) == -1 )
            {
                LOG_ERROR(" EPOLL mod:%d on %d retry on ADD that fail ",
                                                conn->getEvents(), sockfd);
            }
            else
            {
                LOG_ERROR(" EPOLL mod:%d on %d retry on ADD success ",
                                                conn->getEvents(), sockfd );
            }
        }
        else if( oper == EPOLL_CTL_ADD && errno == EEXIST )
        {
            if( epoll_ctl( epollfd_ , EPOLL_CTL_MOD, sockfd, &event ) == -1 )
            {
                LOG_ERROR(" EPOLL add :%d on %d retry on mod that fail ",
                                                    conn->getEvents(), sockfd);
            }
            else
            {
                LOG_ERROR(" EPOLL add :%d on %d retry on mod success ",
                                                    conn->getEvents(), sockfd );
            }

        }
        else if( oper == EPOLL_CTL_DEL &&
                ( errno == ENOENT || errno == EBADF || errno == EPERM ) )
        {
            LOG_ERROR(" EPOLL del:%d in %d fail ",
                                            conn->getEvents(), sockfd );
        }
        else
        {

            LOG_ERROR(" epool_ctl faila...:%d, fd:%d, events:%d ", oper, sockfd, conn->getEvents() );
        }
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
        //LOG_INFO(" no events ");
    }
    else
    {

        LOG_INFO(" error events:%d ", errno );
    }
    return 1;
}




