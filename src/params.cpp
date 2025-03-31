#include <x86intrin.h>
#include <cstdlib>

#include "params.h"

const int WIDTH = 800;
const int HEIGHT = 600;

float zoom = 1.0f;
float offsetX = 0.0f, offsetY = 0.0f;


const int MAX_ITER = 255;
const int R2_MAX = 10;
const __m256  _m256s_R2_MAX = _mm256_set1_ps(R2_MAX);
const __m256d _m256d_R2_MAX = _mm256_set1_pd(R2_MAX);

const size_t n_tests_default = 100;
