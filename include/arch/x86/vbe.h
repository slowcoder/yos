#ifndef _ARCH_X86_VBE_H_
#define _ARCH_X86_VBE_H_

void vbe_init(void);

typedef struct {
  uint16 attributes;
  uint8 winA,winB;
  uint16 granularity;
  uint16 winsize;
  uint16 segmentA, segmentB;
  uint32 *realFctPtr;
  uint16 pitch; // bytes per scanline

  uint16 Xres, Yres;
  uint8 Wchar, Ychar, planes, bpp, banks;
  uint8 memory_model, bank_size, image_pages;
  uint8 reserved0;

  uint8 red_mask, red_position;
  uint8 green_mask, green_position;
  uint8 blue_mask, blue_position;
  uint8 rsv_mask, rsv_position;
  uint8 directcolor_attributes;

  uint32 physbase;  // your LFB address ;)
  uint32 reserved1;
  short reserved2;
} vbe_modeinfo_t;

typedef struct {
   char signature[4];             // == "VESA"
   short version;                 // == 0x0300 for VBE 3.0
   short oemString[2];            // isa vbeFarPtr
   unsigned char capabilities[4];
   short videomodes[2];           // isa vbeFarPtr
   short totalMemory;             // as # of 64KB blocks
} vbe_continfo_t;


#endif
