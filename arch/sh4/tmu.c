/*
 * Code to handle the TMU in the Hitachi SH4 CPUs
 * This is hardcoded for a 200MHz core, as the SH4 used in the dreamcast
 * The timer is set to generate an interrupt at a 100Hz frequency
 */
#include "yos.h"

#include "arch/sh4/tmu.h"

void arch_timer_start(uint8 timer) {
  uint8 old_val = *(uint8 *)TMU_TSTR;

  *(uint8 *)TMU_TSTR = old_val | (1);
}

void arch_timer_stop(uint8 timer) {
  uint8 old_val = *(uint8 *)TMU_TSTR;

  *(uint8 *)TMU_TSTR = old_val & ~(1);
}

void arch_timer_init(void) {
  uint8  *TOCR  = (uint8  *)TMU_TOCR;
  uint8  *TSTR  = (uint8  *)TMU_TSTR;
  uint32 *TCOR0 = (uint32 *)TMU_TCOR0;
  uint32 *TCNT0 = (uint32 *)TMU_TCNT0;
  uint16 *TCR0  = (uint16 *)TMU_TCR0;

  volatile uint16 *aica_l = (volatile unsigned short *)0xA0710004;
  uint16 aica_tmp;

  /*
   * Okay, for some wierd reason, the TMU won't initialize properly before
   * the RTC has ticked at least one second..
   */
  aica_tmp = *aica_l;
  while( aica_tmp == *aica_l ) {}

  *TSTR  = 0;
  *TOCR  = 0;
  *TCR0  = 0x0020;
  //*TCR0 = 0x00; // No interrupts for now

  *TCOR0 = 125000;
  *TCNT0 = 125000;

  *TSTR  = 0;
}
