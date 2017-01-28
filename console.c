#include <stdarg.h>

#include "yos.h"
#include "framebuffer.h"

#define MAX_CHARS 20

void kputs(char *str) {
  while( str[0] != 0 ) {
    framebuffer_putchar(str[0]);
    str++;
  }
}

static void NUMtoSTR10(unsigned int val) {
  unsigned char c = 0,x;
  char str[MAX_CHARS];
  char hex[16] = { "0123456789ABCDEF" };

  // Hook for 0
  if(val == 0) {
    framebuffer_putchar('0');
    return;
  }

  // Scan the int, and push it to the 'str'
  while(val && (c < MAX_CHARS)) {
    str[c++] = hex[val % 10];
    val /= 10;
    }
  str[c] = 0;
  c--;

  // Flip it around
  x = 0;
  while( (c-x) > -1 ) {
    framebuffer_putchar(str[c-x]);
    x++;
  }
}

static void NUMtoSTR16(unsigned int val) {
  unsigned char c = 0,x;
  char str[MAX_CHARS];
  char hex[16] = { "0123456789ABCDEF" };

  // Hook for 0
  if(val == 0) {
    framebuffer_putchar('0');
    return;
  }

  // Scan the int, and push it to the 'str'
  while(val && (c < MAX_CHARS)) {
    str[c++] = hex[val % 16];
    val /= 16;
    }
  str[c] = 0;
  c--;

  // Flip it around
  x = 0;
  while( (c-x) > -1 ) {
    framebuffer_putchar(str[c-x]);
    x++;
  }
}

void kprintf(char *fmt,...) {
  va_list ap;
  int d;
  char *s,c;

  va_start(ap,fmt);
  while(*fmt) {
    if( *fmt == '%' ) {
      fmt++; // Fast-forward one char (*fmt++ earlier)
      switch(*fmt) {
      case 'u':
      case 'd':
      case 'i':
        d = va_arg(ap,int);
        NUMtoSTR10(d);
        break;
      case 'x':
      case 'X':
        d = va_arg(ap,int);
        NUMtoSTR16(d);
        break;
      case 'c':
        //c = va_arg(ap,char); // Gives warning from GCC
        c = va_arg(ap,int);
        framebuffer_putchar(c);
        break;
      case 's':
        s = va_arg(ap,char *);
        kputs(s);
        break;
      default:
        break;
      }
    }
    else {
      framebuffer_putchar(*fmt);
    }
    fmt++;
  }
}
