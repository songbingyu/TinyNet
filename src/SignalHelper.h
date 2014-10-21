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
    extern  SigMap        sigMaps;


    extern void addFeedSignal( int sigNum );

    extern void sigHandle( int sigNum );

    extern void pipeEventCb( EventLoop* loop, IEvent* ev, int revents );

    class SignalHelper
    {
        public:
            SignalHelper( EventLoop* loop );
            ~SignalHelper();
        public:
            tiny_forceinline void init();
            tiny_forceinline void addSignal( EventSignal* es );
            tiny_forceinline void delSignal( EventSignal* es );
            tiny_forceinline void pipeWrite();
            tiny_forceinline void onSignalEvent();
        private:
            EventIo*     ev_;
            EventLoop*   loop_;
            int          evPipe_[2];

    };

}


#endif //_SIGNAL_HELPER_H_

