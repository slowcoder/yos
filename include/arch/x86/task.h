#ifndef _X86_TASK_H_
#define _X86_TASK_H_

#include "yos.h"

struct meminfo {
  uint32 *pagedir;
};

struct iframe {
  uint32 gs;
  uint32 fs;
  uint32 es;
  uint32 ds;
  uint32 ss;
  uint32 edi;
  uint32 esi;
  uint32 ebp;
  uint32 esp;
  uint32 ebx;
  uint32 edx;
  uint32 ecx;
  uint32 eax;
  uint32 vector;
  uint32 error_code;
  uint32 eip;
  uint32 cs;
  uint32 flags;
  uint32 user_esp;
  uint32 user_ss;
};

#endif
