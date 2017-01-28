#include "yos.h"

#include "arch/x86/io.h"
#include "arch/x86/timer.h"

#define HZ 100

/* Mode for timer 0.  */
static unsigned char timer0mode = PIT_COUNTER_C0 | PIT_LATCH_BOTH
                                | PIT_MODE_SQUARE_WAVE | PIT_MODE_BINARY;
static unsigned short timer0latch = PIT_CLKNUM / HZ;

void arch_timer_init(void) {
  unsigned char latch;

  /* Send the mode for timer 0 to the mode control register.  */
  out8(PIT_ADDR_MODE, timer0mode);
             
  /* Write counter value (latch) to timer0 (first LSB, then MSB.)  */
  latch = timer0latch & 0xff;
  out8(PIT_ADDR_C0, latch);
  latch = (timer0latch >> 8) & 0xff;
  out8(PIT_ADDR_C0, latch);
}

void arch_timer_start(uint8 num) {
}
void arch_timer_stop(uint8 num) {
}
