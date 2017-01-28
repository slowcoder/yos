/*
 * Interrupt (Exception+IRQ+INT) handling routines for yOS
 *
 * For timer and IRQ code, look at irq.c and timer.c
 */
#include "yos.h"
#include "arch.h"
#include "interrupts.h"

#include "arch/x86/cpu.h"
#include "arch/x86/interrupts.h"

static struct idt_descriptor idt[256];

/*
 * -- Support functions --
 */
static __inline__ void load_idt (struct segment_pointer *sp) {
  __asm__ ("lidt %0" :: "m" (sp->limit));
}

static void idt_fill(struct idt_descriptor *desc,
                     uint32 offset,
                     uint16 selector) {
 
  desc->offset_low  = offset & 0xffff;
  desc->offset_high = offset >> 16;
  desc->zero        = 0;
  desc->type        = 0+2+4+8+0; // 01110b
  desc->dpl         = 0; // Run in kernel-space
  desc->present     = 1; // Sure is present.. Would suck to swap out an IH
  desc->selector    = selector;
}

void trap0();  void trap1();  void trap2();  void trap3();  void trap4();
void trap5();  void trap6();  void trap7();  void trap8();  void trap9();
void trapA();  void trapB();  void trapC();  void trapD();  void trapE();
void trapF();  void trap10(); void trap11(); void trap12(); void trap13();
void trap60(); void trap61(); void trap62(); void trap63();
void trap64(); void trap65(); void trap66(); void trap67();
void trap68(); void trap69(); void trap6A(); void trap6B();
void trap6C(); void trap6D(); void trap6E(); void trap6F();
void trapFE();

#if 0
uint8 *vram = (uint8 *)0xB8000;
static uint8 interrupt_timer_handler(uint8 vector) {
  //jiffies++;
  vram[79*2]++;

  //issue_ipi();

  arch_irq_ack(vector);

  return(1); // Reschedule after this IRQ
}
#endif

extern volatile struct runqueue *running_thread;

uint8 *vram = (uint8 *)0xB8000;
uint32 interrupt_handle_trap(struct iframe frame) {
  uint32 returnto;

  running_thread->thread->iframe = &frame;
  vram[79*2]++;

  //returnto = (uint32)&frame;

  switch(frame.vector) {
  case 0x60:
  case 0x61:
  case 0x62:
  case 0x63:
  case 0x64:
  case 0x65:
  case 0x66:
  case 0x67:
  case 0x68:
  case 0x69:
  case 0x6A:
  case 0x6B:
  case 0x6C:
  case 0x6D:
  case 0x6E:
  case 0x6F:
    interrupt_handler(frame.vector - 0x60);
    break;

  default:
    asm("cli");
    kprintf("Unhandled vector 0x%x\n",frame.vector);
    for(;;);
  }

  returnto = (uint32)running_thread->thread->iframe;
  
  return( returnto );
}

void arch_interrupt_init(void) {
  struct segment_pointer seg_ptr;

  seg_ptr.limit = sizeof(idt) - 1;
  seg_ptr.base  = (void *)idt;
  
  // Exceptions
  idt_fill(&idt[0x00],(uint32)trap0,KERNEL_CS);
  idt_fill(&idt[0x01],(uint32)trap1,KERNEL_CS);
  idt_fill(&idt[0x02],(uint32)trap2,KERNEL_CS);
  idt_fill(&idt[0x03],(uint32)trap3,KERNEL_CS);
  idt_fill(&idt[0x04],(uint32)trap4,KERNEL_CS);
  idt_fill(&idt[0x05],(uint32)trap5,KERNEL_CS);
  idt_fill(&idt[0x06],(uint32)trap6,KERNEL_CS);
  idt_fill(&idt[0x07],(uint32)trap7,KERNEL_CS);
  idt_fill(&idt[0x08],(uint32)trap8,KERNEL_CS);
  idt_fill(&idt[0x09],(uint32)trap9,KERNEL_CS);
  idt_fill(&idt[0x0A],(uint32)trapA,KERNEL_CS);
  idt_fill(&idt[0x0B],(uint32)trapB,KERNEL_CS);
  idt_fill(&idt[0x0C],(uint32)trapC,KERNEL_CS);
  idt_fill(&idt[0x0D],(uint32)trapD,KERNEL_CS);
  idt_fill(&idt[0x0E],(uint32)trapE,KERNEL_CS);
  idt_fill(&idt[0x0F],(uint32)trapF,KERNEL_CS);
  idt_fill(&idt[0x10],(uint32)trap10,KERNEL_CS);
  idt_fill(&idt[0x11],(uint32)trap11,KERNEL_CS);
  idt_fill(&idt[0x12],(uint32)trap12,KERNEL_CS);
  idt_fill(&idt[0x13],(uint32)trap13,KERNEL_CS);

  // External ISRs
  idt_fill(&idt[0x60],(uint32)trap60,KERNEL_CS);
  idt_fill(&idt[0x61],(uint32)trap61,KERNEL_CS);
  idt_fill(&idt[0x62],(uint32)trap62,KERNEL_CS);
  idt_fill(&idt[0x63],(uint32)trap63,KERNEL_CS);
  idt_fill(&idt[0x64],(uint32)trap64,KERNEL_CS);
  idt_fill(&idt[0x65],(uint32)trap65,KERNEL_CS);
  idt_fill(&idt[0x66],(uint32)trap66,KERNEL_CS);
  idt_fill(&idt[0x67],(uint32)trap67,KERNEL_CS);
  idt_fill(&idt[0x68],(uint32)trap68,KERNEL_CS);
  idt_fill(&idt[0x69],(uint32)trap69,KERNEL_CS);
  idt_fill(&idt[0x6A],(uint32)trap6A,KERNEL_CS);
  idt_fill(&idt[0x6B],(uint32)trap6B,KERNEL_CS);
  idt_fill(&idt[0x6C],(uint32)trap6C,KERNEL_CS);
  idt_fill(&idt[0x6D],(uint32)trap6D,KERNEL_CS);
  idt_fill(&idt[0x6E],(uint32)trap6E,KERNEL_CS);
  idt_fill(&idt[0x6F],(uint32)trap6F,KERNEL_CS);

  // IPI
  idt_fill(&idt[0xFE],(uint32)trapFE,KERNEL_CS);

  load_idt(&seg_ptr);
}
