#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "yos.h"

void kputs(char *str);
void kprintf(char *fmt,...);
void kprintfxy(unsigned short x,unsigned short y, const char *fmt, ...);

#endif

