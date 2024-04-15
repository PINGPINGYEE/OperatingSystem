#include <fcntl.h>    // 파일 제어 함수를 위한 헤더 파일
#include <stdio.h>    // 표준 입출력 함수를 위한 헤더 파일
#include <stdlib.h>   // 일반 유틸리티 함수를 위한 헤더 파일
#include <string.h>   // 문자열 처리 함수를 위한 헤더 파일
#include <sys/stat.h> // 파일 상태 정보를 위한 헤더 파일
#include <unistd.h>   // POSIX 운영 체제 API를 위한 헤더 파일

#define FIFO_A_TO_B "fifo_a_to_b"  // A에서 B로 데이터를 보낼 FIFO 파일의 이름
#define FIFO_B_TO_A "fifo_b_to_a"  // B에서 A로 데이터를 받을 FIFO 파일의 이름

int main() {
    int fd;              // 파일 디스크립터를 저장할 변수
    char readbuf[80];    // 데이터 수신을 위한 버퍼
    char input[80];      // 데이터 송신을 위한 입력 버퍼

    // FIFO 파일 생성
    mkfifo(FIFO_A_TO_B, S_IFIFO|0666); // A에서 B로의 통신을 위한 FIFO 파일 생성
    mkfifo(FIFO_B_TO_A, S_IFIFO|0666); // B에서 A로의 통신을 위한 FIFO 파일 생성

    while (1) {
        // 입력 받기
        printf("입력대기: ");
        fgets(input, sizeof(input), stdin); // 사용자 입력 받기
        input[strcspn(input, "\n")] = 0;    // 입력된 문자열에서 개행 문자 제거

        fd = open(FIFO_A_TO_B, O_WRONLY);   // 쓰기 전용으로 FIFO 파일 열기
        write(fd, input, strlen(input)+1);  // 입력받은 데이터를 FIFO를 통해 전송
        close(fd);                          // 파일 디스크립터 닫기

        if (strcmp(input, "exit") == 0)     // 입력받은 데이터가 exit이면 종료
            break;

        // 수신 대기
        printf("수신대기...\n");
        fd = open(FIFO_B_TO_A, O_RDONLY);   // 읽기 전용으로 FIFO 파일 열기
        read(fd, readbuf, sizeof(readbuf)); // FIFO를 통해 데이터 수신
        printf("A) %s\n\n", readbuf);       // 수신된 데이터 출력
        close(fd);                          // 파일 디스크립터 닫기

        if (strcmp(readbuf, "exit") == 0)   // 수신된 데이터가 exit이면 종료
            break;
    }

    unlink(FIFO_A_TO_B); // FIFO 파일 삭제
    unlink(FIFO_B_TO_A); // FIFO 파일 삭제
    return 0;            // 프로그램 종료
}

