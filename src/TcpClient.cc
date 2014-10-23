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


TcpClient::TcpClient( const char* ip, int port ): serverIp_( ip ), serverPort_( port )
{


}

TcpClient::~TcpClient()
{


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

    connctor_ = new Connector( loop_, addr );
    assert( connector_ != NULL );

    connector_->setNewConnCb( new NewConnCallBack( this, &TcpClient::onNewConn) );

    isConnect_ = true;
}

void TcpClient::connect()
{

}

void TcpClient::disconnect()
{

}


void TcpClient::stop()
{

}

void TcpClient::onNewConn()
{

}

void TcpClient::onRead( Connection* conn, int* data )
{

}

void TcpClient::onWrite( Connection* conn, int* data )
{

}

void TcpClient::onClose( Connection* conn, int* data )
{

}

