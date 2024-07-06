#include<iostream>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
#include<unistd.h>
#include<termios.h>

int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF){
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}


int main(){
	printf("program has started working\n");
	sem_t *sem;
	FILE *text;
	const char name[] = "file.txt";
	
	sem = sem_open("/semaphore", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED){
		perror("sem_open");
		return 1;
	}
		
	if((text = fopen(name, "a")) == NULL){
		perror("fopen");
		return 1;
	}
	else {
		while(1){
			sem_wait(sem);
			for(int i = 0; i < 10; i++){
				fputc('2', text);
				printf("2");
				fflush(stdout);
				sleep(1);
			}
			sem_post(sem);
			sleep(1);
			if(kbhit()) break;
		}	
	}
	
	fclose(text);
	sem_close(sem);
	sem_unlink("semaphore");
	printf("\nprogram has finished working\n");
}
