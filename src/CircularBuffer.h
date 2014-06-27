/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include<errno.h>
#include<assert.h>
#include"Log.h"

// Fixme: consider other way?
//
template < typename  T, size_t N   >
class  CircularBuffer
{
public:
    CircularBuffer( ): begin_(0), end_( begin_ ),size_( 0 ), data_( NULL )
    {
#ifdef  _DEBUG_
        assert( N > 0 );
#endif
        data_  = new T[N];
        size_   = N;
    }

   ~CircularBuffer()
    {
        delete data_;
        data_  = NULL;
        begin_ = 0;
        end_   = 0;
        size_  = 0;
    }

public:
   int      size()      {  return   size_; }
   int      capacity()  {  return   end_ >= begin_ ?  end_ - begin_ : end_ + size_ - begin_;  }
   int      freeSize()  {  return   size_ - capacity();    }

   bool     push( T*  data, int  count, bool isOver = false  )
   {
        if( ( end_ + count )% size_ >= begin_ )
        {
                LOG_ERROR(" circual buffer is over ");
                return false;
        }

        if( end_ >= begin_ )
        {
            if( count > ( size_ - end_ ) )
            {
                memcpy( data_+ end_, data, ( size_ -  end_ )*sizeof(T) );
                memcpy( data_, data, ( count - size_ +  end_ )*sizeof(T) );
            }
            else
            {
                memcpy( data_ + end_, data, count*sizeof(T) );
            }

        }
        else
        {
            memcpy( data_ + end_, data_, count*sizeof(T));
        }

        end_ = ( end_ + count )% size_;
        return true;
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

    bool    pop( char* buf, int cnt )
    {

            if( capacity() < cnt  )  return false;

            if( end_ <=  begin_  )
            {
                if( cnt > (size_ - begin_) )
                {
                    memcpy( buf, data_ + begin_, ( size_ - begin_ )*sizeof(T) );
                    memcpy( buf, data_, ( cnt - (size_ - begin_ )*sizeof(T)));
                }
                else
                {
                    memcpy( buf, data_ + begin_,  cnt*sizeof(T));
                }
            }
            else
            {
                memcpy(buf, data_+ begin_, cnt*sizeof(T));
            }

            begin_ = ( begin_ + cnt)% size_;
            return true;
    }

private:

    size_t   begin_;
    size_t   end_;
    size_t   size_;
    T*       data_;
};




