/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include "Connector.h"
#include <errno.h>
#include "Log.h"
#include "Event.h"

Connector::Connector( EventLoop* loop, struct sockaddr_in& addr ):loop_( loop ),localAddr_( addr ),
                                                                 connect_( false ),
                                                                 state_( kDisConnect ),
                                                                 ev_( NULL ),
                                                                 retryCnt_(0)

{

}

Connector::~Connector()
{
    stop();
}


void Connector::start()
{
    tiny_assert( state_ == kDisConnect );
    connect_ = true;
    state_ = kDisConnect;
    connect();
}

void Connector::restart()
{

}

void Connector::stop()
{
    if( expect_true( state_ != kDisConnect ) ){
        connect_ = false;
        state_   = kDisConnect;
        delEvent();
     }
}

void Connector::delEvent( )
{
    if( expect_true( NULL != ev_ )){

        ev_->stop( loop_ );
        TINY_DELETE( ev_ );
    }
}

void Connector::connect()
{
    tiny_assert( state_ == kDisConnect  );

    if( !connect_ ) return;

    int fd = SocketHelper::createNonBlockingSocket();

    int ret = socketHelper_.connect( fd, &localAddr_ );

    int err = ret == 0 ? 0 : errno;

    switch( err ){

        case 0:
        case EINPROGRESS:
        case EINTR:
        case EISCONN:
            onWaitConnectFinish( fd );
            break;
        case EAGAIN:
        case EADDRINUSE:
        case EADDRNOTAVAIL:
        case ECONNREFUSED:
        case ENETUNREACH:
            retry( fd );
            break;
        case EACCES:
        case EPERM:
        case EAFNOSUPPORT:
        case EALREADY:
        case EBADF:
        case EFAULT:
        case ENOTSOCK:
            socketHelper_.close( fd );
            LOG_ERROR(" connect error, errno:%d ", err );
            break;
        default:
            socketHelper_.close( fd );
            LOG_ERROR(" connect unknow error, errno:%d ", err );
            break;
    }

}

void Connector::retry( int fd )
{
    socketHelper_.close( fd );
    if( retryCnt_ <= kMaxReTryCnt ){
        setState( kDisConnect );
        if( connect_ ){
            retryCnt_++;
            this->connect();
        }
    }
}

void Connector::onWaitConnectFinish( int fd )
{
    tiny_assert( NULL != ev_ );
    tiny_assert( state_ == kDisConnect );
    setState( kConnecting );
    ev_ = new EventIo( Connector::onEvents,fd, EV_WRITE );
    ev_->setUserData( this );
    ev_->start( loop_ );
}

void  Connector::onEvents( EventLoop* loop, IEvent* ev, int revents )
{
    Connector* conn = (Connector*)ev->getUserData();

    if( revents&EV_WRITE ){
        conn->onWrite();
    }else if ( revents&EV_ERROR ){
        conn->onError();
    }else {
        LOG_ERROR(" connect receieve unknow event :%d", revents  );
    }
}

void Connector::onWrite()
{
    if( state_ == kConnecting ){
        int fd = ev_->getFd();
        delEvent();

        int err = socketHelper_.getSocketError( fd );

        if( err ){
            LOG_WARN("Connector onWrite get socket error :%d ", err );
            retry( fd );
        } else if( socketHelper_.isSelfConnect( fd ) ){
            LOG_WARN("Connector onWrite is self socket  :%d ", err );
            retry( fd );
        }else {
            setState( kConnected );
            if( connect_ ){

                struct sockaddr_in addr = socketHelper_.getPeerAddr( fd );
                newConnCb_( fd,addr );

            }else {
                socketHelper_.close( fd );
            }
        }
    }
}

void Connector::onError()
{
    if( state_ == kConnecting )
    {
        int fd  = ev_->getFd();
        delEvent();

        int err = socketHelper_.getSocketError( fd );

        LOG_WARN("Connector on error :%d ", err );
        retry( fd );
    }
}







