#ifndef _X86_CPU_H_
#define _X86_CPU_H_

#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
#define USER_CS   0x1B
#define USER_DS   0x23

struct segment_pointer { // Pointer to IDT, LDT or GDT  (Sent to l*dt)
  uint16 limit;
  uint32 *base __attribute__ ((packed));
};

#endif
