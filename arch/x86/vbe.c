#include "yos.h"

#include "arch/x86/vbe.h"
#include "arch/x86/multiboot.h"

void vbe_init(void) {
  multiboot_info_t *mbi;
  vbe_modeinfo_t *vbeMI;
  vbe_continfo_t *vbeCI;
  
  mbi = asm_get_mbi_addr();
  
  if( mbi->flags & (1<<11) ) {
    kprintf("Found VBE info in MBI\n");
    
    vbeCI = *(vbe_continfo_t**)(  (uint32)mbi + 72 );
    vbeMI = *(vbe_modeinfo_t**)(  (uint32)mbi + 76 );
    
    kprintf(" Signat: %c%c%c%c\n",vbeCI->signature[0],vbeCI->signature[1],vbeCI->signature[2],vbeCI->signature[3]);
    kprintf("    Ver: 0x%x\n",vbeCI->version);

    kprintf("  Width: %u\n",vbeMI->Xres);
    kprintf(" Height: %u\n",vbeMI->Yres);
    kprintf("    BPP: %u\n",vbeMI->bpp);
    kprintf("  Masks: %u %u %u\n",vbeMI->red_mask,vbeMI->green_mask,vbeMI->blue_mask);

  } else {
    kprintf("No VBE info in MBI (flags=0x%x)\n",mbi->flags);
  }

#if VBE30  
  {
  uint8 *pBIOS;
  kprintf("Looking for VBE3.0 PMode signature\n");
  
  pBIOS = (uint8*)0xC0000;
  
  for(i=0;i<(32*1024);i++) {
    if( (pBIOS[i] == 'P') && (pBIOS[i+1] == 'M') && (pBIOS[i+2] == 'I') && (pBIOS[i+3] == 'D') ) {
      kprintf("VBE3.0 Struct found at 0x%x\n",0xc0000+i);
    }
  }
  }
#endif

}
