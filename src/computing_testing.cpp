#include "computing_testing.h"
#include "params.h"

const ComputingFuncPtr* Funcs_for_test[] = {&computeMandelbrot, &SIMD_m256s_computeMandelbrot, &SIMD_m256d_computeMandelbrot};

void run_tests(size_t n_tests) {

    std::vector<sf::Uint8> pixels(WIDTH * HEIGHT * 4);

    size_t n_funcs_for_testing = sizeof(Funcs_for_test) / sizeof(Funcs_for_test[0]);
    printf("Number of functions to test: %zu\n", n_funcs_for_testing);
    uint64_t test_results[n_funcs_for_testing] = {};
    uint64_t normilized_test_results[n_funcs_for_testing] = {};
    uint64_t temp_res = 0;

    for (size_t n = 0; n < n_funcs_for_testing; n++) {
        ComputingFuncPtr* func = Funcs_for_test[n];
        func(pixels);
        uint64_t start = rdtsc();
        for (size_t i = 0; i < n_tests; i++) {
            //uint64_t start = rdtsc();
            func(pixels);
            //uint64_t end = rdtsc();
            //temp_res += end - start;
        }
        uint64_t end = rdtsc();
        temp_res = end - start;
        test_results[n] = temp_res;
        temp_res = 0;
    }
    /*
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
    */

    printf("-----Testing results-----\n");
    printf("Number of processor cycles per function:\n");
    for (size_t i = 0; i < n_funcs_for_testing; i++) {
        printf("%zu: %lu\n", i, test_results[i]);
    }

    printf("Normilizes values:\n");
    double prior_elem = static_cast<double>(test_results[0]);
    for (size_t i = 0; i < n_funcs_for_testing; i++) {
        printf("%zu: %lf\n", i, prior_elem / test_results[i]);
    }
}
