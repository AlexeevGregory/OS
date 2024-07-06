#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
	
    	
	printf("parent process: %d\n", getpid());
	
	
    	pid_t pid = fork();
    
    	if (pid == -1) {
        	perror("fork");
        	exit(1);
    	}
    	
    	else if(pid == 0){
		
		
		
		char arg[] = "./prog1";
        	char *args[] = {arg, argv[1], argv[2], argv[3], argv[4], NULL};
        	
        	char val[] = "User";
       		char *env[] = {val, NULL};
       		
        	execve("./prog1", args, env);
    	}
    	
    	else if (pid > 0) {
        	
        	printf("child process: %d\n", pid);
        	int status;
        	
		while(waitpid(pid, &status, WNOHANG) == 0){
			printf("\nwaiting\n");
			sleep(1);
		}
        	printf("status of child process: %d\n", WEXITSTATUS(status));
    	}
    	
    	return 0;
}
