#include "yos.h"

//#include "console.h"
#include "arch/x86/vbe.h"
#include "arch/x86/multiboot.h"
#include "libc.h"

//#include "arch/x86/framebuffer.h"
#include "framebuffer-font.h"

static uint16 cursor_x,cursor_y;

static struct {
  uint16 *pFB;
  uint16 width,height,pitch;
  uint8  colorformat;
} fbinfo;

/*
 * These are all internal unexported functions
 */
static inline void put_pixel(unsigned short x,unsigned short y,unsigned short col) {
//  unsigned short *vram = (unsigned short *)0xA5000000;

  fbinfo.pFB[((y*640)+x)] = col;
}

/*
 * From hereon down, it's all exported functions
 */
void framebuffer_init(void) {
  multiboot_info_t *mbi;
  vbe_modeinfo_t *vbeMI;
  vbe_continfo_t *vbeCI;
  
  mbi = asm_get_mbi_addr();
  /* Double-check that we've got VBE support */
  if( !( mbi->flags & (1<<11) ) )
    return;
    
  vbeCI = *(vbe_continfo_t**)(  (uint32)mbi + 72 );
  vbeMI = *(vbe_modeinfo_t**)(  (uint32)mbi + 76 );


  fbinfo.pFB    = (uint16*)vbeMI->physbase;
  fbinfo.width  = vbeMI->Xres;
  fbinfo.height = vbeMI->Yres;
  fbinfo.pitch  = vbeMI->pitch;
  
  /* Clear screen to white */
  memset( fbinfo.pFB, 0xFF, fbinfo.pitch * fbinfo.height );

  cursor_x = 0; cursor_y = 0;
}

void framebuffer_gotoxy(uint16 x,uint16 y) {
  cursor_x = x; cursor_y = y;
}

void framebuffer_putchar(char ch) {
  uint16 i,x,y;

  if( ch == '\n' ) {
    cursor_y++; cursor_x = 0;
    if(cursor_y > 39) { cursor_x = 0; cursor_y = 0; }
    return;
  } else if( ch == '\r' ) {
    cursor_x = 0;
    return;
  }
  x = cursor_x * 7;
  y = cursor_y * 12;

  // Black text on white background
  for(i=0;i<12;i++) {
    if( FRAMEBUFFER_FONT[(ch*12)+i] &   1 ) put_pixel(x+0,y+i,0); else put_pixel(x+0,y+i,(31<<11)+(63<<5)+31);
    if( FRAMEBUFFER_FONT[(ch*12)+i] &   2 ) put_pixel(x+1,y+i,0); else put_pixel(x+1,y+i,(31<<11)+(63<<5)+31);
    if( FRAMEBUFFER_FONT[(ch*12)+i] &   4 ) put_pixel(x+2,y+i,0); else put_pixel(x+2,y+i,(31<<11)+(63<<5)+31);
    if( FRAMEBUFFER_FONT[(ch*12)+i] &   8 ) put_pixel(x+3,y+i,0); else put_pixel(x+3,y+i,(31<<11)+(63<<5)+31);
    if( FRAMEBUFFER_FONT[(ch*12)+i] &  16 ) put_pixel(x+4,y+i,0); else put_pixel(x+4,y+i,(31<<11)+(63<<5)+31);
    if( FRAMEBUFFER_FONT[(ch*12)+i] &  32 ) put_pixel(x+5,y+i,0); else put_pixel(x+5,y+i,(31<<11)+(63<<5)+31);
    if( FRAMEBUFFER_FONT[(ch*12)+i] &  64 ) put_pixel(x+6,y+i,0); else put_pixel(x+6,y+i,(31<<11)+(63<<5)+31);
    if( FRAMEBUFFER_FONT[(ch*12)+i] & 128 ) put_pixel(x+7,y+i,0); else put_pixel(x+7,y+i,(31<<11)+(63<<5)+31);
  }

  cursor_x++;
  if(cursor_x > 90) { cursor_x = 0; cursor_y++; }
  if(cursor_y > 39) { cursor_x = 0; cursor_y = 0; }
}
