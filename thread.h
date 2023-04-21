typedef struct thread_t{
    int tid;
    char * stack;
}thread_t;

int thread_create(thread_t *t,int (*fn) (void *),void* arg);
int thread_join(thread_t*t);