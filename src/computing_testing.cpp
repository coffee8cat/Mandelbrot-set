#include "computing_testing.h"
#include "params.h"

void run_tests(size_t n_tests) {

    std::vector<sf::Uint8> pixels(WIDTH * HEIGHT * 4);

    uint64_t test_results[3] = {};
    uint64_t temp_res = 0;

    computeMandelbrot(pixels);
    for (size_t i = 0; i < n_tests; i++) {
        uint64_t start = rdtsc();
        computeMandelbrot(pixels);
        uint64_t end = rdtsc();
        temp_res += end - start;
    }
    test_results[0] = temp_res;
    temp_res = 0;

    SIMD_m256d_computeMandelbrot(pixels);
    for (size_t i = 0; i < n_tests; i++) {
        uint64_t start = rdtsc();
        SIMD_m256d_computeMandelbrot(pixels);
        uint64_t end = rdtsc();
        temp_res += end - start;
    }
    test_results[1] = temp_res;
    temp_res = 0;

    SIMD_m256s_computeMandelbrot(pixels);
    for (size_t i = 0; i < n_tests; i++) {
        uint64_t start = rdtsc();
        SIMD_m256s_computeMandelbrot(pixels);
        uint64_t end = rdtsc();
        temp_res += end - start;
    }
    test_results[2] = temp_res;
    temp_res = 0;

    printf("-----Testing results-----\n");
    printf("0: %lu\n", test_results[0]);
    printf("1: %lu\n", test_results[1]);
    printf("2: %lu\n", test_results[2]);
}
