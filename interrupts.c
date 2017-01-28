#include "yos.h"
#include "arch.h"

volatile uint64 jiffies = 0;

struct irq_handler {
  uint8 (*handler)(uint8 interrupt);
};
struct irq_handler irq_handler[16];

void interrupt_handler(uint8 irq) {
  uint8 reschedule;

  reschedule = 0;

  if( irq_handler[irq].handler != NULL )
    reschedule = irq_handler[irq].handler(irq);
  else {
    kprintf("Received unhandled IRQ%u\n",irq);
  }

  arch_irq_ack(irq);

  if(reschedule) scheduler();
}

static uint8 interrupt_timer_handler(uint8 irq) {
  jiffies++;
  return(1);
}

void interrupt_init(void) {
  uint8 i;

  for(i=0;i<=0xF;i++)
    irq_handler[i].handler = NULL;
  
  irq_handler[0x00].handler = &interrupt_timer_handler;

  arch_interrupt_init();
}
