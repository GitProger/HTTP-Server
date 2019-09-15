#ifndef __WIDE_H_INC
#define __WIDE_H_INC
typedef unsigned char byte, *byteptr, uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
#include <stdbool.h>
#include <iso646.h>
#ifndef inline
#define inline __inline
#endif
#ifndef asm
#define asm __asm__ __volatile__
#endif
#endif

