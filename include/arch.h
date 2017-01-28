#ifndef _ARCH_H_
#define _ARCH_H_

#include "tasks.h"

void  arch_init(void);

int   arch_tns(volatile int *val, int set_to, int test_val);  // Test-and-set

void  arch_task_create(struct task *task);
void  arch_thread_create(struct thread *thread,addr_t entry);

uint8 arch_numcpus(void);
void  arch_sleep(void);

void  arch_timer_init(void);
void  arch_timer_start(uint8 timer);
void  arch_timer_stop(uint8 timer);

void  arch_irq_init(void);
void  arch_irq_unmask(uint8 irq);
void  arch_irq_mask(uint8 irq);
void  arch_irq_ack(uint8 irq);

void  arch_interrupt_init(void);

void arch_enable_paging(struct task *task);
void arch_vm_map_page(struct task *task,addr_t phy,addr_t virt);

#endif
