#ifndef _PMM_H_
#define _PMM_H_

#include "yos.h"

void pmm_register_ram(addr_t base,size_t size);
addr_t pmm_alloc_pages(uint32 pages);
void pmm_debug(void);

#endif
