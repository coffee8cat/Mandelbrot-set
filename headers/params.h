#ifndef _MANDELBROT_PARAMS_H__
#define _MANDELBROT_PARAMS_H__

extern const int WIDTH;
extern const int HEIGHT;

extern float zoom;
extern float offsetX, offsetY;


extern const int MAX_ITER;
extern const int R2_MAX;
extern const __m256  _m256s_R2_MAX;
extern const __m256d _m256d_R2_MAX;

extern const size_t n_tests_default;

#endif
