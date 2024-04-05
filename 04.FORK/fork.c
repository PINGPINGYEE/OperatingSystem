#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	pid_t pid;
	
	for(int i = 0; i < 2; i++) {
	
		fork();
	
		if (pid < 0) {
			fprintf(stderr, "Fork Failed");
			return 1;
		}
		else {
			wait(NULL);
			printf("Hello\n");	
		}
	}

	return 0;
}
