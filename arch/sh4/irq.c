#include "yos.h"

unsigned int get_sr();
void set_sr(unsigned int sr);
asm(
".globl _get_sr, _set_sr\n"
"_get_sr:\n"
"        stc     sr,r0\n"
"        rts\n"
"        nop\n"

"_set_sr:\n"
"        ldc     r4,sr\n"
"        rts\n"
"        nop\n"
);


void arch_irq_init(void) {
  // Enables exceptions
  sr = get_sr();
  sr &= 0xefffffff;
  set_sr(sr);

}

void arch_irq_unmask(uint8 num) {
  // Lowers the interrupt-masklevel to 0
  sr = get_sr();
  sr = sr & (0xFFFFFF0F);
  set_sr(sr);
}

void arch_irq_mask(uint8 num) {
  ASSERT(0);
}
