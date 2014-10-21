/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _SIGNAL_HELPER_H_
#define _SIGNAL_HELPER_H_

#include <map>
#include "TinyDefine.h"
#include "Event.h"
#include "ActiveEvent.h"

class EventLoop;


namespace Tiny
{

    typedef std::map<int,ActiveSignalEvent>  SigMap;
    SigMap        sigMaps;


    extern void addSignal( EventSignal* es );
    extern void delSignal( EventSignal* es );
    extern void addFeedSignal( int sigNum );

    extern void sigHandle( int sigNum );

    extern void pipeEventCb( EventLoop* loop, int revents );

    class SignalHelper
    {
        public:
            SignalHelper();
            ~SignalHelper();
        public:
            void init();
        private:
            EventIo*     ev_;
            EventLoop*   loop_;
            int          evPipe_[2];

    };

}


#endif //_SIGNAL_HELPER_H_

