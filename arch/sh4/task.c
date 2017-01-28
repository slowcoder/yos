#include "yos.h"
#include "tasks.h"

void arch_task_create(struct task *task) {
  // Build the VM-pages here to map in the kernel in each task
}

void arch_thread_create(struct thread *thread,addr_t entry) {
}
