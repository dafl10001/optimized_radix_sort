#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define true 1
#define false 0

void countSortPass(int* src, int* dst, int size, int shift, int* offsets) {
    for (int i = 0; i < size; i++) {
        int byte = (src[i] >> shift) & 0xFF;
        dst[offsets[byte]++] = src[i];
    }
}

void radixSort(int* list, int size) {
    int* buffer = (int*)malloc(size * sizeof(int));
    if (!buffer) return;

    int counts[4][256] = {0};

    for (int i = 0; i < size; i++) {
        unsigned int val = (unsigned int)list[i];
        counts[0][val & 0xFF]++;
        counts[1][(val >> 8) & 0xFF]++;
        counts[2][(val >> 16) & 0xFF]++;
        counts[3][(val >> 24) & 0xFF]++;
    }

    for (int p = 0; p < 4; p++) {
        int total = 0;
        for (int i = 0; i < 256; i++) {
            int old_count = counts[p][i];
            counts[p][i] = total;
            total += old_count;
        }
    }
    
    // "ping-pong" between the lists
    countSortPass(list, buffer, size, 0, counts[0]);
    countSortPass(buffer, list, size, 8, counts[1]);
    countSortPass(list, buffer, size, 16, counts[2]);
    countSortPass(buffer, list, size, 24, counts[3]);

    free(buffer);
}

int qsort_cmp(const void *a, const void *b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int main() {
    srand(time(NULL));
    int size = 1000000;
    int array[size];

    for (int i = 0; i < size; i++) {
        array[i] = rand() % 2147483648;
    }

    int buf[size];
    memcpy(&buf, &array, sizeof(array));

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    radixSort(array, size);

    clock_gettime(CLOCK_MONOTONIC, &end);

    long long timetaken = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    //printf("\nOwn Radix sort took %lli ns (%.2f µs)\n", timetaken, timetaken / 1000.0f);
    //
    //clock_gettime(CLOCK_MONOTONIC, &start);
    //
    //qsort(buf, size, sizeof(int), qsort_cmp);
    //
    //clock_gettime(CLOCK_MONOTONIC, &end);
    //timetaken = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    //
    //printf("\nStandard qsort took %lli ns (%.2f µs)\n", timetaken, timetaken / 1000.0f);

    return 0;
}
