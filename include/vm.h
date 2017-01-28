#ifndef _VM_H_
#define _VM_H_

#include "yos.h"
#include "tasks.h"

#ifdef x86
#define VM_PAGESIZE  4096
#define VM_PAGESHIFT 12
#endif
#ifdef sh4
#define VM_PAGESIZE 4096
#define VM_PAGESHIFT 12
#endif

#define VM_ROUND_PAGE(a) ((((addr_t)(a)) + VM_PAGESIZE) & ~VM_PAGESIZE)
#define VM_TRUNC_PAGE(a) ((((addr_t)(a)) & ~VM_PAGESIZE))

struct vm_region {
  struct task *task;

  addr_t base,length;

  struct vm_pagelist *pagelist;

  struct vm_region *next;
};

struct vm_pagelist {
  uint32 type;      // 0 = Physical RAM, 1 = Swapped to disk
  uint32 flags;     // R,W,X,Wired,Etc,Etc
  uint32 refcount;  // How many regions that are linked to us.. If 0, then release all pages
  struct vm_page *pages;
};

struct vm_page {
  addr_t  phy;
  addr_t  offset; // Offset in the region
  struct vm_page *next;
};

void vm_address_range(addr_t min,addr_t max);
void            vm_add_phymem(addr_t base,size_t length);
void vm_add_kernel(addr_t base,size_t length);
void vm_build_tables(void);

struct vm_page *vm_get_page(void);
void            vm_put_page(struct vm_page *page);
kern_ret        vm_new_region(struct task *task,addr_t base,size_t length);
void            vm_pagefault(addr_t addr);

void vm_debug(void);
void vm_debug_task(struct task *task);

#endif
