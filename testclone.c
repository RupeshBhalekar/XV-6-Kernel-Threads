#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int global_arg=10;
int getid;

struct data
{ int a;
  int b;
};


int func(void* arg) {
  int fd = *(int*)arg;
  char buf[128];
  int n = read(fd, buf, sizeof(buf));
  if(n==0){
      printf(1,"CLONE_FILES set and passed\n");
  }
  else if(n%19==0){
      printf(1,"CLONE_FILES not set and passed\n");
  }
  else{
      printf(1,"CLONE_FILES failed\n");
  }
  exit();
}

int test(void * arg){
  int tmp = *(int*)arg;
  if(tmp==100){
      printf(1,"Argument passed successfully\n");
  }
  exit();
}

int testmultiple(void * arg){
  struct data *tmp = (struct data*)arg;
  int x=tmp->a +tmp->b;
  if(x==300){
      printf(1,"Multiple Argument passed successfully\n");
  }
  exit();
}


int VMtest(void * arg){
  int tmp = *(int*)arg;
  global_arg++;
  tmp++;
  exit();
}

int get_tid(void * arg){
  getid=gettid();
  exit();
}

void test_gettid(void){
  int tid;
  char *stack=malloc(4096);
  int arg1=100;
  tid = clone(get_tid,stack,&arg1,CLONE_VM|CLONE_THREAD);
  if(tid == 0)
    exit();
  join(tid);
  if(getid==tid){
      printf(1, "Get_tid function passed successfully\n");
  }
  free(stack);
}

void test_tkill(void)
{
  int tid;
  char *stack=malloc(4096);
  tid = clone(test,stack,0,CLONE_THREAD);
  if(tid == 0)
    exit();
  int ret ;
  ret= tkill(tid);
  if (ret == 0) {
    printf(1, "tkill: process with TID %d killed\n", tid);
  } else if (ret == -1) {
    printf(1, "tkill: process with TID %d has already terminated\n", tid);
  } else {
    printf(1, "tkill: process with TID %d is group leader\n", tid);
  }
  free(stack);
}

void testCLONE_VM(void)
{
  int pid;
  char *stack=malloc(4096);
  int arg1=100;
  pid = clone(VMtest,stack,0, CLONE_VM);
  sleep(5);
  if(pid == 0)
    exit();
  join(pid);
  if(global_arg==11) {
    printf(1,"ClomeVM Flag set and Passed\n");
  }

  pid = clone(VMtest,stack,&arg1, 0);
  sleep(5);
  if(pid == 0)
    exit();
  join(pid);
  if(arg1==100) {
    printf(1,"ClomeVM Flag not set and Passed\n");
  }

  free(stack);
}

void testclone(void)
{
  int pid;
  char *stack=malloc(4096);
  int arg1=100;
  pid = clone(test,stack,&arg1,CLONE_FILES);
  if(pid == 0)
    exit();
  join(pid);
  free(stack);
}

void testclonemultiple(void)
{
  int pid;
  char *stack=malloc(4096);
  struct data *arg1=(struct data *)malloc(sizeof(struct data));
  arg1->a=100;
  arg1->b=200;
  pid = clone(testmultiple,stack,arg1,CLONE_VM|CLONE_FILES);
  if(pid == 0)
    exit();
  join(pid);
  free(stack);
}


void testCLONE_THREADS(void)
{
  int tid;
  char *stack=malloc(4096);
  int arg1=100;
  int flag1=0;
  int flag2=CLONE_THREAD;
  int ret_join;
  tid = clone(get_tid,stack,&arg1,flag2);
  if(tid == 0)
    exit();
  ret_join=join(tid);
  if(tid==ret_join){
      printf(1,"CLONE_THREAD set and Passed\n");
  }
  else{
      printf(1, "CLONE_THREAD set and failed\n");
  }

  tid = clone(get_tid,stack,&arg1,flag1);
  if(tid == 0)
    exit();
  ret_join=join(tid);
  if(ret_join==-1){
      printf(1,"CLONE_THREAD is not set and Passed\n");
  }
  else{
      printf(1, "CLONE_THREAD is not set and failed\n");
  }

  free(stack);
}

void testCLONE_FILES(void){
  void *stack = malloc(4096);
  int flags = CLONE_FILES;

  int fd = open("clone_file.txt", O_CREATE | O_RDWR);
  char msg[128] = "Hello from parent!\n";
  write(fd, msg, strlen(msg));

  int pid =clone(func, stack , &fd, flags);
  if(pid == 0)
    exit();
  join(pid);

  write(fd, msg, strlen(msg));
  pid =clone(func, stack , &fd, 0);
  if(pid == 0)
    exit();
  join(pid);
  free(stack);
}

int main(int argc, char *argv[]) {
  test_gettid();
  testclone();
  testclonemultiple();
  test_tkill();
  testCLONE_VM();
  testCLONE_FILES();
  testCLONE_THREADS();
  exit();
}







