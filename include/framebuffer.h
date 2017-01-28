#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include "yos.h"

/*
 * All framebuffer-devices needs to export these functions
 */
void framebuffer_init(void);
void framebuffer_gotoxy(uint16 x,uint16 y);
void framebuffer_putchar(char ch);

#endif
