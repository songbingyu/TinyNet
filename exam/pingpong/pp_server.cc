
#include <stdio.h>
#include "Log.h"
#include "TcpServer.h"


void onConn( Connection* conn )
{
    if( conn->isConnected() ){
        conn->setTcpNoDelay( true );
    }
}

void onRead( Connection* conn )
{
    Buffer* buffer = conn->getReadBuffer();
    conn->send( buffer->peek(), buffer->capacity() );
    buffer->retrieve(buffer->capacity() );
}

void onClose( Connection* conn )
{

}

int main( int argc, char* argv[] )
{
    if( argc < 2 ){
        printf("Usage: <port> ");
        return 0;
    }

    int port = (int)(atoi(argv[1]));

    TcpServer server(port);

    server.setConnCallBack( onConn );
    server.setReadCallBack( onRead );
    server.setCloseCallBack( onClose );

    server.run( EVRUN_ALWAYES );

}



