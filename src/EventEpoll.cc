/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include <sys/epoll.h>
#include "EventEpoll.h"



EventEpoll::EventEpoll( ): epollfd_(-1 ),
                            events_(100)
{

}

EventEpoll::~EventEpoll()
{

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

}

int EventEpoll::delEvent()
{


}

int EventEpoll::modEvent()
{

}


int EventEpoll::waitEvent()
{

}




