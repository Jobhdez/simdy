#include <stdio.h>
#include <time.h>
#include <immintrin.h> 
#include <stdlib.h>
#include <stddef.h>

float vector_dot_simd(const float* left, const float* right, ptrdiff_t n) {
    __m256 vsum = _mm256_setzero_ps();
    __m128 high_half, low_half;
    float headsum, tailsum = 0.f;
    ptrdiff_t i = 0;

    // Process 8 elements at a time using AVX
    if (n > 7) {
        for (; i < n - 7; i += 8) {
            const __m256 lefti = _mm256_loadu_ps(left + i);
            const __m256 righti = _mm256_loadu_ps(right + i);
            vsum = _mm256_fmadd_ps(lefti, righti, vsum);
        }
    }

    // Reduce 8 elements to 4
    high_half = _mm256_extractf128_ps(vsum, 1);
    low_half = _mm256_castps256_ps128(vsum);
    low_half = _mm_add_ps(low_half, high_half);
    
    // Reduce 4 to 2
    high_half = _mm_movehl_ps(low_half, low_half);
    low_half = _mm_add_ps(low_half, high_half);
    
    // Reduce 2 to 1
    high_half = _mm_movehdup_ps(low_half);
    low_half = _mm_add_ss(low_half, high_half);
    headsum = _mm_cvtss_f32(low_half);

    // Process the remaining elements
    for (; i < n; ++i)
        tailsum += left[i] * right[i];

    return headsum + tailsum;
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
    float simd_result = vector_dot_simd(left, right, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double simd_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("SIMD Dot Product: %f, Time: %f seconds\n", simd_result, simd_time);

    free(left);
    free(right);

    return 0;
}
