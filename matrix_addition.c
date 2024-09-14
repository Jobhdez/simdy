#include <stdio.h>
#include <time.h>
#include <immintrin.h> 
#include <stdlib.h>    

void matrix_add_regular(const float* left, const float* right, float* result, ptrdiff_t rows, ptrdiff_t cols) {
    for (ptrdiff_t i = 0; i < rows; ++i) {
        for (ptrdiff_t j = 0; j < cols; ++j) {
            ptrdiff_t index = i * cols + j;
            result[index] = left[index] + right[index];
        }
    }
}

int main() {
    const ptrdiff_t rows = 1024;
    const ptrdiff_t cols = 1024;
    const ptrdiff_t size = rows * cols;

    float* left = (float*)malloc(size * sizeof(float));
    float* right = (float*)malloc(size * sizeof(float));
    float* result_simd = (float*)malloc(size * sizeof(float));
    float* result_regular = (float*)malloc(size * sizeof(float));

    if (left == NULL || right == NULL || result_simd == NULL || result_regular == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }

    for (ptrdiff_t i = 0; i < size; ++i) {
        left[i] = 1.0f;
        right[i] = 2.0f;
    }

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    matrix_add_regular(left, right, result_regular, rows, cols);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double regular_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Regular Time: %f seconds\n", regular_time);
    
    free(left);
    free(right);
    free(result_simd);
    free(result_regular);

    return 0;
}
