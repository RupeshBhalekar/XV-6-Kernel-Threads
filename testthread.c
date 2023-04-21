#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "thread.h"

int x = 20;

int f(void *arg) {
	int p = *(int *)arg;
	printf(1,"%d x= %d\n", p, x);
    exit();
}

int test_thread_create(){
    thread_t t;
    int R = 6;
    int x = thread_create(&t,f,&R);
    thread_join(&t);
    if(x<0){
        printf(1,"Not ok");
    }
    else{
        printf(1,"Thread test ok");
    }
    return 0;
}

int main(){
    test_thread_create();
    exit();
}