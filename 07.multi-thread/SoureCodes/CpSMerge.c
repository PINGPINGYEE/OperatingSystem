#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 500000

void recursive_merge_sort(int* arr, int left, int right)
{
	int mid = (right + left) / 2;

	if (right > left)
	{
		recursive_merge_sort(arr, left, mid);
		recursive_merge_sort(arr, mid + 1, right);

		int* temp = (int*)malloc(sizeof(int) * (right - left + 1));
		int i = left, j = mid + 1, k = 0;

		while (i <= mid && j <= right)
		{
			if (arr[i] < arr[j])
				temp[k++] = arr[i++];
			else
				temp[k++] = arr[j++];
		}
		while (k < right - left + 1)
		{
			if (i <= mid)
				temp[k++] = arr[i++];
			else
				temp[k++] = arr[j++];
		}

		for (int in = 0; in < right - left + 1; in++)
		{
			arr[in + left] = temp[in];
		}

		free(temp);
	}
}

int main()
{
    int arr[4][SIZE] = {};
    
    for (int i = 0; i < SIZE; i++)
    {
        arr[0][SIZE] = i % 2 ? i * 2: i;
        arr[1][SIZE] = SIZE - i;
        arr[2][SIZE] = i;
		arr[3][SIZE] = 1;
    }

	for (int i = 0; i < 4; i++)
	{
		struct timeval start;
		gettimeofday(&start, NULL);
		
		printf("case %d start sorting...\n", i + 1);

		recursive_merge_sort(arr[i], 0, SIZE - 1);
		
		struct timeval end;
		gettimeofday(&end, NULL);

		printf("%lf msec later\n", ((end.tv_sec * (long)1000000 + end.tv_usec) - (start.tv_sec * (long)1000000 + start.tv_usec)) / (double)1000);
	}
}