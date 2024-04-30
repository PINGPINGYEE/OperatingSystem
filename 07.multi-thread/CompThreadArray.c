#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define ARRAY_SIZE 10000000
#define NUM_THREADS 8

int array[ARRAY_SIZE];

typedef struct {
    int start;
    int end;
} ThreadData;

void merge(int start, int mid, int end);
void merge_sort(int start, int end);
void *thread_merge_sort(void *arg);
struct timeval time_diff(struct timeval start, struct timeval end);

// 배열을 무작위 값으로 초기화하는 함수
void initialize_array(int *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % size;
    }
}

// 두 시간 차이를 초와 마이크로초로 계산하는 함수
struct timeval time_diff(struct timeval start, struct timeval end) {
    struct timeval diff;
    diff.tv_sec = end.tv_sec - start.tv_sec;
    diff.tv_usec = end.tv_usec - start.tv_usec;
    if (diff.tv_usec < 0) {
        diff.tv_sec -= 1;
        diff.tv_usec += 1000000;
    }
    return diff;
}

void merge_sort(int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        merge_sort(start, mid);
        merge_sort(mid + 1, end);
        merge(start, mid, end);
    }
}

void merge(int start, int mid, int end) {
    int *temp = malloc(sizeof(int) * (end - start + 1));
    int i = start, j = mid + 1, k = 0;

    while (i <= mid && j <= end) {
        temp[k++] = (array[i] <= array[j]) ? array[i++] : array[j++];
    }
    while (i <= mid) {
        temp[k++] = array[i++];
    }
    while (j <= end) {
        temp[k++] = array[j++];
    }
    for (i = start, k = 0; i <= end; i++, k++) {
        array[i] = temp[k];
    }

    free(temp);
}

void *thread_merge_sort(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    merge_sort(data->start, data->end);
    return NULL;
}

void print_diff(struct timeval single_time, struct timeval multi_time) {
    printf("Single-thread Time: %ld.%06ld seconds\n", single_time.tv_sec, single_time.tv_usec);
    printf("Multi-thread Time: %ld.%06ld seconds\n", multi_time.tv_sec, multi_time.tv_usec);
}

int main() {
    struct timeval start_time, end_time, single_time, multi_time;

    initialize_array(array, ARRAY_SIZE); // 배열 무작위 값으로 초기화

    // 싱글 스레드 정렬
    gettimeofday(&start_time, NULL);
    merge_sort(0, ARRAY_SIZE - 1);
    gettimeofday(&end_time, NULL);
    single_time = time_diff(start_time, end_time);

    // 멀티 스레드 정렬
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int segment_size = ARRAY_SIZE / NUM_THREADS;

    gettimeofday(&start_time, NULL);
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * segment_size;
        thread_data[i].end = (i + 1) * segment_size - 1;
        if (i == NUM_THREADS - 1) thread_data[i].end = ARRAY_SIZE - 1; // 마지막 세그먼트 조정
        pthread_create(&threads[i], NULL, thread_merge_sort, &thread_data[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end_time, NULL);
    multi_time = time_diff(start_time, end_time);

    print_diff(single_time, multi_time);

    return 0;
}

