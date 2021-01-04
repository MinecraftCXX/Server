#ifndef GAMESERVER_ENDIAN_HPP
#define GAMESERVER_ENDIAN_HPP

#ifdef _MSC_VER

#include <cstdlib>
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

#include <libkern/OSByteOrder.h>
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bswap_32(x) BSWAP_32(x)
#define bswap_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bswap_32(x) swap32(x)
#define bswap_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)
#endif

#else
#error "no byteswap function found"
#endif

#ifdef BIG_ENDIAN
#define hostToNetworkInt(x) (x)
#define hostToNetworkLong(x) (x)
#define networkToHostInt(x) (x)
#define networkToHostLong(x) (x)
#else
#define hostToNetworkInt(x) (bswap_32(((uint32_t) x)))
#define hostToNetworkLong(x) (bswap_64(((uint64_t) x)))
#define networkToHostInt(x) hostToNetworkInt(x)
#define networkToHostLong(x) hostToNetworkLong(x)
#endif

#endif //GAMESERVER_ENDIAN_HPP
