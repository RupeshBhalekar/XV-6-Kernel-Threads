// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "stat.h"
#include "user.h"

#define N  1000


int test(void * arg){
  int tmp = *(int*)arg;
  printf(1,"test : %d\n", tmp);
  return 0;
}

void testclone(void)
{
  int pid;
  char *stack=malloc(4096);
  int arg1=100;
  printf(1, "clone test\n");

  pid = clone(test,stack,&arg1);
  if(pid == 0)
    exit();
  free(stack);
  printf(1, "clone test OK\n");
}

int
main(void)
{
  testclone();
  exit();
}