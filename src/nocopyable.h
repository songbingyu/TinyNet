/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _NOCOPYABLE_H_
#define _NOCOPYABLE_H_


//from boost::nocopyable
class nocopyable
{
protected:
    nocopyable()  { }
    ~nocopyable() { }
private:
    nocopyable(nocopyable&);
    nocopyable& operator=(nocopyable&);
};


#endif // _NOCOPYABLE_H_




