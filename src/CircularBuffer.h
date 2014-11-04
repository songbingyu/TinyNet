/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <errno.h>
#include <assert.h>
#include "ByteOrder.h"
#include "Log.h"
#include "nocopyable.h"

//is not a best way?
template < size_t N >
class  CircularBuffer:public nocopyable
{
public:
    CircularBuffer(): begin_(0), end_( begin_ ), count_(0), size_( 0 ), data_( NULL )
    {
#ifdef  _DEBUG_
        tiny_assert( N > 0 );
#endif
        data_  = new char[N];
        size_   = N;
    }

    ~CircularBuffer()
    {
        delete []data_;
        data_ = NULL;
        begin_ = 0;
        end_   = 0;
        count_ = 0;
        size_  = 0;
    }

public:
    int      size()   const    {  return   size_; }
    int      capacity()  const {  return   count_; }
    int      freeSize()  const {  return   size_ - count_;    }

    void retrieveAll()
    {
        end_ = begin_ = count_ = 0;
    }

    void retrieve( int len )
    {
        tiny_assert( len <= capacity() );
        if( len < capacity() ){
            end_ = ( end_ + len )%size_;
            count_ += len;
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
            //Fixme:should clear?
            LOG_ERROR("buffer is over ....");
            return;
        }

        if( end_ > begin_ ){
            if( len > (int)( size_ - end_ ) ){
                memcpy( data_+end_, data, size_-end_ );
                memcpy( data_, data_+size_-end_, len-(size_-end_) );
            }else{
                memcpy( data_+end_, data, len );
            }
        } else {
            memcpy( data_+end_, data, len );
        }

        end_ = ( end_ + len )%size_;
        count_ += len;
    }

    bool  peek( char* buf, int cnt )
    {
        tiny_assert( NULL!= buf && cnt >=0 );

        if( capacity() < cnt  )  return false;

        if( end_ <  begin_  )
        {
            if( cnt > (int)(size_ - begin_) )
            {
                memcpy( buf, data_ + begin_, ( size_ - begin_ ) );
                memcpy( buf + size_ - begin_, data_, ( cnt - (size_ - begin_ ) ) );
            }
            else
            {
                memcpy( buf, data_ + begin_, cnt );
            }
        }else{
            memcpy(buf, data_+ begin_, cnt);
        }

        // begin_ = ( begin_ + cnt)% size_;
        //count_ -= cnt;
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

        int len =  begin_ <=  end_ ? size_ - end_ : begin_ - end_ ;
        int ret =  socketHelper_->read( fd ,  data_ + end_ ,  len  );
        if( ret == 0 )
        {
            return -1;
        } else  if( ret < 0 ) {

            if(  errno != EWOULDBLOCK || errno != EAGAIN )  return -2;
        } else {
            end_ = (end_ +  ret )% size_;
            count_ += len;
        }
        return len;
    }

    int32_t flushFd( SocketHelper* socketHelper_, int fd )
    {
        tiny_assert( capacity() > 0  );
#ifdef _DEBUG_
        char* sendBuf = new char[ capacity()];
        peek( sendBuf, capacity() );

        int n = socketHelper_->write( fd, sendBuf, capacity() );
        if( n > 0 ){
            retrieve( n );
        } else {
            //Fixme ? what should do ?
        }

        delete []sendBuf;
#else
        int n = 0;
        if( begin_ <=  end_ ){
            n = socketHelper_->write( fd, data_ + begin_, capacity() );
        } else {
            n = socketHelper_->write(  fd, data_ + begin_, size_ - begin_ );
            if( n > 0 ){
                if( n == (int)(size_ - begin_) ){
                    //try continue write
                    n += socketHelper_->write( fd, data_, ( count_ -(size_ - begin_) ) );
                }
            }
        }
        retrieve( n );
#endif
        return n;
    }

private:
    size_t   begin_;
    size_t   end_;
    size_t   count_;
    size_t   size_;
    char*    data_;
};

#endif // _CIRCULAR_BUFFER_H_



