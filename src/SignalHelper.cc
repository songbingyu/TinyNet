/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include "SignalHelper.h"
#include <unistd.h>
#include "EventLoop.h"


using namespace Tiny;

void addSignal( EventSignal* es )
{


}


void delSignal( EventSignal* es )
{

}

void sigHandle( int sigNum )
{
    ActiveSignalEvent&  ev = sigMaps[ sigNum ];

}


SignalHelper::SignalHelper():ev_( NULL )
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

    ev_ = new EventIo( pipeEventCb, evPipe_[1], EV_READ );

    ev_->start( loop_ );

    loop_->delActiveCnt();
}




























