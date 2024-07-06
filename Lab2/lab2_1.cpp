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
		for(int i = 0; i < 10; i++){
			printf("%c", args->sym);
			fflush(stdout);
			sleep(1);
		}
		sleep(1);
	}
	
	
	printf("\nthread 1 has finished working\n");
	pthread_exit((void*)2);
}
void *proc2(void *arg){
	printf("\nthread 2 has started working\n");
	targs *args = (targs*) arg;
	
	while(args->flag == 0){
		for(int i = 0; i < 10; i++){
			printf("%c", args->sym);
			fflush(stdout);
			sleep(1);
		}
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
	pthread_t id1, id2;
	
	pthread_create(&id1, NULL, proc1, &arg1);
	pthread_create(&id2, NULL, proc2, &arg2);
	printf("\nthe program is waiting to key pressed\n");
	getchar();
	printf("\nthe key is pressed\n");
	
	arg1.flag = 1; 
	arg2.flag = 1;
	
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	
	
	printf("program has finished working\n");
}

