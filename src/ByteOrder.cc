/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#include <endian.h>
#include <stdint.h>
#include "TinyDefine.h"


namespace ByteOrder {
    tiny_forceinline  uint64_t hostToNet64(uint64_t host64)
    {
        return htobe64(host64);
    }

    tiny_forceinline  uint32_t hostToNet32(uint64_t host32)
    {
        return htobe32(host32);
    }

    tiny_forceinline uint16_t hostToNet16(uint16_t host16)
    {
        return htobe16(host16);
    }

    tiny_forceinline uint64_t netToHost64(uint64_t net64)
    {
        return be64toh(net64);
    }

    tiny_forceinline uint32_t netToHost32(uint32_t net32)
    {
        return be32toh(net32);
    }

    tiny_forceinline uint16_t netToHost16(uint16_t net16)
    {
        return be16toh(net16);
    }


}// end nampspace Byteorder


