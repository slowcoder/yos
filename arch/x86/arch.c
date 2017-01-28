#include "yos.h"
#include "framebuffer.h"
#include "vm.h"
#include "pmm.h"
#include "arch/x86/pci.h"
#include "arch/x86/vbe.h"

#include "arch/x86/multiboot.h"

void arch_init(void) {
  multiboot_info_t *mbi;

  mbi = asm_get_mbi_addr();

  framebuffer_init();

  kprintf("yOS v%s booting on a x86\n",YOS_VERSION);
  kprintf("Installed RAM: %uMB\n",(1024 + mbi->mem_upper) / 1024);

  pmm_register_ram(0x200000,(mbi->mem_upper * 1024) - 0x100000);

  pci_init();
  vbe_init();
}

uint8 arch_numcpus(void) {
  return(1);
}

void arch_sleep(void) {
  asm("hlt");
}
