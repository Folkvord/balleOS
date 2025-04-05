#ifndef TYPES
#define TYPES

#define LOWER_16(hex) (u16)(hex & 0xffff)
#define UPPER_16(hex) (u16)((hex >> 16) & 0xffff)

typedef unsigned char  u8;    // 1 Unsigned byte
typedef char           s8;    // 1 Byte
typedef unsigned short u16;   // 2 Unsigned bytes
typedef short          s16;   // 2 Bytes
typedef unsigned int   u32;   // 4 Unsigned bytes
typedef int            s32;   // 4 Bytes

#endif