/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include<errno.h>
#include<assert.h>
#include"Log.h"

// Fixme: consider other way?
//
template < size_t N >
class  CircularBuffer
{
public:
    CircularBuffer( ): begin_(0), end_( begin_ ), count_(0), size_( 0 ), data_( NULL )
    {
#ifdef  _DEBUG_
        assert( N > 0 );
#endif
        data_  = new char[N];
        size_   = N;
    }

    ~CircularBuffer()
    {
        delete data_;
        data_  = NULL;
        begin_ = 0;
        end_   = 0;
        count_ = 0;
        size_  = 0;
    }

public:
    int      size()      {  return   size_; }
    int      capacity()  {  return   count_; }
    int      freeSize()  {  return   size_ - count_;    }
    void retrieveAll()
    {
        end_ = begin_ = 0;
    }

    void retrieve( int len )
    {
        tiny_assert( len <= capacity() );
        if(capacity() < len ){
            end_ = ( end_ + len )%size_;
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

    void retrieveInt8()
    {
        retrieve(sizeof(int8));
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
            if( len_ > ( size_ - end_ ) ){
                memcpy( data_+end_, data, size_-end_ );
                memcpy( data_, data_+size_-end_, len-(size_-end_) );
            }else{
                memcpy( data_+end_, data, len );
            }
        }else{
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
            if( cnt > (size_ - begin_) )
            {
                memcpy( buf, data_ + begin_, ( size_ - begin_ ) );
                memcpy( buf, data_, ( cnt - (size_ - begin_ ) ) );
            }
            else
            {
                memcpy( buf, data_ + begin_, cnt );
            }
        }else{
            memcpy(buf, data_+ begin_, cnt);
        }

        begin_ = ( begin_ + cnt)% size_;
        count_ -= cnt;
        return ;
    }



    int     push( SocketHelper* socketHelper, int fd )
    {
            // should read 2 ?
            int len =  begin_ <=  end_ ? size_ - end_ : begin_ - end_ ;
            int ret =  socketHelper->read( fd ,  data_ + end_ ,  len  );
            if( ret == 0 )
            {
                return -1;
            }
            else  if( ret < 0 )
            {
                if(  errno != EWOULDBLOCK || errno != EAGAIN )  return -2;
            }
            else
            {
                end_ = (end_ +  ret )% size_;
            }

            return 0;
    }

private:
    size_t   begin_;
    size_t   end_;
    size_t   count_;
    size_t   size_;
    char*    data_;
};




