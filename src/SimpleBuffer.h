/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _SIMPLE_BUFFER_H_
#define _SIMPLE_BUFFER_H_

#include <errno.h>
#include <vector>
#include "ByteOrder.h"
#include "Log.h"


class  SimpleBuffer
{
public:
    static const kInitSize=1024*4;
public:
    SimpleBuffer(): buffer_(1024*4),begin_(0), end_( begin_ )
    {

    }

    ~SimpleBuffer()
    {
        TINY_DELETE( data_ );
        begin_ = 0;
        end_   = 0;
        count_ = 0;
        size_  = 0;
    }

public:
    int      size()      {  return   buffer.size(); }
    int      capacity()  {  return   end_ - begin_ ; }
    int      freeSize()  {  return   buffer_.size() - end_ ;    }

    void retrieveAll()
    {
        end_ = begin_ = 0;
    }

    void retrieve( int len )
    {
        tiny_assert( len <= capacity() );
        if( len < capacity() ){
            begin_ += len;
        }else{
            retrieveAll();
        }
    }

    void retrieveInt64()
    {
        retrieve(sizeof(int64_t));
    }

    void retrieveInt32()
    {
        retrieve(sizeof(int32_t));
    }

    void retrieveInt16()
    {
        retrieve(sizeof(int16_t));
    }

    void retrieveInt8()
    {
        retrieve(sizeof(int8_t));
    }

    void append( char* data, int len )
    {
        tiny_assert( len !=0 && NULL != data );
        if( freeSize() < len ){
            makeSpace( len );
        }

        std::copy( data, data+len,begin() + end_ );

        hasWritten(len);

    }

    bool hasWritten( size_t len )
    {
        tiny_assert( len < freeSize() );
        end_ += len;
    }

    char* peek() { return begin() + begin_; }

    bool  peek( char* buf, int cnt )
    {

        tiny_assert( cnt <= capacity() );
        memcpy( buf, peek(), cnt );
        return true;
    }

    bool peekInt64() const
    {
        tiny_asssert( capacity() >= sizeof(int64_t) );
        int64_t be64 = 0;
        peek( (char*)&be64, sizeof(int64_t) );
        return ByteOrder::netToHost64(be64);
    }

    bool peekInt32() const
    {
        tiny_assert( capacity() >= sizeof(int32_t) );
        int32_t be32 = 0;
        peek( (char*)&be32, sizeof(int32_t) );
        return ByteOrder::netToHost32(be32);
    }

    bool peekInt16() const
    {
        tiny_assert( capacity() >= sizeof(int16_t) );
        int16_t be16;
        peek( (char*)&be16,sizeof(int16_t) );
        return ByteOrder::netToHost16(be16);
    }

    bool peekInt8() const
    {
        tiny_assert( capacity() >= sizeof(int8_t) );
        int8_t be8;
        peek( (char*)&be8, sizeof(int8_t) );
        return be8;
    }

    void read( char* data, int len )
    {
        peek( data, len );
        retrieve( len );
        return;
    }

    int64_t readInt64()
    {
        int64_t ret = peekInt64();
        retrieveInt64();
        return ret;
    }

    int32_t readInt32()
    {
        int32_t ret = peekInt32();
        retrieveInt32();
        return ret;
    }

    int16_t readInt16()
    {
        int16_t ret = peekInt16();
        retrieveInt16();
        return ret;
    }

    int8_t readInt8()
    {
        int8_t ret = peekInt8();
        retrieveInt8();
        return ret;
    }

    int32_t readFd( SocketHelper* socketHelper_, int fd )
    {
        // should read 2 ?
        tiny_assert( freeSize() > 0 );

        char extrbuf[65535]={0};

        srtuct iovec vec[2];

        const size_t writable = freeSize();
        vec[0].iov_base = begin() + end_;
        vec[0].iov_len  = writable;

        vec[0].iov_base = extrbuf;
        vec[0].iov_len  = sizeof(extrbuf);

        const int iovcnt  =  writable < sizeof(extrbuf) ? 2 : 1;
        int ret =  socketHelper_->readv( fd, vec, iovcnt );

        if( ret < 0 ) {
            if(  errno != EWOULDBLOCK || errno != EAGAIN )  return -2;
        } else if( (size_t)n <= writable ) {
            end_ += n;
        }else{
            end_ = buffer.size();
            append( extrbuf, n - writable );
        }

        return ret;
    }

    int32_t flushFd( SocketHelper* socketHelper_, int fd )
    {
        tiny_assert( capacity() > 0  );
        int n = socketHelper_->write( fd, begin() + begin_, capacity() );
        if( n > 0 ){
            retrieve( n );
        } else {
            //Fixme ? what should do ?
        }
        return n;
    }
private:
    char* begin() { return &*buffer_.begin() };

    void makeSpace( int len )
    {
        if( freeSize() < len ) ){
            buffer_.resize( end_ + len );
        } else {
            size_t readble = capacity();
            std::copy( begin()+begin_, begin()+end_, begin());
            begin_ =0;
            end_ = begin_ + readble;
            tiny_assert( readble == capacity() );
        }
    }
private:
    std::vector<char> buffer_;
    size_t   begin_;
    size_t   end_;
};

#endif // _SIMPLE_BUFFER_H_



