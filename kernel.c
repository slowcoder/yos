#include "yos.h"
#include "console.h"
#include "pmm.h"
#include "tasks.h"
#include "phyheap.h"
#include "spinlock.h"
#include "interrupts.h"
#include "framebuffer.h"

#include "arch.h"

void kernel_thread(void) {
  uint8 ch = 0;
  for(;;) {
    framebuffer_gotoxy(77,0);
    framebuffer_putchar(ch++);
  }
}
void kernel_thread2(void) {
  uint8 ch = 0;
  for(;;) {
    framebuffer_gotoxy(79,0);
    framebuffer_putchar(ch++);
  }
}

void kernel_entry(void) {
  uint8 *buff;
  uint32 i;

  arch_init();

  phyheap_init();

  i = 65535;
  buff = (uint8 *)phyheap_kmalloc(i);
  for(;i>0;i--) {
    buff[i]=0xFF;
  }
  //phyheap_debug();

  tasks_init();

  thread_create( task_kernel(),(addr_t)&kernel_thread );
  thread_create( task_kernel(),(addr_t)&kernel_thread2 );

  interrupt_init();

  arch_enable_paging(task_kernel());

  kprintf("Tada!\n");
  kprintf("Starting timers\n");
  arch_timer_start(0);
  arch_irq_init();
  arch_irq_unmask(0);

  pmm_debug();

  kprintf("Halting..\n");
  for(;;);
}

