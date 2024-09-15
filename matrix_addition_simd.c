#include <stdio.h>
#include <time.h>
#include <immintrin.h> 
#include <stdlib.h>
#include <stddef.h>

void matrix_add(const float* left, const float* right, float* result, ptrdiff_t rows, ptrdiff_t cols) {
    const ptrdiff_t size = rows * cols;
    ptrdiff_t i = 0;
    for (; i + 8 < size; i += 8) {
        __m256 left_vec = _mm256_loadu_ps(left + i);
        __m256 right_vec = _mm256_loadu_ps(right + i);
        __m256 res_vec = _mm256_add_ps(left_vec, right_vec);
        _mm256_storeu_ps(result + i, res_vec);
    }
    // Handle remaining elements
    for (; i < size; ++i) {
        result[i] = left[i] + right[i];
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
    matrix_add(left, right, result_simd, rows, cols);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double simd_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("SIMD Time: %f seconds\n", simd_time);
    
    free(left);
    free(right);
    free(result_simd);
    free(result_regular);

    return 0;
}
