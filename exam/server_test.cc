#include<stdio.h>

#include"TcpServer.h"

#include "TcpClient.h"

#include "Event.h"
#include "EventLoop.h"

void onTimer( EventLoop* loop, IEvent* ev, int revents )
{
    LOG_INFO("onTimer");
}


int main( )
{
    //TcpServer  server;
    //

    //TcpClient client("182.92.218.149", 8001 );

   // client.init();

    //client.connect();

    //client.run();

    //server.run();
    //
    //


    EventLoop* loop = new EventLoop();

    EventTimer timer( loop, onTimer, 1,1 );

    timer.start();

    loop->run( EVRUN_ALWAYES );



}

















