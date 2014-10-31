/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include "SignalHelper.h"
#include <unistd.h>
#include <errno.h>
#include "EventLoop.h"
#include "Event.h"
#include "Log.h"


using namespace Tiny;


namespace Tiny
{
    SigMap  sigMaps;
    void addSignal( EventSignal* es )
    {


    }


    void delSignal( EventSignal* es )
    {

    }

    void sigHandle( int sigNum )
    {
        //SigMap::iterator it = sigMaps.find( sigNum );
        ActiveSignalEvent&  ev = sigMaps[ sigNum ];
        if( ev.loop_ != NULL ){
            ev.loop_->addFeedSignal( sigNum );
        }
    }

    void pipeEventCb( EventLoop* loop, IEvent* ev, int revents )
    {
        EventIo* eio = (EventIo*)ev;
        if( revents&EV_READ ){
            char dummy[4];
            read( eio->getFd(), &dummy, sizeof(dummy) );
        }

        loop->onSignalEvent();
    }

    SignalHelper::SignalHelper( EventLoop* loop ):ev_( NULL ),loop_( loop )
    {
        evPipe_[0] = -1;
        evPipe_[1] = -1;
    }

    SignalHelper::~SignalHelper()
    {
        delete ev_;
    }

    void SignalHelper::init()
    {
        if( NULL != ev_ || ev_->isActive() ){
            return;
        }

        int fds[2];
        if( pipe( evPipe_ )== -1  ){

            perror("pipe");
            exit(0);
        }

        fdInteral( fds[0] );
        evPipe_[0] = fds[0];
        evPipe_[1] = fds[1];

        fdInteral(evPipe_[1]);

        ev_ = new EventIo( loop_, pipeEventCb, evPipe_[0], EV_READ );

        ev_->start();

        loop_->delActiveCnt();
    }

    void SignalHelper::addSignal( EventSignal* es )
    {
        sigMaps[es->getSigNum()].pending_ = 0;
        sigMaps[es->getSigNum()].loop_   = loop_;
        sigMaps[es->getSigNum()].addList((EventList*)es);
    }

    void SignalHelper::delSignal( EventSignal* es )
    {
        sigMaps[es->getSigNum()].pending_ = 0;
        sigMaps[es->getSigNum()].delList((EventList*)es);
        if( sigMaps[es->getSigNum()].head_ == NULL ){
            sigMaps[es->getSigNum()].loop_   = loop_;
        }
    }

    void SignalHelper::pipeWrite()
    {
        int oldErrno = errno;

        write( evPipe_[1], &(evPipe_[1]), 1);

        errno = oldErrno;

    }

    void SignalHelper::onSignalEvent()
    {
        SigMap::iterator it = sigMaps.begin();
        for( ; it != sigMaps.end(); ++it ){
            if( expect_false( it->second.pending_ ) ){
                it->second.pending_ = 0;
                it->second.addFeedEvent();
            }
        }
    }

}//namespace Tiny



























