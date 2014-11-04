/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include "TcpClient.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "Connector.h"
#include "EventLoop.h"

using namespace std::placeholders;

TcpClient::TcpClient( EventLoop*  loop, const char* ip, int port ): loop_(loop),
                                                                    serverIp_( ip ), serverPort_( port ),
                                                                    conn_(NULL)
{


}

TcpClient::~TcpClient()
{
    TINY_DELETE( connector_ );
}

void TcpClient::init()
{

    struct sockaddr_in addr;
    bzero( &addr, sizeof(addr) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( serverPort_ );
    if( inet_pton( AF_INET, serverIp_.c_str(), &addr.sin_addr ) <= 0 ){
        LOG_ERROR(" inet pton error, src:%s ", serverIp_.c_str() );
        exit(1);
        return;
    }

    //loop_ = new EventLoop();
    assert( loop_ != NULL );

    connector_ = new Connector( loop_, addr );
    assert( connector_ != NULL );

    connector_->setNewConnCb( std::bind( &TcpClient::onNewConn, this, _1, _2 ) );

    isConnect_ = true;
}

void TcpClient::run()
{
    loop_->run( EVRUN_ALWAYES );
}

void TcpClient::connect()
{
    isConnect_ = true;
    connector_->start();
}

void TcpClient::disconnect()
{
    isConnect_ = false;
    if( NULL != conn_ && conn_->isConnected() )
    {
        conn_->close();
        conn_ = NULL;
    }
}


void TcpClient::stop()
{
    isConnect_ = false;
    connector_->stop();
}

void TcpClient::onNewConn( int fd, struct sockaddr_in& addr  )
{

    Connection* conn  = new Connection( fd, loop_, addr );
    tiny_assert( NULL != conn );

    conn->setReadCallBack( readCallBack_  );
    conn->setConnCallBack( connCallBack_ );
    //conn->setWriteCallBack( std::bind( &TcpClient::onWrite, this, _1 ));
    conn->setCloseCallBack( std::bind( &TcpClient::onRemoveConnection, this, _1 ));


    conn->onConnFinish();

    conn_= conn;

}

void TcpClient::onRemoveConnection( Connection* conn )
{
    closeCallBack_(conn);
    conn->onConnDestory();
    TINY_DELETE( conn );
    conn_ = NULL;
}








