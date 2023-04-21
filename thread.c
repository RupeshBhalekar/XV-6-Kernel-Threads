#include "types.h"
#include "fcntl.h"
#include "user.h"
#include "thread.h"

int thread_create(thread_t *t,int (*fn) (void *),void* arg){
    t->stack = malloc(4096);
    t->tid = clone(fn,t->stack,arg,CLONE_THREAD);
    printf(1,"Thread id:%d\n",t->tid);
    return 0;
}

int thread_join(thread_t * t){
    int tid1 = join(t->tid);
    return tid1;
}