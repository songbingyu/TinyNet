//add by bingyu fot TinyNet pingpong
//
//

#include <vector>
#include <string>
#include "Event.h"
#include "EventLoop.h"
#include "TcpClient.h"
#include "Log.h"


using namespace std::placeholders;

class Client;

void onTimer( EventLoop* loop, IEvent* ev, int revents );

class Session
{

public:

    Session( EventLoop* loop, const char* ip, int port, Client* owner ):
                                        client_(loop,ip,port),owner_(owner),
                                        bytesRead_(0),bytesWrite_(0),readCnt_(0)
    {

        client_.setConnCallBack( std::bind(&Session::onConnect, this, _1) );
        client_.setCloseCallBack( std::bind(&Session::onClose, this, _1) );
        client_.setReadCallBack( std::bind(&Session::onRead, this, _1) );
        client_.init();
    }
public:
    void start()  { client_.connect(); }
    void stop()   { client_.disconnect(); }
public:
    void onConnect( Connection* conn );
    void  onClose( Connection* conn );

    void onRead( Connection* conn )
    {
        Buffer* buffer = conn->getReadBuffer();
        ++readCnt_;
        bytesRead_ += buffer->capacity();
        bytesWrite_ += buffer->capacity();
        conn->send( buffer->peek(), buffer->capacity()  );
        buffer->retrieveAll();
        //LOG_INFO(" on read sendLen :%d ", sendLen );
    }
private:

    TcpClient  client_;
    Client*     owner_;
public:
    int64_t     bytesRead_;
    int64_t     bytesWrite_;
    int64_t     readCnt_;

};

class Client
{
public:
    Client( EventLoop* loop, const char* ip, int port,
                        int blockSize, int sessionCnt,int timeout ):loop_(loop),
                        sessionCnt_(sessionCnt), timeOut_( timeout ),
                        timer_(loop,onTimer,timeOut_, 0 )
    {
        for( int i=0; i < blockSize; ++i ){
            msg_.push_back( static_cast<char>( i %128) );
        }

        sessionVec_.reserve( sessionCnt_ );
        for( int i=0; i < sessionCnt_; ++i ){
            Session* session = new Session( loop, ip, port, this );

            session->start();

            sessionVec_.push_back( session );

        }

        numConnected_ = 0;

        timer_.setUserData( (void*)this );
        timer_.start();
    }

    ~Client()
    {
        int size = sessionVec_.size();
        for( int i=0; i < size; ++i ){
            delete sessionVec_[i];
        }
        timer_.stop();
    }
public:
    void onTimerOut()
    {
        int size = sessionVec_.size();
        for( int i=0; i < size; ++i ){
            sessionVec_[i]->stop();
        }
    }

    void onConn( Connection* conn )
    {
        LOG_INFO(" on add conn " );
        numConnected_++;
    }

    void onClose( Connection* conn )
    {
        LOG_INFO(" on  conn close  " );
        numConnected_--;
        if( numConnected_ == 0 ){
            int64_t  totalBytesRead = 0;
            int64_t  totalReadCnt = 0;
            int size = sessionVec_.size();
            for( int i=0; i < size; ++i ){
                totalBytesRead += sessionVec_[i]->bytesRead_;
                totalReadCnt  += sessionVec_[i]->readCnt_;
            }

            printf("%ld          totalBytesRead\n", totalBytesRead );
            printf("%ld          totalReadCnt\n", totalReadCnt );
            printf("%ld          average size\n", totalBytesRead/totalReadCnt );
            printf("%ld        Mib/s  throughput\n", totalBytesRead/( timeOut_*1024*1024) );
        }
    }

    std::string* getMsg() { return &msg_; }

private:
    EventLoop* loop_;
    int     sessionCnt_;
    int     timeOut_;
    std::vector<Session*> sessionVec_;
    std::string  msg_;
    int32_t numConnected_;
    EventTimer  timer_;
};

void Session::onConnect( Connection* conn )
{
    conn->setTcpNoDelay();
    conn->send( const_cast<char*>(owner_->getMsg()->c_str()), owner_->getMsg()->size() );
    owner_->onConn( conn );
}

void  Session::onClose( Connection* conn )
{
    owner_->onClose( conn );
}

void onTimer( EventLoop* loop, IEvent* ev, int revents )
{
    Client* client = (Client*)ev->getUserData();
    client->onTimerOut();
}


int main( int argc, char* argv[] )
{
    if( argc != 7 ){
        fprintf( stderr, "Usage: client ip port threads blocksize session time ");
        return 0;
    }

    const char* ip = argv[1];
    int port = atoi( argv[2]);
    //int threadCnt = atoi(argv[3]);

    int blockSize = atoi( argv[4]);
    int sessionCnt = atoi(argv[5]);

    int timeout = atoi(argv[6]);

    EventLoop* loop = new EventLoop();

    Client client( loop, ip, port, blockSize, sessionCnt, timeout );

    loop->run( EVRUN_ALWAYES|EVRUN_NOWAIT );

    return 1;
}


