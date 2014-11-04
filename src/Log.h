/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef     _LOG_H_
#define     _LOG_H_

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// TODO : add write file, color control ,  option ...

class Log
{

public:
    enum LogLevel
    {
        LOG_INFO,
        LOG_DEBUG,
        LOG_WARN,
        LOG_ERROR,
        LOG_SYS,
    };

    enum
    {
        MAX_LOG_LEN = 1024,
    };

public :

    static void WriteLog(const char* logLevel,const  char* file, const int  line,  const char* fmt , ...  )
    {

           /* char  buf[ MAX_LOG_LEN ] = { 0 };
            char  logbuf[ 512 ]      = { 0 };

            struct timeval tv;
            gettimeofday( &tv, NULL );
            struct tm tm;
            localtime_r( (const time_t*)&tv.tv_sec, &tm );
            va_list  ap;

            va_start(ap, fmt);
            (void) vsnprintf( logbuf , 512 , fmt, ap);
            va_end(ap);

            const char*  filename = strrchr( file, '/');
            sprintf(buf,"[%s][%04d-%02d-%02d %02d:%02d:%02d.%06ld ] %s %d : ",logLevel,
                    tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
                    tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec, filename+1, line );
            strcat( buf, logbuf );
            strcat( buf, "\n"   );
            printf( buf );
            flush(stdout);*/
    }

};

#define     LOG_INFO(  fmt, ... )    Log::WriteLog( "INFO",   __FILE__, __LINE__, fmt, ## __VA_ARGS__  )
#define     LOG_DEBUG( fmt, ... )    Log::WriteLog( "DEBUG",  __FILE__, __LINE__, fmt, ## __VA_ARGS__  )
#define     LOG_WARN(  fmt, ... )    Log::WriteLog( "WARN",   __FILE__, __LINE__, fmt, ## __VA_ARGS__  )
#define     LOG_ERROR( fmt, ... )    Log::WriteLog( "ERROR",  __FILE__, __LINE__, fmt, ## __VA_ARGS__  )
#define     LOG_SYS( fmt, ... )      Log::WriteLog( "SYS",    __FILE__, __LINE__, fmt, ## __VA_ARGS__  )



#endif // _LOG_H_



