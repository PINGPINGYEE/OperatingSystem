#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

// 시그널 핸들러 함수
void signal_handler(int sig) {
    char response[10]; // 사용자 응답을 저장할 문자 배열

    // 받은 시그널에 따라 적절한 작업 수행
    switch(sig) {
        case SIGINT: // SIGINT 시그널을 받은 경우 (Ctrl+C)
            printf("Received SIGINT. Do you want to exit? (yes/no): ");
            scanf("%s", response); // 사용자로부터 응답 받기
            if (strcmp(response, "yes") == 0) { // "yes"라고 답하면 프로그램 종료
                printf("Exiting...\n");
                exit(0);
            } else { // "no"라고 답하면 계속 실행
                printf("Continuing...\n");
            }
            break;
        case SIGTERM: // SIGTERM 시그널을 받은 경우
            printf("Received SIGTERM. Shutting down the program...\n");
            exit(0); // 바로 프로그램 종료
            break;
        case SIGALRM: // SIGALRM 시그널을 받은 경우 (알람 시그널)
            printf("Received SIGALRM. Doing next task...\n");
            // 필요한 경우 여기에 추가 작업 코드를 넣을 수 있습니다.
            break;
        default: // 예상치 못한 다른 시그널을 받은 경우
            printf("Received unknown signal %d\n", sig);
            break;
    }
}

int main() {
    struct sigaction sa; // 시그널을 처리하기 위한 구조체 선언
    sa.sa_handler = signal_handler; // 시그널이 오면 실행할 함수 지정
    sigemptyset(&sa.sa_mask); // 다른 시그널이 핸들러 실행 중 방해되지 않도록 모든 시그널을 시그널 마스크에서 제거
    sa.sa_flags = 0; // 추가적인 옵션 사용하지 않음

    // SIGINT, SIGTERM, SIGALRM에 대해 시그널 핸들러 설정
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);

    // 무한 루프를 돌면서 프로그램이 계속 실행되도록 함
    while (1) {
        printf("Program is running...\n");
        sleep(1); // 1초 동안 대기
    }
    return 0; // 프로그램이 정상적으로 종료되면 0을 반환
}

