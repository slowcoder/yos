#include "yos.h"
#include "pmm.h"
#include "vm.h"
#include "console.h"

#include "phyheap.h"

void phyheap_init(void) {}
void *phyheap_kmalloc(addr_t size) {
  uint32 addr;

  size = VM_ROUND_PAGE(size);
  size = size >> VM_PAGESHIFT;
  
  addr = pmm_alloc_pages(size + 1);

  return( (void *)addr );
}

void phyheap_kfree(void *ptr) {}
void phyheap_debug(void) {}

#if 0
struct phyheap *phyheap_first;

void phyheap_init(void) {
  // Okay, we start out with allocating just one page for the kernel-heap.. It can always add more later

  phyheap_first = (struct phyheap *)pmm_alloc_pages(1);
  phyheap_first->size  = 4096;
  phyheap_first->magic = PHYHEAP_MAGIC;
  phyheap_first->used  = 0;
  phyheap_first->next  = (struct phyheap *)NULL;
}

void *phyheap_kmalloc(addr_t size) {
  struct phyheap *m,*n;
  addr_t          addr,pages;

  // First thing to do is to scan through the heap and see if we've got room for the allocation already
  for(m = (struct phyheap *)phyheap_first;m != NULL;m = m->next) {
    if(m->used) continue;

    if(size < m->size) { // We found room for it
      addr = (addr_t)m + size + sizeof(struct phyheap);
      n = (struct phyheap *)addr;

      n->used  = 0;
      n->magic = PHYHEAP_MAGIC;
      n->next  = m->next;
      n->size  = m->size - sizeof(struct phyheap) - size;
      m->next  = n;
      m->size  = size;
      m->used  = 1;

      return( (void *)( (addr_t)m + sizeof(struct phyheap) ) );
    }
  }

  // If we got here, we didn't find room in the heap for it, so lets add some pages to the heap
  pages = (size / VM_PAGESIZE) + 1;
  //kprintf("[PHYH] Expanding heap by %u pages..",pages);

  for(m = (struct phyheap *)phyheap_first;m->next != NULL;m = m->next); // FFWD to the last entry in the heap
  
  m->next = (struct phyheap *)pmm_alloc_pages(pages);
  if(m->next == NULL) {
    kprintf("[PHYH] Failed to allocate more pages for kernel-heap\n");
    return(NULL);
  }
  //kprintf("done\n");

  // Check to see if we allocated enough to have some spare room in these pages
  if( (size + (sizeof(struct phyheap)*2)) < (pages * VM_PAGESIZE) ) {
    m = m->next;
    m->size = size;
    m->used = 1;
    m->magic = PHYHEAP_MAGIC;

    // This marks what's left of the allocated block as free
    n = (struct phyheap *)((addr_t)m + sizeof(struct phyheap));
    m->next  = n;
    n->size  = (pages * VM_PAGESIZE) - size - (2 * sizeof(struct phyheap));
    n->used  = 0;
    n->magic = PHYHEAP_MAGIC;
    n->next  = (struct phyheap *)NULL;

    return( (void *)( (addr_t)m + sizeof(struct phyheap) ) );
  } else { // We didn't
    m = m->next;
    m->size  = pages * VM_PAGESIZE;
    m->used  = 1;
    m->magic = PHYHEAP_MAGIC;
    m->next  = (struct phyheap *)NULL;
    return( (void *)( (addr_t)m + sizeof(struct phyheap) ) );
  }

  return(NULL); // We should never get here..
}

void phyheap_kfree(void *ptr) {
  struct phyheap *m,*n;

  m = (struct phyheap *)(ptr - sizeof(struct phyheap));
  if(m->magic != PHYHEAP_MAGIC) {
    kprintf("[PHYH] Attempt to kfree() a block at 0x%x with bad magic-number\n",m);
    return;
  }

  n = (struct phyheap *)phyheap_first;
  if(n->magic != PHYHEAP_MAGIC) {
    kprintf("[PHYH] Kernel heap corrupt in kfree()\n");
    return;
  }
  for(; n != NULL; n = n->next) {
    if(n==m) break;
  }
  if(n == NULL) {
    kprintf("[PHYH] Attept to free a block (0x%x) that's not part of the kernel heap\n",ptr);
    return;
  }

  // Okay, all seems to check out.. Free the block
  m->used = 0;

  // Coalesce adjacent free blocks 
  for(m = (struct phyheap *)phyheap_first; m != NULL; m = m->next) {
    while(!m->used && m->next != NULL && !m->next->used) {
      // resize this block
      m->size += sizeof(struct phyheap) + m->next->size;
      // merge with next block
      m->next = m->next->next;
    }
  }
  
}


void phyheap_debug(void) {
  struct phyheap *m;

  kprintf("======= HEAP DUMP ========\n");
  for(m = (struct phyheap *)phyheap_first;m != NULL;m = m->next) {
    if(m->magic == PHYHEAP_MAGIC) {
      kprintf("Heapheader addr: 0x%x\n",m);
      kprintf(" Use: %x    Size: 0x%x",m->used,m->size);
      kprintf(" Next: 0x%x",m->next);
      kprintf("\n");
    } else {
      kprintf("Corrupt heap!!\n");
      m->next = NULL;
    }
  }
  kprintf("==========================\n");
}

#endif
