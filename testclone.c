#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int func(void* arg) {
  int fd = *(int*)arg;
  char buf[128];
  int n = read(fd, buf, sizeof(buf));
  printf(1,"Child read %d bytes from file descriptor %d:\n%s\n", n, fd, buf);
  exit();
}

int test(void * arg){
  int tmp = *(int*)arg;
  printf(1,"test : %d\n", tmp);
  exit();
}

void testclone(void)
{
  int pid;
  char *stack=malloc(4096);
  int arg1=100;
  printf(1, "clone test\n");
  pid = clone(test,stack,&arg1,CLONE_FILES);
  if(pid == 0)
    exit();
  join(pid);
  free(stack);
  printf(1, "clone test OK\n\n");
}


void testCLONE_THREADS(void)
{
  int tid;
  char *stack=malloc(4096);
  int arg1=100;
  int flag1=0;
  int flag2=CLONE_THREAD;
  int ret_join;
  printf(1, "clone_thread test\n");
  printf(1, "clone_thread set\n");

  tid = clone(test,stack,&arg1,flag2);
  if(tid == 0)
    exit();
  ret_join=join(tid);
  printf(1, "tid = %d , return value of join=%d\n",tid,ret_join);

  printf(1, "clone_thread not et\n");
  tid = clone(test,stack,&arg1,flag1);
  if(tid == 0)
    exit();
  ret_join=join(tid);
  printf(1, "tid = %d , return value of join=%d\n",tid,ret_join);

  free(stack);
  printf(1, "clone_thread test OK\n");
}

void testCLONE_FILES(void){
  void *stack = malloc(4096);
  int flags = CLONE_FILES;

  int fd = open("clone_file.txt", O_CREATE | O_RDWR);
  char msg[128] = "Hello from parent!\n";
  printf(1, "clone_file set\n");
  write(fd, msg, strlen(msg));

  int pid =clone(func, stack , &fd, flags);
  if(pid == 0)
    exit();
  join(pid);

  printf(1, "clone_file not set\n");
  write(fd, msg, strlen(msg));
  pid =clone(func, stack , &fd, 0);
  if(pid == 0)
    exit();
  join(pid);
  free(stack);
  printf(1, "clone_file test OK\n\n");
}

int main(int argc, char *argv[]) {
  testclone();
  testCLONE_FILES();
  testCLONE_THREADS();
  exit();
}







