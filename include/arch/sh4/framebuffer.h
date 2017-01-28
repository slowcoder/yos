#ifndef _SH4_FRAMEBUFFER_H_
#define _SH4_FRAMEBUFFER_H_

#include "yos.h"

// These are only prototypes for the assembly-functions
void   framebuffer_lowlevel_init(uint32 cabletype,uint32 pixelmode);
uint32 framebuffer_check_cable(void);
void   framebuffer_clrscr(uint32 color);

#endif
