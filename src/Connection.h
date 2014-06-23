/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _CONNECTION_H_
#define _CONNECTION_H_


class EventLoop;


class Connection
{
public:
    Connection( int fd,  EventLoop* loop );
    ~Connection();
public:
    int  onRead( );
    int  onWrite( );
    int  onClose( );
private:
    int         sockfd_;
    EventLoop*  loop_;
};

#endif // _CONNECTION_H_







