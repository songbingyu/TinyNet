/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include<assert.h>
#include"Log.h"


template < typename  T >
class  CircularBuffer
{
public:
    CircularBuffer( int N ): begin_(0), end_( begin_ ),size_( 0 ), data_( NULL )
    {
#ifdef  _DEBUG_
        assert( N > 0 );
#endif
        data_  = new T[N];
        size   = N;
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
   int      size() { retuen size_; }
   int      capacity( ) {  return end_ > begin_ ?  end_ - begin_ : end_ + size_ - begin_;  }
   int      freeSize()  {  return  size_ - capacity();    }
   bool     push( T& v, bool isOver   )
   {
        if( ( end_ + 1 )% size_ == begin_ )
        {
            if( !isOver )
            {
                LOG_ERROR(" circual buffer is over ");
                retuen false;
            }
            else
            {
              data_[ end_ ] = v;
              begin_ = ( begin_+ 1 )% size_;
              end_   = ( end_  + 1 )% size_;

              retuen true;
            }
        }

        data_[ end_ ] = v;
        end_ = ( end_ +1 )% size_;
        retuen true;
   }

    bool   pop( T& v )
    {
        if( capacity() == 0 )  return false;

        v  =  data_[ begin_ ];
        begin_ = ( begin_+ 1 )% size_;

    }
private:

    int  begin_;
    int  end_;
    int  size_;
    T*   data_;
};




