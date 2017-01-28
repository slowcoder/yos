#include "yos.h"
#include "include/arch.h"

#include "spinlock.h"

int arch_tns(volatile int *val, int set_to, int test_val);  // Test-and-set

/* Initialize LOCK.  */
inline void spinlock_init (spinlock_t *lock) {
  *lock = SPINLOCK_INITIALIZER;
}

/* Unlock LOCK.  */
inline void spinlock_unlock(spinlock_t *lock) {
   arch_tns(lock,0,1);
}

/* Try to lock LOCK; returns the old value of the lock, i.e 0 if we locked, 1 if we didn't */
inline int spinlock_try_lock(spinlock_t *lock) {
  return( arch_tns(lock,1,0) );
}

/* Try to lock LOCK; spin until we got it.  */
inline void spinlock_lock(spinlock_t *lock) {
  while(*lock == 1)
    while( spinlock_try_lock(lock) );
}
#if 0
  while (! spinlock_try_lock (lock))
    while (*lock == 1)
      ;
}
#endif

/* Return nonzero if LOCK is locked.  */
inline int spinlock_locked(spinlock_t *lock) {
  return *lock != 0;
}
