/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include <sys/epoll.h>
#include "Log.h"
#include "IConnection.h"
#include "EventLoop.h"

EventLoop::EventLoop(): isRuning_( true )
{
    eventEpoll_.epollCreate();
}

EventLoop::~EventLoop()
{

    activeConnections_.clear();

}

int  EventLoop::run()
{

    while( isRuning_  )
    {

        activeConnections_.clear();
        //Fixme: what time ?
        eventEpoll_.waitEvent( 10, &activeConnections_ );

        ConnectionVec::iterator it = activeConnections_.begin();

        for( ; it != activeConnections_.end(); ++it )
        {
            IConnection* conn = *it;
            if( NULL != conn )
            {
                // begin
                int revents = conn->getReadEvents();
                if( ( revents & EPOLLHUP) && !( revents& EPOLLIN )  )
                {
                     conn->onClose();
                }

                if( revents & ( EPOLLERR ) )
                {
                    conn->onError();
                }

                if( revents & ( EPOLLIN | EPOLLPRI | EPOLLRDHUP ))
                {
                    conn->onRead();
                }

                if( revents &  EPOLLOUT )
                {
                    conn->onWrite();
                }

            }
            else
            {
                //TODO should do what?
                LOG_ERROR( " connection is null " );
            }

        }
    }

    return 1;

}




