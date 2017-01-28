#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include "yos.h"

void interrupt_init(void);
void interrupt_handler(uint8 irq);

#endif
