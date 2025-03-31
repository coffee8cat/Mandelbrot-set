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

#endif
