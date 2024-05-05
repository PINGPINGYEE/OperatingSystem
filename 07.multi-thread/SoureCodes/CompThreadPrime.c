#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define NUM_THREADS 13
#define MAX_NUMBER 50000000

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num % 2 == 0 && num > 2) return 0;
    for (int i = 3; i <= sqrt(num); i += 2) {
        if (num % i == 0) return 0;
    }
    return 1;
}

void* find_primes(void* threadarg) {
    int thread_index = *(int*)threadarg;
    int segment = MAX_NUMBER / NUM_THREADS;
    int start = thread_index * segment;
    int end = (thread_index == NUM_THREADS - 1) ? MAX_NUMBER : start + segment;
    for (int num = start; num < end; num++) {
        is_prime(num);
    }
    pthread_exit(NULL);
}

struct timeval do_single_thread() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    
    for (int num = 2; num <= MAX_NUMBER; num++) {
        is_prime(num);
    }
    
    gettimeofday(&end_time, NULL);
    struct timeval elapsed_time;
    timersub(&end_time, &start_time, &elapsed_time);
    return elapsed_time;
}

struct timeval do_multi_thread() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, find_primes, (void *)&thread_args[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    gettimeofday(&end_time, NULL);
    struct timeval elapsed_time;
    timersub(&end_time, &start_time, &elapsed_time);
    return elapsed_time;
}

void print_diff(struct timeval single_time, struct timeval multi_time) {
    printf("Single Thread Time: %ld.%06ld seconds\n", single_time.tv_sec, single_time.tv_usec);
    printf("Multi Thread Time: %ld.%06ld seconds\n", multi_time.tv_sec, multi_time.tv_usec);
}

int main(int argc, char* argv[]) {
    struct timeval single_thread_processing_time = do_single_thread();
    struct timeval multi_thread_processing_time = do_multi_thread();

    print_diff(single_thread_processing_time, multi_thread_processing_time);

    return 0;
}

