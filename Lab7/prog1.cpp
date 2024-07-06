#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>


int flag = 0; int msgid = -1;
typedef struct {
	long mtype;
	char buffer[256];
} TMessage;




void sig_handler(int signo) {
	printf("\nget SIGINT; %d\n", signo);

	msgctl(msgid, IPC_RMID, NULL);
	exit(0);
}

void *proc(void *arg) {
	printf("\nthread has started working\n");
	clock_t time;
	TMessage *args = (TMessage*)arg;
	
	while (flag == 0) {
		time = clock();
		sprintf(args->buffer, "%ld", time);
		printf("time: %ld\n", time);
		msgsnd(msgid,  args, sizeof(args->buffer), IPC_NOWAIT);
		sleep(1);
	}
	printf("\nthread has finished working\n");
	pthread_exit(NULL);
}

int main() {
	signal(SIGINT, sig_handler);
	printf("program has started working\n");
	pthread_t id;
	TMessage msg;
	
	key_t key = ftok("filename",'A');
	msgid = msgget(key,0);
	if(msgid < 0) msgid = msgget(key, IPC_CREAT | 0666);
	msg.mtype = 1;

	pthread_create(&id, NULL, proc, &msg);

	printf("\nthe program is waiting for a key to be pressed\n");
	getchar();
	printf("\nthe key is pressed\n");

	flag = 1;

	pthread_join(id, NULL);

	msgctl(msgid, IPC_RMID, NULL);
	printf("program has finished working\n");
	return 0;
}
