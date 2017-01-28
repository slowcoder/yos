#ifndef _YOS_H_
#define _YOS_H_

#include "console.h"

#define YOS_VERSION "0.2"

typedef unsigned char      uint8;
typedef char                int8;
typedef unsigned short     uint16;
typedef short               int16;
typedef unsigned int       uint32;
typedef int                 int32;
typedef unsigned long long uint64;
typedef long long           int64;

typedef unsigned char    kern_ret;
#define KERN_OK   1
#define KERN_FAIL 0

extern volatile uint64 jiffies;

#ifdef x86
 typedef unsigned long   addr_t;
 typedef unsigned long   size_t;
#endif
#ifdef sh4
 typedef unsigned long   addr_t;
 typedef unsigned long   size_t;
#endif

#ifdef DEBUG
#define DEBUG_MESSAGE(STR) kprintf(#STR);
#else
#define DEBUG_MESSAGE(STR)
#endif

#ifdef DEBUG
#define ASSERT(x) \
  { if(!(x)) { kprintf("ASSERT FAILED (%s:%d): %s\n", __FILE__, __LINE__, #x); \
      for(;;); } \
  }
#else
#define ASSERT(x)
#endif


#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#endif
