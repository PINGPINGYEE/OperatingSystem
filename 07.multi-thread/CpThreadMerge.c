#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define SIZE 10000000       // 배열의 크기를 정의
#define THREAD_COUNT 10     // 사용할 스레드의 수를 정의

double array[SIZE];         // 입력 데이터를 저장할 배열, 멀티 스레드에서 재사용
double single_thread_result[SIZE]; // 단일 스레드 결과를 저장할 배열

struct ThreadData {
    int start;               // 처리할 배열의 시작 인덱스
    int end;                 // 처리할 배열의 끝 인덱스
};

// 스레드가 실행할 함수로, 배열의 지정된 부분에 sin 함수를 적용
void *compute(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    for (int i = data->start; i < data->end; i++) {
        array[i] = sin(array[i]);
    }
    return NULL;
}

// 단일 스레드로 모든 계산을 수행하고, 실행 시간을 측정
struct timeval do_single_thread() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL); // 시작 시간을 측정
    
    for (int i = 0; i < SIZE; i++) {
        single_thread_result[i] = sin(array[i]); // 각 요소에 대해 sin 계산을 수행
    }
    
    gettimeofday(&end_time, NULL); // 종료 시간을 측정
    return (struct timeval){.tv_sec = end_time.tv_sec - start_time.tv_sec,
                            .tv_usec = end_time.tv_usec - start_time.tv_usec};
}

// 멀티 스레드로 계산을 수행하고, 실행 시간을 측정
struct timeval do_multi_thread() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL); // 시작 시간을 측정
    
    pthread_t threads[THREAD_COUNT];
    struct ThreadData data[THREAD_COUNT];
    
    // 배열을 스레드 수에 따라 나누고 각 스레드에 작업을 할당
    for (int i = 0; i < THREAD_COUNT; i++) {
        data[i].start = i * (SIZE / THREAD_COUNT);
        data[i].end = (i + 1) * (SIZE / THREAD_COUNT);
        pthread_create(&threads[i], NULL, compute, &data[i]);
    }
    
    // 모든 스레드가 작업을 완료할 때까지 대기
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    
    gettimeofday(&end_time, NULL); // 종료 시간을 측정
    return (struct timeval){.tv_sec = end_time.tv_sec - start_time.tv_sec,
                            .tv_usec = end_time.tv_usec - start_time.tv_usec};
}

// 단일 스레드와 멀티 스레드의 실행 시간 차이를 출력
void print_diff(struct timeval single_thread_time, struct timeval multi_thread_time) {
    printf("Single_thread 소요시간: %ld.%06ld seconds\n", single_thread_time.tv_sec, single_thread_time.tv_usec);
    printf("Multi_thread 소요시간: %ld.%06ld seconds\n", multi_thread_time.tv_sec, multi_thread_time.tv_usec);
    printf("속도 차이: %.2f times\n", (single_thread_time.tv_sec + single_thread_time.tv_usec / 1000000.0) /
                                      (multi_thread_time.tv_sec + multi_thread_time.tv_usec / 1000000.0));
}

// 단일 스레드와 멀티 스레드의 값 비교를 수행, 부동 소수점 비교 시 정밀도 문제를 고려
void verify_results() {
    for (int i = 0; i < SIZE; i++) {
        if (fabs(single_thread_result[i] - array[i]) > 1e-9) {
            printf("단일 스레드와 멀티 스레드의 값이 다름.\n");
            return;
        }
    }
    printf("단일 스레드와 멀티 스레드의 값이 같음.\n");
}

int main(int argc, char* argv[]) {
    // 배열을 초기화
    for (int i = 0; i < SIZE; i++) {
        array[i] = i * 0.1;
    }

    struct timeval single_thread_time = do_single_thread();
    // 단일 스레드 결과로부터 array에 다시 데이터를 채움
    for (int i = 0; i < SIZE; i++) {
        array[i] = i * 0.1; // 배열을 원래 값으로 재설정
    }
    struct timeval multi_thread_time = do_multi_thread();

    print_diff(single_thread_time, multi_thread_time);
    verify_results();

    return 0;
}
