#if !defined TYPES_H
typedef unsigned long u32;
typedef unsigned int u16;
typedef unsigned char u8;
typedef long s32;
typedef int s16;
typedef char s8;

typedef struct{
	u32 msb;
	u32 lsb;
}u64;

#define TYPES_H
#endif
