#include "yos.h"

char *strncpy(char *dest,const char *src,size_t n) {
  char *dst;

  dst = dest;

  while( n && *src ) {
    *dest++ = *src++;
  }
  if(n) *dest = 0;

  return(dst);
}

addr_t strnlen(const char *s,addr_t maxlen) {
  addr_t i = 0;

  while( (i<maxlen) && (*s != 0) ) *s++;

  return(i);
}
