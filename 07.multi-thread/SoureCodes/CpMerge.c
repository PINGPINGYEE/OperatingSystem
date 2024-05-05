#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE 500000
#define CASE 4

typedef struct params {
    int* arr;
    int left;
    int right;
    int is_threaded;
} Params;

void* recursive_merge_sort(void* input) {
    Params* params = (Params*)input;
    int mid = (params->right + params->left) / 2;

    if (params->right > params->left) {
        Params left = {params->arr, params->left, mid, params->is_threaded - 1};
        Params right = {params->arr, mid + 1, params->right, params->is_threaded - 1};

        if (params->is_threaded > 0) {
            pthread_t sub_l;
            int status_l;

            status_l = pthread_create(&sub_l, NULL, recursive_merge_sort, (void*)&left);
            if (status_l != 0) {
                perror("pthread_create failed");
                return NULL;
            }

            recursive_merge_sort(&right);

            status_l = pthread_join(sub_l, NULL);
            if (status_l != 0) {
                perror("pthread_join failed");
            }
        } else {
            recursive_merge_sort(&left);
            recursive_merge_sort(&right);
        }

        int* temp = (int*)malloc(sizeof(int) * (params->right - params->left + 1));
        int i = params->left, j = mid + 1, k = 0;

        while (i <= mid && j <= params->right) {
            temp[k++] = params->arr[i] < params->arr[j] ? params->arr[i++] : params->arr[j++];
        }
        while (i <= mid) {
            temp[k++] = params->arr[i++];
        }
        while (j <= params->right) {
            temp[k++] = params->arr[j++];
        }

        for (int in = 0; in < params->right - params->left + 1; in++) {
            params->arr[in + params->left] = temp[in];
        }

        free(temp);
    }
    return NULL;
}

char* order_check(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return "error";
        }
    }
    return "ok";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <depth>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);

    int arr[CASE][SIZE];

    for (int i = 0; i < SIZE; i++) {
        arr[0][i] = i % 2 ? i * 2: i;
        arr[1][i] = SIZE - i;
        arr[2][i] = i;
        arr[3][i] = 1;
    }

    double sum = 0;

    for (int i = 0; i < CASE; i++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);

        Params params = {arr[i], 0, SIZE - 1, n};
        printf("case %d start sorting...\n", i + 1);

        recursive_merge_sort(&params);

        gettimeofday(&end, NULL);

        double subsum = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000.0;
        sum += subsum;

        printf("%lf msec later\n", subsum);
        printf("check %s\n", order_check(arr[i], SIZE));
    }

    printf("average time: %lf msec\n", sum / CASE);
    return 0;
}
