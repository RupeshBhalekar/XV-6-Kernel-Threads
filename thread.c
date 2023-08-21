#include "types.h"
#include "fcntl.h"
#include "user.h"
#include "thread.h"

void initlock(struct spinlock *lk){
  lk->lock = 0;
}

static uint xchg(volatile uint *addr, uint newval){
  uint result;
  asm volatile("lock; xchgl %0, %1"
               : "+m"(*addr), "=a"(result)
               : "1"(newval)
               : "cc");
  return result;
}

void acquire(struct spinlock *lk){
  while(xchg(&(lk->lock), 1) != 0)
    ;
  __sync_synchronize();
}

void release(struct spinlock *lk){
  __sync_synchronize();
  asm volatile("movl $0, %0" : "+m" (lk->lock) : );
}

int thread_create(int (*fn) (void *),void* arg){
    thread_t *t=(thread_t*)malloc(sizeof(thread_t));
    t->stack = (char *)malloc(4096);
    t->tid = clone(fn,t->stack,arg,CLONE_VM | CLONE_THREAD);
    return t->tid;
}

int thread_join(int tid){
    int tid1 = join(tid);
    return tid1;
}

int thread_exit(int tid){
    int tid1 = tkill(tid);
    return tid1;
}