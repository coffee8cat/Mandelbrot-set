#include "computing_testing.h"
#include "params.h"

const ComputingFuncPtr* Funcs_for_test[] = {&computeMandelbrot, &Loop_Unroll_compute_Mandelbrot,
                                            &SIMD_m256s_computeMandelbrot, &SIMD_m256d_computeMandelbrot};
const size_t n_funcs_for_testing = sizeof(Funcs_for_test) / sizeof(Funcs_for_test[0]);

const size_t benchmark_index = 0;
const char dump_file_name[] = "run_results.txt";

void run_tests(size_t n_tests) {

    std::vector<sf::Uint8> pixels(WIDTH * HEIGHT * 4);
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

    printf("-----Testing results-----\n");
    printf("Number of processor cycles per function:\n");
    for (size_t i = 0; i < n_funcs_for_testing; i++) {
        printf("%zu: %lu\n", i, test_results[i]);
    }

    printf("Normilized values:\n");
    double benchmark_elem = static_cast<double>(test_results[benchmark_index]);
    for (size_t i = 0; i < n_funcs_for_testing; i++) {
        printf("%zu: %lf\n", i, benchmark_elem / test_results[i]);
    }

    file_dump(test_results);
}

size_t uint64_array_to_string(char* buffer, size_t buffer_size, const uint64_t* array, size_t array_len) {
    char* ptr = buffer;
    int remaining = buffer_size;

    for (size_t i = 0; i < array_len; ++i) {
        int written = snprintf(ptr, remaining, "%lu \n", array[i]);

        if (written < 0 || written >= remaining) {
            break;
        }

        ptr += written;
        remaining -= written;
    }

    printf("Buffer is ready for fwrite of %zu bytes: \"%s\"\n", (size_t)(ptr - buffer), buffer);
    return (size_t)(ptr - buffer);
}

void file_dump(uint64_t* test_results) {
    assert(test_results);

    char results_string[BUFSIZ] = "";
    size_t written = uint64_array_to_string(results_string, BUFSIZ, test_results, n_funcs_for_testing);

    char file_name[64] = "";
    snprintf(file_name, 64, "dump/%s", dump_file_name);

    FILE* fp = fopen(file_name, "a+");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file for writing %s in %s in %s:%d", dump_file_name, __func__, __FILE__, __LINE__);
        return;
    }

    fwrite(results_string, written, sizeof(char), fp);

    if (not fclose(fp)) {
        fprintf(stderr, "Could not close file after writing %s in %s in %s:%d", dump_file_name, __func__, __FILE__, __LINE__);
    }
}
