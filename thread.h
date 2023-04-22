typedef struct thread_t{
    int tid;
    char * stack;
}thread_t;

struct spinlock{
    uint lock;
};

void initlock(struct spinlock *lk);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);
int thread_create(int (*fn) (void *),void* arg);
int thread_join(int tid);
int thread_exit(int tid);