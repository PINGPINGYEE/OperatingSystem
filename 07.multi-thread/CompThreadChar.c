#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define NUM_THREADS 12  // CPU 코어 수에 맞춰 스레드 수 조정
#define STRING_LENGTH 1200000000  // 문자열 길이 증가

char string[STRING_LENGTH];
int global_count[NUM_THREADS] = {0};

void* count_characters(void* threadarg) {
    int thread_index = *(int*)threadarg;
    int start = thread_index * (STRING_LENGTH / NUM_THREADS);
    int end = (thread_index == NUM_THREADS - 1) ? STRING_LENGTH : start + (STRING_LENGTH / NUM_THREADS);
    char target = 'a';  // 빈도를 세고자 하는 문자

    for (int i = start; i < end; i++) {
        if (string[i] == target) {
            global_count[thread_index]++;
        }
    }
    pthread_exit(NULL);
}

// 싱글 스레드, 멀티 스레드 함수 및 메인 함수는 동일하게 유지

int single_thread_count() {
    char target = 'a';
    int count = 0;
    for (int i = 0; i < STRING_LENGTH; i++) {
        if (string[i] == target) {
            count++;
        }
    }
    return count;
}

struct timeval do_single_thread() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    int count = single_thread_count();
    gettimeofday(&end_time, NULL);
    printf("Single Thread: Total occurrences of 'a': %d\n", count);
    struct timeval elapsed_time;
    timersub(&end_time, &start_time, &elapsed_time);
    return elapsed_time;
}

struct timeval do_multi_thread() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    int total_count = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, count_characters, (void *)&thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_count += global_count[i];
    }

    gettimeofday(&end_time, NULL);
    printf("Multi Thread: Total occurrences of 'a': %d\n", total_count);

    struct timeval elapsed_time;
    timersub(&end_time, &start_time, &elapsed_time);
    return elapsed_time;
}

void print_diff(struct timeval single_time, struct timeval multi_time) {
    printf("Single Thread Time: %ld.%06ld seconds\n", single_time.tv_sec, single_time.tv_usec);
    printf("Multi Thread Time: %ld.%06ld seconds\n", multi_time.tv_sec, multi_time.tv_usec);
}

int main(int argc, char* argv[]) {
    // Initialize the string with random characters
    for (int i = 0; i < STRING_LENGTH; i++) {
        string[i] = 'a' + rand() % 26;
    }

    struct timeval single_thread_processing_time = do_single_thread();
    struct timeval multi_thread_processing_time = do_multi_thread();

    print_diff(single_thread_processing_time, multi_thread_processing_time);

    return 0;
}

