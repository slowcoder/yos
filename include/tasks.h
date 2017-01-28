#ifndef _TASKS_H_
#define _TASKS_H_

/*
#include "cpu.h"
#include "memory.h"
*/
#ifdef x86
#include "arch/x86/task.h"
#endif
#ifdef sh4
#include "arch/sh4/task.h"
#endif

#include "vm.h"

#define THREAD_STATE_STOPPED 0
#define THREAD_STATE_RUNNING 1

struct thread {
  uint8           state;
  struct task    *task;
  volatile uint32 running;

  struct iframe  *iframe;
  void           *kstack;
  void           *ustack;
  addr_t          usp,ksp;  // Only used by architectures without TSS equivalents

  struct thread  *next;
};

struct task {
  char             *name;
  uint8             mode;  // 0 = Kernel, 1 = Usermode

  struct thread    *threads;
  struct vm_region *regionlist,*lastregion;
  struct meminfo   *meminfo;

  struct task      *next;
};

struct runqueue {
  struct runqueue *prev;
  struct thread   *thread;
  struct runqueue *next;
};


void tasks_init(void);
struct task *task_kernel(void);
struct task *task_create(char *name);
struct thread *thread_create(struct task *task,addr_t entry);
void tasks_debug(void);

void scheduler(void);


#endif
