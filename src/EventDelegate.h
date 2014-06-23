/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _EVENTDELEGATE_H_
#define _EVENTDELEGATE_H_


struct sockaddr_in;

template < typename  C, typename A1, typename A2  >
class  EventDelegate
{
public:
    typedef   void  ( C::* Func  )( A1*  arg1, A2* arg2 );

    EventDelegate( C* inst, Func func ): inst_( inst ), func_( func )
    {

    }

    void callback( A1* arg1, A2* arg2 = NULL   )
    {

        ( inst_->*func_ )( arg1, arg2 );

    }

    /*void callback( T* arg, struct sockaddr_in & addr )
    {
        (inst_->*func_ )( arg, addr );
    }*/

private:
    C*      inst_;
    Func    func_;
};



#endif// _EVENTDELEGATE_H_











