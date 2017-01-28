#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

#define SPINLOCK_INITIALIZER   0

typedef volatile int spinlock_t;

void spinlock_init(spinlock_t *lock);
void spinlock_unlock(spinlock_t *lock);
void spinlock_lock(spinlock_t *lock);
int  spinlock_locked(spinlock_t *lock);

#endif
