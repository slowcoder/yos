#include "yos.h"
#include "vm.h"
#include "console.h"
#include "spinlock.h"

static spinlock_t pmmlock = SPINLOCK_INITIALIZER;

struct phypage {
  uint8  use; // 0 = Free, 1 = Allocated, 2 = Reserved, 3 = PCI space
  addr_t phy;
};

struct phypage *phypage = NULL;
uint32 phypages = 0;

void pmm_register_ram(addr_t base,size_t size) {
  uint32 pages,headsize,ramsize,i;

  ASSERT( !(base &  (VM_PAGESIZE-1)) );
  ASSERT( !(size &  (VM_PAGESIZE-1)) );

  pages = size >> VM_PAGESHIFT;  // Total number of pages
  
  headsize = sizeof(struct phypage) * pages;
  headsize = VM_ROUND_PAGE(headsize); // Total size that the headers will occupy. (Some loss here)

  ramsize  = VM_TRUNC_PAGE(size - headsize);
  pages    = ramsize >> VM_PAGESHIFT; // Number of available pages minus the pages the headers are in
  phypages = pages;

#if 0
  phypage = (struct phypage *)base;
  kprintf("pmm_register_ram(0x%x,%u)\n",base,size);

  kprintf("Tot: %ub, Head: %u, Space %u\n",size,headsize,ramsize);

  kprintf("Sizeof pmm: 0x%x\n",sizeof(struct phypage));
  kprintf("PhyPage0.use: 0x%x\n",&phypage[0].use);
  kprintf("PhyPage1.use: 0x%x\n",&phypage[1].use);
  kprintf("PhyPage2.use: 0x%x\n",&phypage[2].use);

  for(;;);
#endif

  phypage = (struct phypage *)base;

  for(i=0;i<pages;i++) {
    phypage[i].use = 0;
    phypage[i].phy = base + headsize + (i * VM_PAGESIZE);
  }
}

addr_t pmm_alloc_pages(uint32 pages) {
  uint32 i,j;

  ASSERT(pages < phypages);
  ASSERT(pages != 0);
  ASSERT(phypage != NULL);

  spinlock_lock(&pmmlock);

  for(i=0;i<(phypages-pages);i++) { // Look for a free page that we're later going to check if there's free pages behind it
    if( phypage[i].use == 0 ) { // If it's free
      for(j=0;j<pages;j++) { // Check if the trailing blocks are okay
	if( phypage[i+j].use != 0 ) j = pages + 1; // Not enough free blocks
      }
      if(j==pages) { // This run was okay
	for(j=0;j<pages;j++) phypage[i+j].use = 1; // Mark them as allocated
	spinlock_unlock(&pmmlock);

	//kprintf("Allocated 0x%x->0x%x\n",phypage[i].phy,phypage[i].phy + (pages * VM_PAGESIZE));

	return(phypage[i].phy);
      }
    }
  }

  spinlock_unlock(&pmmlock);
  ASSERT(0);
  return(0);
}

void pmm_debug(void) {}
#if 0
void Xpmm_register_ram(addr_t base,size_t size) {
  uint32 pages,tmp;

  ASSERT( !(base &  (VM_PAGESIZE-1)) );
  ASSERT( !(size &  (VM_PAGESIZE-1)) );

  phypage = (struct phypage *)base;

  // Calc how many pages that struct will take up, and remove that from the total
  pages = size >> VM_PAGESHIFT;
  tmp   = pages * sizeof(struct phypage);
  tmp   = (tmp >> VM_PAGESHIFT)+1;
  pages -= tmp;

  ASSERT((base+size)>base+pages*(sizeof(struct phypage)+VM_PAGESIZE));

  // Shift the base up
  base += pages * sizeof(struct phypage);
  
  base &= ~(VM_PAGESIZE-1);
  base += VM_PAGESIZE;

  phypages = pages;

  for(tmp=0;tmp<pages;tmp++) {
    phypage[tmp].use = 0;
    phypage[tmp].phy = base + (tmp * VM_PAGESIZE);

    ASSERT((phypage[tmp].phy & (VM_PAGESIZE-1)) == 0);
  }
}

addr_t Xpmm_alloc_pages(uint32 pages) {
  uint32 i,j;

  ASSERT(pages < phypages);

  spinlock_lock(&pmmlock);

  for(i=0;i<(phypages-pages);i++) {

    //kprintf("[PMM ] Scanning off: %u\n",i);
    if( !phypage[i].use ) { // Yup, this page is free, let's check if the trailing ones are
      for(j=0;j<pages;j++) {
	if( phypage[i+j].use ) j = pages + 1; // Nope, this run wasn't okay
      }
      if(j==pages) { // This run was okay, so allocate it
	kprintf("[PMM] Pages %u to %u allocated (0x%x -> 0x%x)\n",i,i+pages-1,phypage[i].phy,phypage[i].phy+(pages-1)*VM_PAGESIZE);
	for(j=0;j<pages;j++) {
	  phypage[i+j].use       = 1;
	}
	//kprintf("[PMM ] Phys: 0x%x\n",pmm_page[i].phys);
	spinlock_unlock(&pmmlock);
	return(phypage[i].phy);
      } else {
	kprintf("PAGERUN not ok\n");
      }
    }
  }
  spinlock_unlock(&pmmlock);
  kprintf("[PMM ] Null?\n");
  return( (addr_t)NULL );
}

void pmm_debug(void) {
  uint32 usage[4],i;

  usage[0] = usage[1] = usage[2] = usage[3] = 0;

  for(i=0;i<phypages;i++) {
    usage[ phypage[i].use ]++;
  }

  kprintf("[PMM ] Pages free/used/reserved/pci: %u/%u/%u/%u\n",usage[0],usage[1],usage[2],usage[3]);
}
#endif
#if 0
struct pmm_struct {
  uint8   use;  // 0 = Free, 1 = Allocated, 2 = Reserved, 3 = PCI space
  addr_t  phy;
  struct pmm_struct *next;
};

static struct pmm_struct *pagelist = NULL;
static struct pmm_struct *lastpage = NULL;
//static uint32 pmm_pages;

/*
 * This will add a chunk of RAM to the pagelist.
 * Can be called multiple times
 */
void pmm_register_ram(addr_t base,size_t size) {
  uint32 pages,i,ssize;
  struct pmm_struct *tmppage;

  // Make sure everything is aligned
  /*  kprintf("Base: 0x%x  Size: 0x%x\n",base,size);
  kprintf("         (VM_PAGESIZE-1) = 0x%x\n",(VM_PAGESIZE-1));
  kprintf(" base &  (VM_PAGESIZE-1) = 0x%x\n",base & (VM_PAGESIZE-1));
  kprintf(" size &  (VM_PAGESIZE-1) = 0x%x\n",size & (VM_PAGESIZE-1));*/
  ASSERT( !(base &  (VM_PAGESIZE-1)) );
  ASSERT( !(size &  (VM_PAGESIZE-1)) );

  pages = size >> VM_PAGESHIFT;

  ssize = (((pages * sizeof(struct pmm_struct)) >> VM_PAGESHIFT)+1) << VM_PAGESHIFT; // VM_ROUND_UP
  pages -= ssize >> VM_PAGESHIFT; // Since the structs take up some of the space

  ASSERT(pages>1);
  
  tmppage = (struct pmm_struct *)base;
  base   += ssize;
  
  i = 0;

  // If the pagelist is empty, we'll need to 'prime' it
  if(pagelist == NULL) pagelist = tmppage;

  lastpage->next = tmppage;

  kprintf("Adding %u pages\n",pages);
  //kprintf("Struct addr: ");
  for(;i<pages;i++) {
    //kprintf("0x%x ",(uint32)tmppage);
    tmppage->phy   = base + (i * VM_PAGESIZE);
    tmppage->use   = 0;
    tmppage->next  = (struct pmm_struct *)((uint32)tmppage + sizeof(struct pmm_struct));

    lastpage = tmppage;
    tmppage = tmppage->next;
  }
  //kprintf("\n");
  lastpage->next = NULL;
}

addr_t pmm_alloc_pages(uint32 pages) {
  uint32 i;
  struct pmm_struct *page,*pagei;

  spinlock_lock(&pmmlock);
  //kprintf("[PMM ] Allocating %u pages\n",pages);

  for(page=pagelist;page!=NULL;page=page->next) {
    pagei = page;
    for(i=0;i<pages;i++) {
      // Check usage
      if(pagei->use) i = pages + 1;

      // Check contiguity
      if(pagei->next->phy != (pagei->phy + VM_PAGESIZE)) i = pages + 1;

      pagei = pagei->next;
    }
    if(i==pages) { // Yep, it all checks out, allocate the same block
      pagei = page;
      for(i=0;i<pages;i++) {
	pagei->use = 1;
	pagei = pagei->next;
      }
      spinlock_unlock(&pmmlock);
      return(page->phy);
    }
  }

  spinlock_unlock(&pmmlock);
  return( (addr_t)NULL );
}

void pmm_debug(void) {
  uint32 usage[4];
  struct pmm_struct *page;

  usage[0] = usage[1] = usage[2] = usage[3] = 0;

  for(page=pagelist;page!=NULL;page=page->next) {
    //kprintf("DAddr: 0x%x -> 0x%x\n",(uint32)page,page->phy);
    usage[ page->use ]++;
  }

  kprintf("[PMM ] Pages free/used/reserved/pci: %u/%u/%u/%u\n",usage[0],usage[1],usage[2],usage[3]);
}

#endif
