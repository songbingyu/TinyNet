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
#include "Connection.h"
#include "EventLoop.h"

using namespace std::placeholders;

TcpClient::TcpClient( const char* ip, int port ): serverIp_( ip ), serverPort_( port )
{


}

TcpClient::~TcpClient()
{
    TINY_DELETE( connector_ );
    TINY_DELETE(conn_);
    TINY_DELETE( loop_ );

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


    loop_ = new EventLoop();
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

}


void TcpClient::stop()
{
    isConnect_ = false;
    connector_->stop();
}

void TcpClient::onNewConn( int fd, struct sockaddr_in& addr  )
{

    Connection* conn  = new Connection( fd, loop_, addr );

    conn->setReadCallBack( std::bind( &TcpClient::onRead, this, _1 ) );
    conn->setWriteCallBack( std::bind( &TcpClient::onWrite, this, _1 ));
    conn->setCloseCallBack( std::bind( &TcpClient::onRemoveConnection, this, _1 ));

    onConn();

    conn->onConnFinish();

}

void TcpClient::onRemoveConnection( Connection* conn )
{

}

void TcpClient::onRead( Connection* conn  )
{
    LOG_INFO("on read ");
}

void TcpClient::onWrite( Connection* conn )
{

    LOG_INFO("on write ");
}

void TcpClient::onClose( Connection* conn )
{

    LOG_INFO("on close ");
}

void TcpClient::onConn()
{
    LOG_INFO("on conn ");
}






