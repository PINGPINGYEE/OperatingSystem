#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_A_TO_B "fifo_a_to_b"
#define FIFO_B_TO_A "fifo_b_to_a"

int main() {
    int fd;
    char readbuf[80];
    char input[80];

    // FIFO 파일 생성
    mkfifo(FIFO_A_TO_B, S_IFIFO|0666);
    mkfifo(FIFO_B_TO_A, S_IFIFO|0666);

    while (1) {
        // 입력 받기
        printf("입력대기: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // 개행 문자 제거

        fd = open(FIFO_A_TO_B, O_WRONLY);
        write(fd, input, strlen(input)+1);
        close(fd);

        if (strcmp(input, "exit") == 0)
            break;

        // 수신 대기
        printf("수신대기...\n");
        fd = open(FIFO_B_TO_A, O_RDONLY);
        read(fd, readbuf, sizeof(readbuf));
        printf("A) %s\n\n", readbuf);
        close(fd);

        if (strcmp(readbuf, "exit") == 0)
            break;
    }
    unlink(FIFO_A_TO_B);
    unlink(FIFO_B_TO_A);
    return 0;
}
