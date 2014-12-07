/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef     _SINGLETON_H_
#define     _SINGLETON_H_

#include <pthread.h>
#include "nocopyable.h"

#ifndef  _MULTI_THREAD_
// Fixme : not thread safe ...
template<typename T>
class Singleton : public nocopyable
{
public:
    static T* getInstance() {
        if (NULL == instance_) {
             instance_ = new T();
        }

        return instance_;
    }

private:
    Singleton()     { };
    ~Singleton()    { }

private:
   static  T*   instance_;

};

template<typename  T>
T *Singleton<T>::instance_ = NULL;
#else

template<typename T>
classs Singleton : public nocopyable {
public:
    static T& getInstance() {
        pthread_once(&ponce_, &Singleton::init);
        return value_;
    }

private:
    Singleton();
    ~Singleton();

    static void init() {
        value_ = new T();
    }

private:
    static pthread_once_t ponce_;
    static T* value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ =  NULL:

#endif  //_MULTI_THREAD_

#endif      //_SINGLETON_H_












