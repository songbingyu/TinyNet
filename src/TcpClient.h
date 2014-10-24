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
struct sockaddr_in;

class    TcpClient
{
public:
    TcpClient( const char* ip, int port );
    ~TcpClient();
public:
    void init( );
    void run();
    void connect();
    void stop();
    void disconnect();
    void onNewConn( int fd, struct sockaddr_in& addr  );
    void onRemoveConnection( Connection* conn );
    void onConn();
    void onRead( Connection* conn );
    void onWrite( Connection* conn );
    void onClose( Connection* conn );
private:

    std::string serverIp_;
    int         serverPort_;

    EventLoop*  loop_;
    bool        isConnect_;
    Connector*  connector_;
    Connection* conn_;

};

#endif  //     _TCP_SERVER_H_




