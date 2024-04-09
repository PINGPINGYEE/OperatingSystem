#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid; // Declare a variable to store the process ID

  for(int i = 0; i < 2; i++) {

    pid = fork(); // Create a new process.

    if (pid < 0) { // Check if fork failed to create a new process
      fprintf(stderr, "Fork Failed");
      return 1; // And exit with an error code
    }
    else {
      wait(NULL); // Parent will wait for the child to complete
      printf("Hello\n");
    }
  }

  return 0;
}

