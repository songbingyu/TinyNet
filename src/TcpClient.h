/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef     _TCP_CLIENT_H_
#define     _TCP_SERVER_H_

#include <string>
#include "TinyDefine.h"

class EventLoop;
class Connector;
class Connection;

class    TcpClient
{
public:
    TcpClient( const char* ip, int port );
    ~TcpClient();
public:
    void init( );
    void connect();
    void stop();
    void disconnect();
    void onNewConn( int fd, void* data );
    void onRead( Connection* conn, int* data );
    void onWrite( Connection* conn, int* data );
    void onClose( Connection* conn, int* data );
private:

    std::string serverIp_;
    int         serverPort_;

    EventLoop*  loop_;
    bool        isConnect_;
    Connector*  connector_;
    Connection* conn_;

};

#endif  //     _TCP_SERVER_H_




