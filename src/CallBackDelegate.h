/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _CALLBACKDELEGATE_H_
#define _CALLBACKDELEGATE_H_


struct sockaddr_in;

template < typename  C, typename A1, typename A2  >
class  CallBackDelegate
{
public:
    typedef   void  ( C::*Func  )( A1*  arg1, A2* arg2 );

    CallBackDelegate( C*  inst, Func func ): inst_( inst ), func_( func )
    {

    }

    void callback( A1* arg1, A2* arg2 = NULL   )
    {

        ( inst_->*func_ )( arg1, arg2 );

    }


private:
    C* const      inst_;
    Func    func_;
};



#endif// _CALLBACKDELEGATE_H_











