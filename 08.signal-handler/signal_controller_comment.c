#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int choice;  // 사용자의 선택을 저장할 변수
    int seconds; // 알람 시간을 저장할 변수

    while(1) {
        // 사용자 인터페이스 출력
        printf("--------Signal Controller--------\n");
        printf("1 : send SIGTERM\n2 : set SIGALRM\n3 : EXIT\n");
        printf("Input : ");
        scanf("%d", &choice); // 사용자로부터 선택 입력 받기

        // 선택에 따라 조건 처리
        if (choice == 1) {
            // SIGTERM 시그널 보내기
            system("./signal_pipe_comment.sh SIGTERM");
            printf("Exit the program\n"); // 프로그램 종료 메시지 출력
            break; // 반복문 종료
        } else if (choice == 2) {
            // SIGALRM 시간 설정
            printf("Enter the number of seconds to send SIGALRM : ");
            scanf("%d", &seconds);
            char command[50]; // 추후 시스템 함수에 전달되어 실제 명령어를 실행 할 시스템 명령을 저장할 문자열
            sprintf(command, "./signal_pipe_comment.sh SIGALRM %d", seconds); // 명령 문자열 생성
            system(command); // 시그널 전송 명령 실행
        } else if (choice == 3) {
            // 프로그램 종료 선택
            printf("Exit the program\n"); // 종료 메시지 출력
            break; // 반복문 종료
        } else {
            // 잘못된 입력 처리
            printf("Invalid choice.\n"); // 잘못된 입력 메시지 출력
        }
    }
    return 0; // 프로그램 종료
}

