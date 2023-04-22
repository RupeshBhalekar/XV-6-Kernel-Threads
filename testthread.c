#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "thread.h"

#define N 1000
#define K 10
#define MAX_TERMS 50

int fib_sequence[MAX_TERMS];
int arr[K];
int x = 20;
int found1= 0,found2= 0;
int c1=0,c2=0,c=0,run = 0;
struct spinlock lk;

int lock1(void *args){
    while(run==1){
        acquire(&lk);
        c++;
        release(&lk);
        c1++;
    }
    exit();
}
int lock2(void *args){
    while(run==1){
        acquire(&lk);
        c++;
        release(&lk);
        c2++;
    }
    exit();
}

int f(void *arg) {
    int tmp=*(int *)arg;
    tmp++;
    exit();
}

int f1(void *arg) {
    int tmp=*(int *)arg;
    tmp++;
    while(tmp){
        ;
    }
    exit();
}

int testThreadcreate(){
    int x = thread_create(f,0);
    thread_join(x);
    if(x<0){
        printf(1,"Thread create test failed\n");
    }
    else{
        printf(1,"Thread create test Passed\n");
    }
    return 0;
}

int testThreadjoin(){
    int ret;
    int x = thread_create(f,0);
    ret=thread_join(x);
    if(ret==x){
        printf(1,"Thread join test Passed\n");
    }
    else{
        printf(1,"Thread join test failed\n");
    }
    return 0;
}

int testThreadexit(){
    int ret;
    int x = thread_create(f1,0);
    ret=thread_exit(x);
    if(ret==0){
        printf(1,"Thread exit test Passed\n");
    }
    else{
        printf(1,"Thread exit test failed\n");
    }
    return 0;
}

int search_left_half(void *arg) {
  int y=*((int *)arg);
  int size = K;
  int r = size / 2 - 1;
  int l = 0;
  int i = l, j = r;
  while (i <= j) {
    int mid = (i + j) / 2;
    if (arr[mid] == y) {
      found1 = 1;
      exit();
    } else if (arr[mid] > y) {
      j = mid - 1;
    } else {
      i = mid + 1;
    }
  }
  exit();
}

int search_right_half(void *arg) {
  int y=*((int *)arg);
  int size = K;
  int l = size / 2;
  int r = size - 1;
  int i = l, j = r;
  while (i <= j) {
    int mid = (i + j) / 2;
    if (arr[mid] == y) {
      found2 = 1;
      exit();
    } else if (arr[mid] > y) {
      j = mid - 1;
    } else {
      i = mid + 1;
    }
  }
  exit();
}

void testLock(){
    initlock(&lk);
    int tid1=thread_create(lock1, 0);
    int tid2=thread_create(lock2, 0);

    sleep(2);
    run = 0;
    join(tid1);
    join(tid2);
    if(c == c1+c2){
        printf(1,"Lock test Passed\n");

    }else{
        printf(1,"Lock test Failed\n");
    }
}

void test_thread(void *arg) {
  for (int i = 0; i < K; i++) {
    arr[i]=i;
  }
  int tid=thread_create(search_left_half, arg);
  thread_join(tid);
  tid=thread_create(search_right_half, arg);
  thread_join(tid);
  if(found1 || found2){
     printf(1, "Binary Search test Passed\n");
  }else{
      printf(1, "Binary Search test Failed\n");
  }
}
int fib(int n) {
  if (n == 0) {
    return 0;
  } else if (n == 1) {
    return 1;
  } else {
    return fib(n-1) + fib(n-2);
  }
  exit();
}

int fib_thread(void *arg) {
  int num_terms = *((int *) arg);
  for (int i = 0; i < num_terms; i++) {
    fib_sequence[i] = fib(i);
  }
  exit();
}

void test_fibonacci_threading(int num_terms) {
  int tid=thread_create(fib_thread, &num_terms);
  thread_join(tid);

  if(fib_sequence[num_terms-1]==34){
      printf(1, "Fibonacci sequence test Passed\n");
  }
  else{
       printf(1, "Fibonacci sequence test failed\n");
  }

}

int main(){
    int p=3;
    testThreadcreate();
    testThreadjoin();
    testThreadexit();
    test_fibonacci_threading(10);
    test_thread(&p);
    testLock();
    exit();
}