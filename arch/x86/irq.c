#include "yos.h"

#include "spinlock.h"
#include "arch/x86/io.h"


void arch_irq_init(void) {

  /* 
     This function is
     Copyright 2001-2004, Travis Geiselbrecht.
  */

  // setup the interrupt controller
  out8(0x20, 0x11);       // Start initialization sequence for #1.
  out8(0xa0, 0x11);       // ...and #2.
  out8(0x21, 0x60);       // Set start of interrupts for #1 (0x60).
  out8(0xa1, 0x68);       // Set start of interrupts for #2 (0x68).
  out8(0x21, 0x04);       // Set #1 to be the master.
  out8(0xa1, 0x02);       // Set #2 to be the slave.
  out8(0x21, 0x01);       // Set both to operate in 8086 mode.
  out8(0xa1, 0x01);
  out8(0x21, 0xfb);       // Mask off all interrupts (except slave pic line).
  out8(0xa1, 0xff);       // Mask off interrupts on the slave.

  asm("sti");
}

void arch_irq_unmask(uint8 irq) {
  if(irq > 15) return;

  if (irq < 8)
    out8(0x21, in8(0x21) & ~(1 << irq));
  else
    out8(0xa1, in8(0xa1) & ~(1 << (irq - 8)));
}

void arch_irq_mask(uint8 irq) {
  if(irq > 15) return;

  if (irq < 8)
    out8(0x21, in8(0x21) | (1 << irq));
  else
    out8(0xa1, in8(0xa1) | (1 << (irq - 8)));
}

void arch_irq_ack(uint8 irq) {
  if( (irq < 0x8) )
    out8(0x20,0x20);
  else if( (irq >= 0x8) && (irq <= 0xF) ) {
    out8(0x20,0xa0);
    out8(0x20,0x20);
  }
}
