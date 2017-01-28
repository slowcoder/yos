/*
 * vm.c
 *
 */
#include "yos.h"
#include "console.h"
#include "tasks.h"
#include "kmalloc.h"

#include "vm.h"

struct vm_page *freelist   = NULL;
struct vm_page *lastfree   = NULL;
struct vm_page *kernellist = NULL;

struct vm_region kernelregion;
struct vm_region kernelpagelist;

static addr_t addressspace[2],kernelspace[2],addressspacesize;

kern_ret vm_new_region(struct task *task,addr_t base,size_t length) {
  ASSERT(freelist != NULL);

  if(task->regionlist == NULL) { // Okay, this is the first region
    // !!! spinlock
    task->regionlist = (struct vm_region *)kmalloc( sizeof(struct vm_region) );

    ASSERT(task->regionlist != NULL);

    task->regionlist->next = NULL;
    task->regionlist->prev = NULL;

    task->regionlist->pagelist = NULL;
    task->regionlist->base     = base;
    task->regionlist->length   = length;
    task->regionlist->task     = task;

    task->lastregion = task->regionlist;
  } else {
    task->lastregion->next = (struct vm_region *)kmalloc( sizeof(struct vm_region) );

    ASSERT(task->lastregion->next != NULL);

    task->lastregion->next->prev = task->lastregion;
    task->lastregion->next->next = NULL;

    task->lastregion->next->pagelist = NULL;
    task->lastregion->next->base     = base;
    task->lastregion->next->length   = length;
    task->lastregion->next->task     = task;

    task->lastregion = task->lastregion->next; // !!! Spinlock
  }
  return(KERN_OK);
}

void vm_handle_pagefault(struct task *task,addr_t addr) {
  kprintf("Pagefault occured at 0x%x for task \"%s\"\n",addr,task->name);
  for(;;);
}

/*
 * !!! Serious cheating going on here.. It assumes that the freelist
 * contains contigous pages, which it won't if we allow freeing of pages
 */
void *vm_kalloc_pages(uint32 pages) {
  uint32 i;
  struct vm_page *tmp;

  tmp = lastfree;
  for(i=pages;i>1;i--) tmp = tmp->prev;

  lastfree       = tmp->prev;
  lastfree->next = NULL;

  //kprintf("Ret: 0x%x\n",tmp->phy);

  return( (void*)tmp->phy );
}

/*
 * Prepares a tasks VM-space by mapping in the kernel-region in it
 */
void vm_initialize_task(struct task *task) {
  task->regionlist = (struct vm_region *)kmalloc( sizeof(struct vm_region) );

  ASSERT(task->regionlist != NULL);
  
  task->regionlist->base     = VM_KERNELBASE;
  task->regionlist->length   = VM_KERNELLEN;
  task->regionlist->task     = task;
  task->regionlist->pagelist = kernellist;
  task->regionlist->next     = NULL;
  task->regionlist->prev     = NULL;
  task->lastregion = task->regionlist;
}

void vm_debug(void) {
  struct vm_page *page;
  uint32 pages,low,high;
  
  pages = high = 0;
  low = -1;
  for(page=freelist;page!=NULL;page=page->next) {
    if(low  > page->phy) low  = page->phy;
    if(high < page->phy) high = page->phy;
    pages++;
  }

  kprintf("%u pages, %u MB RAM free (0x%x-0x%x)\n",pages,pages/256,low,high);

  pages = high = 0;
  low = 0xFFFFFFFF;
  for(page=kernellist;page->next!=NULL;page=page->next) {
    if(low  > page->phy) low  = page->phy;
    if(high < page->phy) high = page->phy;
    if(low==0) kprintf("x at %u\n",pages);
    pages++;
  }
  kprintf("%u pages, %u MB RAM for kernel (0x%x-0x%x)\n",pages,pages/256,low,high);
}

void vm_debug2(void) {
  struct vm_page *page;
  uint32 i;
  
  page = lastfree;
  for(i=0;i<4;i++) {
    kprintf("Page %u -",i);
    kprintf(" Physical: 0x%x\n",page->phy);
    page = page->prev;
  }
    
}
