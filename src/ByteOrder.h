/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef _BYTE_ORDER_H_
#define _BYTE_ORDER_H_


namespace ByteOrder
{
    tiny_forceinline  uint64_t hostToNet64( uint64_t host64 );
    tiny_forceinline  uint32_t hostToNet32( uint64_t host32 );
    tiny_forceinline uint16_t hostToNet16( uint16_t host16 );
    tiny_forceinline uint64_t netToHost64( uint64_t net64 );
    tiny_forceinline uint32_t netToHost32( uint32_t net32 );
    tiny_forceinline uint16_t netToHost16( uint16_t net16 );

}// namespace  Byteorder


#endif //_BYTE_ORDER_H_

