#include "yos.h"
#include "spinlock.h"
#include "pmm.h"
#include "vm.h"
#include "phyheap.h"
#include "libc.h"
#include "arch.h"

static struct task     *tasklist;

static struct runqueue *runqueue       = NULL;
struct        runqueue *running_thread = NULL;

static spinlock_t       runqueuelock   = SPINLOCK_INITIALIZER;

static void idle_thread(void) {
  for(;;) {
    arch_sleep();
  }
}

void tasks_init(void) {
  uint8 cpus;

  // 1. Create a kernel-task
  tasklist = (struct task *)phyheap_kmalloc( sizeof(struct task) );
  tasklist->name = (char *)phyheap_kmalloc(7);
  strncpy(tasklist->name,"Kernel",7);
  tasklist->next = NULL;
  tasklist->mode = 0; // TASK_KERNEL
  tasklist->threads = NULL;

  arch_task_create(tasklist); // Builds the VM-tables and whatnot

  // 2. Create a dummy-thread for the already running thread
  thread_create(tasklist,0);
  tasklist->threads->running = 1; // First thread is running.. Duh..

  running_thread = runqueue;

  // 3. Create numcpus-1 idle-threads in this task (-1 since the first one is the main kernel thread)
  cpus = arch_numcpus();
  while(cpus > 1) {
    thread_create(tasklist,(addr_t)&idle_thread);
    cpus--;
  }

}

struct task *task_kernel(void) {
  return(tasklist);
}

struct task *task_create(char *name) {
  struct task *task;

  task = (struct task *)phyheap_kmalloc( sizeof(struct task) );

  task->name = phyheap_kmalloc(strnlen(name,32));
  strncpy(task->name,name,strnlen(name,32));

  arch_task_create(task);

  return(task);
}

struct thread *thread_create(struct task *task,addr_t entry) {
  struct thread   *thread;
  struct runqueue *lastqueue;

  thread = (struct thread *)phyheap_kmalloc( sizeof(struct thread) );

  thread->state   = THREAD_STATE_RUNNING;
  //thread->cpu     = arch_get_bootprocessor();  // !!! hardcoded
  thread->task    = task;
  thread->running = 0;
  thread->next    = NULL;
  thread->kstack  = (void *)pmm_alloc_pages(1);
  thread->ustack  = (void *)pmm_alloc_pages(1);

  arch_thread_create(thread,entry);

  // Okay, we're all done setting up the thread.. Let's add it to the runqueue
  spinlock_lock(&runqueuelock);

  if( runqueue == NULL ) {
    runqueue = (struct runqueue *)phyheap_kmalloc( sizeof(struct runqueue) );
    runqueue->prev   = NULL;
    runqueue->next   = NULL;
    runqueue->thread = thread;
  }
  else {
    for( lastqueue=runqueue; lastqueue->next != NULL; lastqueue = lastqueue->next );
    // lastqueue now points to the last entry in the runqueue

    lastqueue->next = (struct runqueue *)phyheap_kmalloc( sizeof(struct runqueue) );
    lastqueue->next->next   = NULL;
    lastqueue->next->prev   = lastqueue;
    lastqueue->next->thread = thread;
    lastqueue = lastqueue->next;
  }
  spinlock_unlock(&runqueuelock);

  return(thread);
}

void scheduler(void) {
  spinlock_lock(&runqueuelock);
  //ASSERT(0);

  running_thread->thread->running--;
  if( running_thread->next == NULL ) running_thread = runqueue;
  else running_thread = running_thread->next;
  running_thread->thread->running++;

  spinlock_unlock(&runqueuelock);
}

void tasks_debug(void) {
  struct runqueue *queue;

  for(queue = runqueue; queue != NULL; queue = queue->next ) {
    kprintf("Queue 0x%x\n",queue);
    kprintf(" Thread: 0x%x\n",queue->thread);
    kprintf(" Next  : 0x%x\n",queue->next);
  }
}
