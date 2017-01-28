#include "yos.h"
#include "console.h"
#include "framebuffer.h"
#include "pmm.h"

void arch_init(void) {
  framebuffer_init();

  kprintf("yOS v%s booting on a Hitachi SH4 (Dreamcast)\n",YOS_VERSION);
  kprintf("Installed SDRAM: 16MB\n");

  //vm_add_phymem(0x8c100000,0xEFFFFF);
  pmm_register_ram(0x8c100000,0xE00000);
}

uint8 arch_numcpus(void) {
  return(1);
}

void arch_sleep(void) {
  // Afaik, the SH4 cannot halt
}
