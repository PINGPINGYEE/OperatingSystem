#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int choice;
    int seconds;

    while(1) {
				printf("--------Signal Controller--------\n");
        printf("1 : send SIGTERM\n2 : set SIGALRM\n3 : EXIT\n");
				printf("Input : ");
        scanf("%d", &choice);

        if (choice == 1) {
            system("./signal_pipe.sh SIGTERM");
						printf("Exit the program");
						break;
        } else if (choice == 2) {
            printf("Enter the number of seconds to send SIGALRM : ");
            scanf("%d", &seconds);
            char command[50];
            sprintf(command, "./signal_pipe.sh SIGALRM %d", seconds);
            system(command);
        } else if (choice == 3) {
						printf("Exit the program");
						break;
				}	else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}

