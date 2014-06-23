/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _EVENTDELEGATE_H_
#define _EVENTDELEGATE_H_

template < typename  C, typename T  >
class  EventDelegate
{
public:
    typedef   void  ( C::* Func  )( T*  arg );

    EventDelegate( T* inst, Func fun ): inst_( inst ), func_( func )
    {

    }

    void operator()( T* arg )
    {
        ( inst->*func )( arg );
    }
private:
    T*      inst_;
    Func    func_;
}



#endif// _EVENTDELEGATE_H_











