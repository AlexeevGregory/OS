#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
#include<time.h>
#include<fcntl.h>

using namespace std;

int pipedes[2];

typedef struct{
	clock_t time;
	int flag;
}targs;


void *proc1(void *arg){
	printf("\nthread 1 has started working\n");
	char buffer[100];
	ssize_t rv;
	targs *args = (targs*) arg;
	
	while(args->flag == 0){
		
		args->time = clock();
		rv = write(pipedes[1], &args->time, sizeof(args->time));
		
		if(rv > 0){
			printf("time = %ld\n", args->time);
		}
		if(rv == -1){
			perror("read");
			exit(EXIT_FAILURE);
		}
		if(rv == 0){
			printf("\nfile is empty\n");
		}
		sleep(1);	
	}
	
	printf("\nthread 1 has finished working\n");
	pthread_exit((void*)2);
}
void *proc2(void *arg){
	printf("\nthread 2 has started working\n");
	char buffer[100];
	ssize_t rv;
	targs *args = (targs*) arg;
	
	while(args->flag == 0){
	
		args->time = clock();
		rv = read(pipedes[0], &args->time, sizeof(args->time));
		
		if(rv > 0){
			printf("time = %ld\n", args->time);
		}
		if(rv == -1){
			perror("read");
			exit(EXIT_FAILURE);
		}
		if(rv == 0){
			printf("\nfile is empty\n");
		}
		sleep(1);
	}
	
	printf("\nthread 2 has finished working\n");
	pthread_exit((void*)2);
}



int main(){
	printf("program has started working\n");
	targs arg1, arg2;
	arg1.flag = 0; 
	arg2.flag = 0;
	int *exitcode;
	pthread_t id1, id2;
	pipe2(pipedes, O_NONBLOCK);
	
	
	pthread_create(&id1, NULL, proc1, &arg1);
	pthread_create(&id2, NULL, proc2, &arg2);
	printf("\nthe program is waiting to key pressed\n");
	getchar();
	printf("\nthe key is pressed\n");
	
	arg1.flag = 1; 
	arg2.flag = 1;
	
	pthread_join(id1, (void**)&exitcode);
	pthread_join(id2, (void**)&exitcode);
	
	
	close(pipedes[0]); 
	close(pipedes[1]);
	printf("program has finished working\n");
}
