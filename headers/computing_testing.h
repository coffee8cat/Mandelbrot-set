#ifndef _COMPUTING_TESTING_H__
#define _COMPUTING_TESTING_H__

//#include <cstdlib>
//#include <cstdio>
//#include <cstdint>

#include "computing.h"

inline uint64_t rdtsc() {
    return __rdtsc();
}

void run_tests(size_t n_tests);
size_t uint64_array_to_string(char* buffer, size_t buffer_size, const uint64_t* array, size_t array_len);
void file_dump(uint64_t* test_results);

#endif
