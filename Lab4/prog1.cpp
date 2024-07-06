#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char *argv[]) {

	printf("current process: %d\n", getpid());
	printf("parent process: %d\n", getppid());
   
	for (int i = 1; i < argc; i++) {
		printf("argument %d: %s\n", i, argv[i]);
		sleep(1);
	}
	for (char **env = environ; *env != NULL; ++env) {
		printf("enviroment parametr: %s\n", *env);
		sleep(1);
	}

	return 5;
}
