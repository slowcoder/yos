#include "yos.h"

//#include "console.h"
#include "arch/sh4/framebuffer.h"
#include "framebuffer-font.h"

static uint16 cursor_x,cursor_y;

/*
 * These are all internal unexported functions
 */
static inline void put_pixel(unsigned short x,unsigned short y,unsigned short col) {
  unsigned short *vram = (unsigned short *)0xA5000000;

  vram[((y*640)+x)] = col;
}

/*
 * From hereon down, it's all exported functions
 */
void framebuffer_init(void) {
  framebuffer_lowlevel_init(framebuffer_check_cable(),0);
  framebuffer_clrscr(0xFFFFFFFF);

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
