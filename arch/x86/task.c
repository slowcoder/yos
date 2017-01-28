#include "yos.h"
#include "tasks.h"
#include "pmm.h"

#include "arch.h"

#include "arch/x86/cpu.h"
#include "vm.h"

void arch_task_create(struct task *task) {
  uint32 i;

  // Build the VM-pages here to map in the kernel in each task

  // Allocate a new pagedir for this task, and fill all of it as 'not present'
  task->meminfo->pagedir = (uint32 *)pmm_alloc_pages(1);
  kprintf("Allocated PD at 0x%x ",task->meminfo->pagedir);

  for(i=0;i<1024;i++)
    task->meminfo->pagedir[i] = 0; // Not-present

  kprintf("End at 0x%x\n",&task->meminfo->pagedir[i]);

  // Now, map in the 1MB -> 4MB area (kernel)
  for(i=0x100000;i<0x400000;i+=4096)
    arch_vm_map_page(task,i,i);
  //arch_vm_map_page(task,0,0);
}

static __inline__ void set_cr3 (void *cr3) {
  register uint32 value = (uint32) cr3;
  __asm__ __volatile__ ("mov %0, %%cr3" :: "r" (value));
}
static __inline__ unsigned int get_cr0 (void) {
  register unsigned int value;
  __asm__ __volatile__ ("mov %%cr0, %0" : "=r" (value));
  return value;
}

static __inline__ void set_cr0 (unsigned int cr0) {
  register unsigned int value = (unsigned int) cr0;
  __asm__ __volatile__ ("mov %0, %%cr0" :: "r" (value));
}

void Xarch_enable_paging(struct task *task) {
  uint32 i;

  for(i=0;i<1024;i++) {
    if( task->meminfo->pagedir[i] ) kprintf("PD %u: 0x%x\n",i,task->meminfo->pagedir[i]);
  }
}

void arch_enable_paging(struct task *task) {
  uint32 i,*pd,*pt;

  pd = task->meminfo->pagedir;
  kprintf("PD at 0x%x\n",pd);
  for(i=0;i<1024;i++) {
    if(pd[i]) {
      kprintf("%u:%u -> 0x%x\n",i,0,(uint32)pd[i]);
      pt = (uint32 *)((uint32)pd[i] & 0xFFFFF800);
      //for(j=0;j<1024;j++)
      //kprintf(" %u:%u - 0x%x\n",i,j,pt[i]);
    }
  }

  set_cr3(task->meminfo->pagedir); // put that page directory address into CR3
  //set_cr0(get_cr0() | 0x80000000); // set the paging bit in CR0 to 1
}

void arch_thread_create(struct thread *thread,addr_t entry) {
  thread->iframe = (struct iframe *)thread->kstack - sizeof(struct iframe);

  thread->iframe->eip = entry;
  thread->iframe->esp = (uint32)thread->ustack + 0xFFF;  // This should really never be used

  if( thread->task->mode == 0 ) {
    thread->iframe->cs = KERNEL_CS;
    thread->iframe->ds = KERNEL_DS;
    thread->iframe->es = KERNEL_DS;
    thread->iframe->fs = KERNEL_DS;
    thread->iframe->gs = KERNEL_DS;
    thread->iframe->ss = KERNEL_DS;
  } else {
    thread->iframe->cs = USER_CS;
    thread->iframe->ds = USER_DS;
    thread->iframe->es = USER_DS;
    thread->iframe->fs = USER_DS;
    thread->iframe->gs = USER_DS;
    thread->iframe->ss = KERNEL_DS;  // Will be overwritten by user_ss on iret
    thread->iframe->user_ss  = USER_DS;
    thread->iframe->user_esp = (uint32)thread->ustack + 0xFFF;
  }

  thread->iframe->flags = 0x0202;
}
