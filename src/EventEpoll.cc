/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */


#include "EventEpoll.h"
#include <assert.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "Log.h"
#include "Event.h"
#include "EventLoop.h"
#include "ActiveEvent.h"


#define  EV_EPERM   0x80

EventEpoll::EventEpoll( EventLoop* loop ): IPoller( loop ), epollfd_( -1 ),epollEventMax_(128),events_( epollEventMax_ )
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


void  EventEpoll::updateEvent( int fd, int oev, int nev )
{
    LOG_INFO(" updtate event to epoll fd :%d old event :%d new event: %d  ", fd, oev, nev );

    //libev
    if( !nev )   return;

    struct epoll_event  ev;
    memset( &ev, 0, sizeof(ev) );

    ActiveFdEvent*  activeEv  = loop_->getActiveFdEventByFd( fd );
    int oldRevents = activeEv->revents_;
    activeEv->revents_ = nev;

    ev.data.u64  = (uint64_t)(uint32_t)fd |
                (uint64_t)(uint32_t)++activeEv->egen_ << 32;
    ev.events    = ( nev & EV_READ ? EPOLLIN : 0 )
                | ( nev & EV_WRITE ? EPOLLOUT : 0 );

    if( expect_true( !epoll_ctl( epollfd_, oev&&oldRevents != nev ? EPOLL_CTL_MOD: EPOLL_CTL_ADD, fd, &ev) ) ){
        return;
    }

    if( expect_true( errno == ENOENT ) ) {
        if( !nev ) {
            goto  dec_egen;
            if( !epoll_ctl( epollfd_, EPOLL_CTL_ADD, fd, &ev ) ) {
                return;
            }
        }
    } else if( expect_true( errno == EEXIST ) ) {
        if( oldRevents == nev ) {
            goto  dec_egen;
        }

        if( !epoll_ctl( epollfd_, EPOLL_CTL_MOD, fd, &ev ) ) {
            return;
        }
    } else if( expect_true( errno == EPERM ) ) {
        activeEv->revents_ = EV_EPERM;

        if( !( oldRevents & EV_EPERM ) ){
            this->epermFds_.push_back( fd );
        }
        return;
    }

    //This is a error
    activeEv->killFd( loop_ );

dec_egen:

    activeEv->egen_--;


    return ;
}

void  EventEpoll::waitEvent( Timestamp ts )
{
#ifdef _DEBUG_
   // assert( epollfd_ != NULL );
#endif

    if( expect_false( epermFds_.size() ) ) {
        ts = 0;
    }

    int numEvents =  epoll_wait( epollfd_, &*events_.begin(), (int)events_.size(), ts*1e3 );

    if( expect_false( numEvents < 0 ) ) {
        if( errno != EINTR ){
            LOG_ERROR(" epoll_wait error");
        }
    }

    for ( int i=0; i < numEvents;  ++i ) {

        struct  epoll_event*  ev = &events_[i];
        int     fd      = (uint32_t)ev->data.u64;
        ActiveFdEvent*  activeEv =  loop_->getActiveFdEventByFd( fd );

        int     want    =  activeEv->events_;

        int     got     =  ( ev->events &( EPOLLOUT|EPOLLERR|EPOLLHUP)? EV_WRITE : 0 )
                        |  ( ev->events &(EPOLLIN|EPOLLERR|EPOLLHUP)? EV_READ : 0 );

        if( expect_false((uint32_t)activeEv->egen_ != (uint32_t)( ev->data.u64>>32 ))){
            continue;
        }

        if( expect_false( got & ~want )) {

            activeEv->revents_ = want;

            ev->events = ( want & EV_READ ? EPOLLIN  : 0 )
                        |( want & EV_WRITE? EPOLLOUT : 0 );
            if( epoll_ctl( epollfd_, want? EPOLL_CTL_MOD:EPOLL_CTL_DEL, fd, ev )){
                continue;
            }
        }
        activeEv->fdEvent( loop_, got );
    }

    if( expect_false( numEvents == epollEventMax_ )) {
        //TODO: find a suitable size ?
        epollEventMax_ *= 2;
        events_.resize( epollEventMax_ );
    }

    int epermCnt = epermFds_.size();

    for( int i= epermCnt-1; i>=0;  --i ){
        int fd = epermFds_[i];
        ActiveFdEvent*  activeEv =  loop_->getActiveFdEventByFd( fd );
        int events  =   activeEv->events_&( EV_READ | EV_WRITE );
        if( activeEv->revents_ & EV_EPERM && events ){
            activeEv->fdEvent( loop_, events );
        } else {
            epermFds_[i]= epermFds_[ epermFds_.size()-1];
            epermFds_.pop_back();
        }
    }

}




