#ifndef _PHYHEAP_H_
#define _PHYHEAP_H_

#define PHYHEAP_MAGIC 0x3141592

struct phyheap {
  addr_t size;
  struct phyheap *next;
  uint32 magic : 31,
          used : 1;
};

void  phyheap_init(void);
//void  phyheap_addram(addr_t start,addr_t end);
void *phyheap_kmalloc(addr_t size);
void  phyheap_kfree(void *ptr);

void  phyheap_debug(void);

#endif
