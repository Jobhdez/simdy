#include <stdio.h>
#include <time.h>
#include <immintrin.h> 
#include <stdlib.h>


float vector_dot_regular(const float* left, const float* right, ptrdiff_t n) {
    float result = 0.f;
    for (ptrdiff_t i = 0; i < n; ++i) {
        result += left[i] * right[i];
    }
    return result;
}

int main() {
    const ptrdiff_t n = 1024 * 1024; 
    float* left = (float*)malloc(n * sizeof(float));
    float* right = (float*)malloc(n * sizeof(float));

    if (left == NULL || right == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }

    for (ptrdiff_t i = 0; i < n; ++i) {
        left[i] = 1.0f;
        right[i] = 2.0f;
    }

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    float regular_result = vector_dot_regular(left, right, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double regular_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Regular Dot Product: %f, Time: %f seconds\n", regular_result, regular_time);

    free(left);
    free(right);

    return 0;
}
