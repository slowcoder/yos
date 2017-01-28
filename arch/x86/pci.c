#include "yos.h"
#include "arch/x86/io.h"

#include "arch/x86/pci.h"

#define PCI_CONF1_ADDRESS(bus,fn,reg) (0x80000000 | (bus << 16) | (fn << 8) | (reg & ~3))

static int pci_conf1_read(unsigned int bus,
                          unsigned int devfn, int reg, int len, uint32 *value)
{
        if ((bus > 255) || (devfn > 255) || (reg > 255)) {
                *value = -1;
                return -1;
        }

        out32( 0xCF8, PCI_CONF1_ADDRESS(bus, devfn, reg) );

        switch (len) {
        case 1:
                *value = in8(0xCFC + (reg & 3));
                break;
        case 2:
                *value = in16(0xCFC + (reg & 2));
                break;
        case 4:
                *value = in32(0xCFC);
                break;
        }

        return 0;
}

typedef struct {
  uint16 uClass;
  char  *pzClass;
} classid_t;

static classid_t classid[] = { { 0x0300, "VGA Compatible controller" },
                               { 0x0680, "Bridge" },
                               { 0x0200, "Ethernet controller" },
                               { 0x0C03, "USB Controller" },
                               { 0x0600, "Host bridge" },
                               { 0x0601, "ISA Bridge" },
                               { 0x0101, "IDE Bridge" },
                               { 0x0401, "Multimedia audio controller" },
                               { 0xFFFF, "Unknown" } };

static char *ResolveClass(uint16 uClass) {
  int i;
  
  i=0;
  while( classid[i].uClass != 0xFFFF ) {
    if( classid[i].uClass == uClass ) return classid[i].pzClass;
    i++;
  }
  return classid[i].pzClass;
}

static void pci_probe(void) {
  unsigned int dev;
  uint32   confhead[2];
  
  for(dev=0;dev<256;dev++) {
    pci_conf1_read(0,dev,0,4,&confhead[0]);
    pci_conf1_read(0,dev,8,4,&confhead[1]);
    if( confhead[0] != 0xFFFFFFFF )
      kprintf("[PCI] (Bus/Dev) %x:%x -> %x:%x (%s)\n",0,dev,confhead[0] & 0xFFFF,confhead[0] >> 16,ResolveClass(confhead[1]>>16));
  }
}

int pci_init(void) {
  unsigned int t;
  
  out8(0xCFB,0x01);
  t = in32(0xCF8);
  out32(0xCF8,0x80000000);

  if( in32(0xCF8) != 0x80000000 ) { /* PCI type1 is NOT OK */
    kprintf("[PCI] No PCI detected\n");
    return 1;
  }
  
  kprintf("[PCI] Subsystem initializing (Type 1)\n");
  out32(0xCF8,t);
  pci_probe();
  return 0;
}
