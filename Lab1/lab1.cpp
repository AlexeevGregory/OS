#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>

using namespace std;

typedef struct{
	int flag;
	char sym;
}targs;

void *proc1(void *arg){
	printf("\nthread 1 has started working\n");
	targs *args = (targs*) arg;
	
	while(args->flag == 0){
		printf("%c", args->sym);
		fflush(stdout);
		sleep(1);
	}
	printf("\nthread 1 has finished working\n");
	pthread_exit((void*)2);
}
void *proc2(void *arg){
	printf("\nthread 2 has started working\n");
	targs *args = (targs*) arg;
	
	while(args->flag == 0){
		printf("%c", args->sym);
		fflush(stdout);
		sleep(1);
	}
	printf("\nthread 2 has finished working\n");
	pthread_exit((void*)2);
}


int main(){
	printf("program has started working\n");
	targs arg1, arg2;
	arg1.flag = 0; arg1.sym = '1';
	arg2.flag = 0; arg2.sym = '2';
	int *exitcode;
	pthread_t id1, id2;
	
	pthread_create(&id1, NULL, proc1, &arg1);
	pthread_create(&id2, NULL, proc2, &arg2);
	printf("\nthe program is waiting to key pressed\n");
	getchar();
	printf("\nthe key is pressed\n");
	
	arg1.flag = 1; 
	arg2.flag = 1;
	
	pthread_join(id1, (void**)&exitcode);
	printf("exitcode = %p\n", exitcode);
	pthread_join(id2, (void**)&exitcode);
	printf("exitcode = %p\n", exitcode);
	
	
	int detachstate;
	size_t stacksize; 
	int inheritsched; 
	int scope; 
	int schedpolicy; 
	struct sched_param param;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_getdetachstate(&attr, &detachstate); 	
	pthread_attr_getstacksize(&attr, &stacksize); 	
	pthread_attr_getinheritsched(&attr, &inheritsched); 
	pthread_attr_getscope(&attr, &scope); 
	pthread_attr_getschedpolicy(&attr, &schedpolicy); 
	pthread_attr_getschedparam(&attr, &param); 
	
	printf("\nDefault thread attributes:\n"); 
	printf("Detach state: %d\n", detachstate); 
	printf("Stack size: %ld\n", stacksize); 
	printf("Inherit scheduler: %d\n", inheritsched); 
	printf("Scope: %d\n", scope); 
	printf("Scheduling policy: %d\n", schedpolicy); 
	printf("Priority: %d\n", param.sched_priority);
	pthread_attr_destroy(&attr);
	
	printf("program has finished working\n");
}

