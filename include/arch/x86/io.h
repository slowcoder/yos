#ifndef _IO_H_
#define _IO_H_

#define out8(port,value) \
__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))
                                                                                
#define out16(port,value) \
__asm__ ("outw %%ax,%%dx"::"a" (value),"d" (port))
                                                                                
#define out32(port,value) \
__asm__ ("outl %%eax,%%dx"::"a" (value),"d" (port))

#define in8(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
_v; \
})
                                                                                
#define in16(port) ({ \
unsigned short _v; \
__asm__ volatile ("inw %%dx,%%ax":"=a" (_v):"d" (port)); \
_v; \
})
                                                                                
#define in32(port) ({ \
unsigned int _v; \
__asm__ volatile ("inl %%dx,%%eax":"=a" (_v):"d" (port)); \
_v; \
})

#endif
