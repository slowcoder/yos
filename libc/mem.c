#include "yos.h"

void *memset(void *s, int c, addr_t count) {
  char *xs = (char *) s;
  
  while (count--)
    *xs++ = c;
  
  return(s);
}

void bzero(void *dst, addr_t count) {
  memset(dst, 0, count);
}

void memcpy(char *dest,char *src,int n) {
  while(n) {
    *dest++ = *src++;
    n--;
  }
}

int memcmp(const void *s1,const void *s2,int n) {
  unsigned char *ss1,*ss2;

  ss1 = (char *)s1; ss2 = (char *)s2;
  while(n) {
    if(*ss1 < *ss2) return(-1);
    if(*ss1 > *ss2) return(1);
    *ss1++; *ss2++;
    n--;
  }
  return(0);
}
