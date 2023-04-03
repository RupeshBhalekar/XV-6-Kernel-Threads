// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "stat.h"
#include "user.h"

#define N  1000


int test(){
  printf(1,"test\n");
  return 0;
}

void testclone(void)
{
  int pid;
  char *stack=malloc(4096);
  int tmp=100;
  printf(1, "clone test\n");
  pid = clone(test,stack,&tmp);
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