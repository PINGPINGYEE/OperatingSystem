#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define SIZE 1000000       // 배열의 크기를 정의합니다.
#define THREAD_COUNT 10     // 사용할 스레드의 수를 정의합니다.

double array[SIZE];         // 입력 데이터를 저장할 배열입니다. 멀티 스레드에서 재사용됩니다.
double single_thread_result[SIZE]; // 단일 스레드 결과를 저장할 배열입니다.
double multi_thread_result[SIZE];  // 멀티 스레드 결과를 저장할 배열입니다.

struct ThreadData {
    int start;               // 처리할 배열의 시작 인덱스
    int end;                 // 처리할 배열의 끝 인덱스
};

// 스레드가 실행할 함수로, 배열의 지정된 부분에 sin 함수를 적용합니다.
void *compute(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    for (int i = data->start; i < data->end; i++) {
        multi_thread_result[i] = sin(array[i]);
    }
    return NULL;
}

// 단일 스레드로 모든 계산을 수행하고, 실행 시간을 측정합니다.
struct timeval do_single_thread() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL); // 시작 시간을 측정합니다.
    
    for (int i = 0; i < SIZE; i++) {
        single_thread_result[i] = sin(array[i]); // 각 요소에 대해 sin 계산을 수행합니다.
    }
    
    gettimeofday(&end_time, NULL); // 종료 시간을 측정합니다.
    return (struct timeval){.tv_sec = end_time.tv_sec - start_time.tv_sec,
                            .tv_usec = end_time.tv_usec - start_time.tv_usec};
}

// 멀티 스레드로 계산을 수행하고, 실행 시간을 측정합니다.
struct timeval do_multi_thread() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL); // 시작 시간을 측정합니다.
    
    pthread_t threads[THREAD_COUNT];
    struct ThreadData data[THREAD_COUNT];
    
    // 배열을 스레드 수에 따라 나누고 각 스레드에 작업을 할당합니다.
    for (int i = 0; i < THREAD_COUNT; i++) {
        data[i].start = i * (SIZE / THREAD_COUNT);
        data[i].end = (i + 1) * (SIZE / THREAD_COUNT);
        pthread_create(&threads[i], NULL, compute, &data[i]);
    }
    
    // 모든 스레드가 작업을 완료할 때까지 대기합니다.
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    
    gettimeofday(&end_time, NULL); // 종료 시간을 측정합니다.
    return (struct timeval){.tv_sec = end_time.tv_sec - start_time.tv_sec,
                            .tv_usec = end_time.tv_usec - start_time.tv_usec};
}

// 단일 스레드와 멀티 스레드의 실행 시간 차이를 출력합니다.
void print_diff(struct timeval single_thread_time, struct timeval multi_thread_time) {
    printf("Single thread: %ld.%06ld seconds\n", single_thread_time.tv_sec, single_thread_time.tv_usec);
    printf("Multi thread: %ld.%06ld seconds\n", multi_thread_time.tv_sec, multi_thread_time.tv_usec);
    printf("Speedup: %.2f times\n", (single_thread_time.tv_sec + single_thread_time.tv_usec / 1000000.0) /
                                      (multi_thread_time.tv_sec + multi_thread_time.tv_usec / 1000000.0));
}

// 단일 스레드와 멀티 스레드의 값 비교를 수행합니다. 부동 소수점 비교 시 정밀도 문제를 고려합니다.
void verify_results() {
    for (int i = 0; i < SIZE; i++) {
        if (fabs(single_thread_result[i] - multi_thread_result[i]) > 1e-9) {
            printf("Results differ.\n");
            return;
        }
    }
    printf("Results are identical.\n");
}

int main(int argc, char* argv[]) {
    // 배열을 초기화합니다.
    for (int i = 0; i < SIZE; i++) {
        array[i] = i * 0.1;
    }

    struct timeval single_thread_time = do_single_thread();
    struct timeval multi_thread_time = do_multi_thread();

    print_diff(single_thread_time, multi_thread_time);
    verify_results();

    return 0;
}
