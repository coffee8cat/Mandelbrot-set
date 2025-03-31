#ifndef _COMPUTING_TESTING_H__
#define _COMPUTING_TESTING_H__

//#include <cstdlib>
//#include <cstdio>
//#include <cstdint>

#include "computing.h"

inline uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("cpuid\n\t"  // Сериализация
                          "rdtsc\n\t"
                          "mov %%edx, %0\n\t"
                          "mov %%eax, %1\n\t"
                          : "=r" (hi), "=r" (lo)
                          :: "%rax", "%rbx", "%rcx", "%rdx");
    return ((uint64_t)hi << 32) | lo;
}

void run_tests(size_t n_tests);

#endif
