#ifndef _LIBC_H_
#define _LIBC_H_

void  *memset(void *s, int c, addr_t count);
void   bzero(void *dst, addr_t count);
void   memcpy(char *dest,char *src,int n);
int    memcmp(const void *s1,const void *s2,int n);
addr_t strnlen(const char *s,addr_t maxlen);
char  *strncpy(char *dest,const char *str,size_t n);

#endif
