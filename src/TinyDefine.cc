/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include"TinyDefine.h"
#include<sys/time.h>
#include<sys/select.h>



Timestamp  tinyGetTime()
{
    struct timeval tv;
    gettimeofday( &tv, 0 );
    return tv.tv_sec + tv.tv_usec*1e-9;
}

void tinySleep( Timestamp t )
{
    struct timeval tv;
    tv.tv_sec = (long)t;
    tv.tv_usec=(long)( ( t - tv.tv_sec ) * 1e6 );
    select( 0, 0, 0, 0, &tv );
}




