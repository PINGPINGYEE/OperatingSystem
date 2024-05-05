#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strcmp 함수를 사용하기 위해 추가
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig) {
    char response[10];
    switch(sig) {
        case SIGINT:
            printf("Received SIGINT. Do you want to exit? (yes/no): ");
            scanf("%s", response);
            if (strcmp(response, "yes") == 0) {
                printf("Exiting...\n");
                exit(0);
            } else {
                printf("Continuing...\n");
            }
            break;
        case SIGTERM:
            printf("Received SIGTERM. Shutting down the program...\n");
            exit(0);
            break;
        case SIGALRM:
            printf("Received SIGALRM. Doing next task...\n");
            break;
        default:
            printf("Received unknown signal %d\n", sig);
            break;
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);

    while (1) {
        printf("Program is running...\n");
        sleep(1);
    }
    return 0;
}

