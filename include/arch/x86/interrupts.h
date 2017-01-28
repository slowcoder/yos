#ifndef _X86_INTERRUPTS_H_
#define _X86_INTERRUPTS_H_

#include "yos.h"

struct idt_descriptor {
  uint32 offset_low  : 16,
         selector    : 16,
         reserved    : 5,
         zero        : 3,
         type        : 5,
         dpl         : 2,
         present     : 1,
         offset_high : 16;
};

#endif
