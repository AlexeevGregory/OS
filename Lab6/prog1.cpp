#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

typedef struct {
	clock_t time;
	int flag;
} targs;

int size = 256;
sem_t *sem_read, *sem_write;
int shm_fd;
void *ptr;

void sig_handler(int signo) {
	printf("\nget SIGINT; %d\n", signo);

	sem_unlink("/read_semaphore");
	sem_unlink("/write_semaphore");

	munmap(ptr, size);
	shm_unlink("/shared_memory");
	exit(0);
}

void *proc(void *arg) {
	printf("\nthread has started working\n");
	char buffer[256];
	targs *args = (targs*)arg;

	while (args->flag == 0) {
		args->time = clock();
		sprintf(buffer, "%ld", args->time);
		printf("time: %ld\n", args->time);
		memcpy(ptr, buffer, strlen(buffer) + 1);
		sem_post(sem_write);
		sem_wait(sem_read);
		sleep(1);
	}
	printf("\nthread has finished working\n");
	pthread_exit((void*)2);
}

int main() {
	signal(SIGINT, sig_handler);
	printf("program has started working\n");

	pthread_t id;
	targs *arg = (targs*)malloc(sizeof(targs));
	arg->flag = 0;

	shm_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0644);
	if (shm_fd == -1) {
		perror("shm_open");
		return 1;
	}

	if (ftruncate(shm_fd, size) == -1) {
		perror("ftruncate");
		return 1;
	}

	ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	sem_write = sem_open("/write_semaphore", O_CREAT | O_RDWR, 0644, 0);
	sem_read = sem_open("/read_semaphore", O_CREAT | O_RDWR, 0644, 0);
	if ((sem_write == SEM_FAILED) || (sem_read == SEM_FAILED)) {
		perror("sem_open");
		return 1;
	}

	pthread_create(&id, NULL, proc, arg);

	printf("\nthe program is waiting for a key to be pressed\n");
	getchar();
	printf("\nthe key is pressed\n");

	arg->flag = 1;

	pthread_join(id, NULL);

	sem_close(sem_read);
	sem_unlink("/read_semaphore");
	sem_close(sem_write);
	sem_unlink("/write_semaphore");

	munmap(ptr, size);
	close(shm_fd);
	shm_unlink("/shared_memory");

	free(arg);
	printf("program has finished working\n");
	return 0;
}
