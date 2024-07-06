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
long msgtype = 0;
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
	TMessage *args = (TMessage*)arg;
	
	while (flag == 0) {
		msgrcv(msgid,  args, sizeof(args->buffer), 1, IPC_NOWAIT | MSG_NOERROR);
		printf("buffer = %s\n", args->buffer);
		sleep(1);
	}
	printf("\nthread has finished working\n");
	pthread_exit(NULL);
}

int main() {
	signal(SIGINT, sig_handler);
	printf("program has started working\n");
	pthread_t id;
	TMessage read_msg;
	
	memset(read_msg.buffer, 0, sizeof(read_msg.buffer));
	
	key_t key = ftok("filename",'A');
	msgid = msgget(key,0);
	if(msgid < 0) msgid = msgget(key, 0666);

	pthread_create(&id, NULL, proc, &read_msg);

	printf("\nthe program is waiting for a key to be pressed\n");
	getchar();
	printf("\nthe key is pressed\n");

	flag = 1;

	pthread_join(id, NULL);

	msgctl(msgid, IPC_RMID, NULL);
	printf("program has finished working\n");
	return 0;
}
