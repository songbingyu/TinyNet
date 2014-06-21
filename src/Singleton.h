/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef     _SINGLETON_H_
#define     _SINGLETON_H_


// Fixme : not thread safe ...
template< typename T >
class Singleton
{
public:
     static     T*  GetInstance()
    {
        if( NULL == instance_ )
        {
             instance_ = new T();
        }

        return instance_;
    }

private:
    Singleton()     { };
    ~Singleton()    { }

private:
   static  T*   instance_;

}

template<typename  T>
T *Singleton<T>::instance_ = NULL;

#endif      //_SINGLETON_H_









