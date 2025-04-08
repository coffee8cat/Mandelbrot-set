#include "computing.h"
#include "params.h"

inline float iy_to_y0(int iy) {
    return (iy - HEIGHT / 2.0) * 3.0 / (HEIGHT * zoom) + offsetY;
}

inline float ix_to_x0(int ix) {
    return (ix - WIDTH / 2.0) * 3.0 / (WIDTH * zoom) + offsetX;
}

void Loop_Unroll_compute_Mandelbrot(std::vector<sf::Uint8>& pixels) {
    const size_t loop_unroll_degree = 8;
    for (int iy = 0; iy < HEIGHT; iy++) {

        //prepare y coords for ix iterations
        float y0_coord = iy_to_y0(iy);
        float y0[loop_unroll_degree] = {};
        float y[loop_unroll_degree]  = {};

        for (size_t i = 0; i < loop_unroll_degree; i++) {
            y0[i] = y0_coord;
        }

        for (int ix = 0; ix < WIDTH; ix = ix + loop_unroll_degree) {
            float x0[loop_unroll_degree] = {};
            float x[loop_unroll_degree]  = {};

            for (size_t i = 0; i < loop_unroll_degree; i++) {
                x0[i] = ix_to_x0(ix + i);
                x[i]  = x0[i];
                y[i]  = y0_coord;
            }

            uint32_t active_mask = (1 << loop_unroll_degree) - 1;
            size_t N = 0;

            size_t iterations[loop_unroll_degree] = {};

            float x2[loop_unroll_degree] = {};
            float y2[loop_unroll_degree] = {};
            float xy[loop_unroll_degree] = {};
            float r2[loop_unroll_degree] = {};

            for (; N < MAX_ITER; N++) {

                for (size_t i = 0; i < loop_unroll_degree; i++) {
                    float x_temp = x[i];
                    float y_temp = y[i];
                    x2[i] = x_temp * x_temp;
                    y2[i] = y_temp * y_temp;
                    xy[i] = x_temp * y_temp;
                    r2[i] = x2[i]  + y2[i];
                }

                for (size_t i = 0; i < loop_unroll_degree; i++) {
                    if (r2[i] > R2_MAX) {
                        active_mask &= ~(1 << i);
                    }
                }

                if (not active_mask) { break; }

                for (size_t i = 0; i < loop_unroll_degree; i++) {
                    if (!(active_mask & (1 << i))) continue;
                    x[i] = x2[i] - y2[i] + x0[i];
                    y[i] = xy[i] * 2 + y0[i];
                    (iterations[i])++;
                }
            }

            // RGBA for Mandelbrot set
            // (50,  25, 170)       - set (N = MAX_ITER = 254, N % 2 == 0)
            // (120, 25, 170)       - N % 2 == 0
            // (50, 100, 170)       - N % 2 == 1

            for (size_t i = 0; i < loop_unroll_degree; i++) {

                size_t N = iterations[i];
                float t = static_cast<float>(N) / MAX_ITER;
                sf::Uint8 R = static_cast<sf::Uint8>(50 + 70 * (1 - t) * ((N + 1) / 2));
                sf::Uint8 G = static_cast<sf::Uint8>(25 + 75 * (N % 2));
                sf::Uint8 B = static_cast<sf::Uint8>(170);

                // fill RGBA for pixel
                int index = (iy * WIDTH + ix + i) * 4;
                pixels[index]     = R;
                pixels[index + 1] = G;
                pixels[index + 2] = B;
                pixels[index + 3] = 255;
            }
        }
    }
}

void computeMandelbrot(std::vector<sf::Uint8>& pixels) {
    for (int iy = 0; iy < HEIGHT; iy++) {
        for (int ix = 0; ix < WIDTH; ix++) {
            float x0 = ix_to_x0(ix);
            float y0 = iy_to_y0(iy);
            float x = x0, y = y0;

            size_t N = 0;
            for (; N < MAX_ITER; N++) {
                float x2 = x * x, y2 = y * y, xy = x * y, r2 = x2 + y2;
                if (r2 > R2_MAX) break;

                x = x2 - y2 + x0;
                y = xy + xy + y0;
            }
            // (50,  25, 170)       - set (N = MAX_ITER = 254, N % 2 == 0)
            // (120, 25, 170)       - N % 2 == 0
            // (50, 100, 170)       - N % 2 == 1

            float t = static_cast<float>(N) / MAX_ITER;
            sf::Uint8 R = static_cast<sf::Uint8>(50 + 70 * (1 - t) * ((N+1) / 2));
            sf::Uint8 G = static_cast<sf::Uint8>(25 + 75 * (N % 2));
            sf::Uint8 B = static_cast<sf::Uint8>(170);

            // fill RGBA for pixel
            int index = (iy * WIDTH + ix) * 4;
            pixels[index]     = R;
            pixels[index + 1] = G;
            pixels[index + 2] = B;
            pixels[index + 3] = 255;
        }
    }
}

void SIMD_m256s_computeMandelbrot(std::vector<sf::Uint8>& pixels) {
    //assert(pixels);
    //#pragma omp parallel for
    for (int iy = 0; iy < HEIGHT; iy++) {
        __m256 y0 = _mm256_set1_ps(iy_to_y0(iy));

        for (int ix = 0; ix < WIDTH - 8; ix += 8) {
            __m256 x0 = _mm256_set_ps(  ix_to_x0(ix+7), ix_to_x0(ix+6), ix_to_x0(ix+5), ix_to_x0(ix+4),
                                        ix_to_x0(ix+3), ix_to_x0(ix+2), ix_to_x0(ix+1), ix_to_x0(ix) );

            __m256 x = x0;
            __m256 y = y0;
            __m256 N = _mm256_setzero_ps();

            for (size_t iter = 0; iter < MAX_ITER; iter++) {
                __m256 x2 = _mm256_mul_ps(x, x);
                __m256 y2 = _mm256_mul_ps(y, y);
                __m256 xy = _mm256_mul_ps(x, y);
                __m256 r2 = _mm256_add_ps(x2, y2);

                __m256 mask = _mm256_cmp_ps(r2, _m256s_R2_MAX, _CMP_LT_OQ);
                int mask_bits = _mm256_movemask_ps(mask);
                if (mask_bits == 0) { break; } // check if all pixels escaped

                N = _mm256_sub_ps(N, _mm256_and_ps(mask, _mm256_set1_ps(1.0f)));
                x = _mm256_add_ps(_mm256_sub_ps(x2, y2), x0);
                y = _mm256_add_ps(_mm256_add_ps(xy, xy), y0);
            }

            alignas(32) float N_results[8];
            _mm256_store_ps(N_results, N);

            for (size_t i = 0; i < 8; i++) {
                bool parity = static_cast<int>(N_results[i]) % 2;

                // (50,  25, 170)       - set (N = MAX_ITER = 254, N % 2 == 0)
                // (120, 25, 170)       - N % 2 == 0
                // (50, 100, 170)       - N % 2 == 1
                float t = N_results[i] / MAX_ITER;

                sf::Uint8 R = static_cast<sf::Uint8>(50 + 70 * (1 - std::sin(t)) * ((static_cast<int>(N_results[i]) + 1) / 2));
                sf::Uint8 G = static_cast<sf::Uint8>(25 + 75 * parity);
                sf::Uint8 B = static_cast<sf::Uint8>(170);

                int index = (iy * WIDTH + (ix + i)) * 4;
                pixels[index]     = R;
                pixels[index + 1] = G;
                pixels[index + 2] = B;
                pixels[index + 3] = 255;
            }
        }
    }
}


void SIMD_m256d_computeMandelbrot(std::vector<sf::Uint8>& pixels) {
    for (int iy = 0; iy < HEIGHT; iy++) {
        __m256d y0 = _mm256_set1_pd(iy_to_y0(iy));

        for (int ix = 0; ix < WIDTH; ix += 4) {
            __m256d x0 = _mm256_set_pd( ix_to_x0(ix+3), ix_to_x0(ix+2), ix_to_x0(ix+1), ix_to_x0(ix) );

            __m256d x = x0;
            __m256d y = y0;
            __m256d N = _mm256_setzero_pd();

            for (size_t iter = 0; iter < MAX_ITER; iter++) {
                __m256d x2 = _mm256_mul_pd(x, x);
                __m256d y2 = _mm256_mul_pd(y, y);
                __m256d xy = _mm256_mul_pd(x, y);
                __m256d r2 = _mm256_add_pd(x2, y2);

                __m256d mask  = _mm256_cmp_pd(r2, _m256d_R2_MAX, _CMP_LT_OQ);
                int mask_bits = _mm256_movemask_pd(mask);
                if (mask_bits == 0) { break; } // check if all pixels escaped

                N = _mm256_sub_pd(N, _mm256_and_pd(mask, _mm256_set1_pd(1.0f)));
                x = _mm256_add_pd(_mm256_sub_pd(x2, y2), x0);
                y = _mm256_add_pd(_mm256_add_pd(xy, xy), y0);
            }

            alignas(32) double N_results[4];
            _mm256_store_pd(N_results, N);

            for (size_t i = 0; i < 4; i++) {
                bool parity = static_cast<int>(N_results[i]) % 2;

                // (50,  25, 170)       - set (N = MAX_ITER = 254, N % 2 == 0)
                // (120, 25, 170)       - N % 2 == 0
                // (50, 100, 170)       - N % 2 == 1
                float t = N_results[i] / MAX_ITER;

                sf::Uint8 R = static_cast<sf::Uint8>(50 + 70 * (1 - std::sin(t)) * ((static_cast<int>(N_results[i]) + 1) / 2));
                sf::Uint8 G = static_cast<sf::Uint8>(25 + 75 * parity);
                sf::Uint8 B = static_cast<sf::Uint8>(170);

                int index = (iy * WIDTH + (ix + i)) * 4;
                pixels[index]     = R;
                pixels[index + 1] = G;
                pixels[index + 2] = B;
                pixels[index + 3] = 255;
            }
        }
    }
}
