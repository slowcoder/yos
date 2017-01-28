#include "yos.h"
#include "console.h"
#include "pmm.h"

#include "tasks.h"
#include "vm.h"

void Xarch_vm_map_page(struct task *task,addr_t phy,addr_t virt) {
  uint32 i,pte,pde;

  ASSERT((phy  & (VM_PAGESIZE-1)) == 0);
  ASSERT((virt & (VM_PAGESIZE-1)) == 0);

  kprintf("PageDir for task: 0x%x\n",&task->meminfo->pagedir[0]);
  kprintf("EndPDir for task: 0x%x\n",&task->meminfo->pagedir[1023]);

  pde = virt >> 22;
  i = pmm_alloc_pages(1);
  kprintf("PT = 0x%x\n",i);
  task->meminfo->pagedir[pde] = i;

  for(i=0;i<1024;i++) {
    if( task->meminfo->pagedir[i] ) kprintf("PD %u: 0x%x\n",i,task->meminfo->pagedir[i]);
  }
}

void arch_vm_map_page(struct task *task,addr_t phy,addr_t virt) {
  uint32 pde,pte,i,*pt;

  ASSERT((phy  & (VM_PAGESIZE-1)) == 0);
  ASSERT((virt & (VM_PAGESIZE-1)) == 0);

  pde = virt >> 22;
  pte = (virt & 0x3FFFFF) >> VM_PAGESHIFT;

  //kprintf("New page: %u:%u\n",pde,pte);

  if( task->meminfo->pagedir[pde] == 0 ) { // If this entry in the pagedir is empty, we need to allocate a new pagetable for this 4MB range
    pt = (uint32 *)pmm_alloc_pages(1);
    task->meminfo->pagedir[pde] = (uint32)pt;
    for(i=0;i<1024;i++) pt[i] = 0; // Not present
  }

  pt = (uint32 *)task->meminfo->pagedir[pde];
  pt[pte] = virt | 3; // Supervisor,RW,Present

  // !!! Add TLB flushes here

  //kprintf("arch_vm_map_page(%s,0x%x,0x%x)\n",task->name,phy,virt);
  //kprintf(" PDI=%u  PTI=%u\n",pde,pte);
}
