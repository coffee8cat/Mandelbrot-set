#include "computing.h"
#include "params.h"

void computeMandelbrot(std::vector<sf::Uint8>& pixels) {
    for (int iy = 0; iy < HEIGHT; iy++) {
        for (int ix = 0; ix < WIDTH; ix++) {
            float x0 = (ix - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX;
            float y0 = (iy - HEIGHT / 2.0f) * 3.0f / (HEIGHT * zoom) + offsetY;
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
    for (int iy = 0; iy < HEIGHT; iy++) {
        // Precompute y0 for the entire row (constant for all ix)
        float y0_scalar = (iy - HEIGHT / 2.0f) * 3.0f / (HEIGHT * zoom) + offsetY;
        __m256 y0 = _mm256_set1_ps(y0_scalar);

        for (int ix = 0; ix < WIDTH; ix += 8) {
            // Initialize x0 with 8 consecutive x-coordinates
            // order is reversed because it fills the vector from high to low memory.
            __m256 x0 = _mm256_set_ps(
                (ix + 7 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 6 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 5 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 4 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 3 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 2 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 1 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 0 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX
            );

            __m256 x = x0;
            __m256 y = y0;
            __m256 N = _mm256_setzero_ps();

            for (size_t iter = 0; iter < MAX_ITER; iter++) {
                __m256 x2 = _mm256_mul_ps(x, x);
                __m256 y2 = _mm256_mul_ps(y, y);
                __m256 xy = _mm256_mul_ps(x, y);
                __m256 r2 = _mm256_add_ps(x2, y2);

                // Mask for pixels that haven't escaped
                __m256 mask = _mm256_cmp_ps(r2, _m256s_R2_MAX, _CMP_LT_OQ);
                int mask_bits = _mm256_movemask_ps(mask);
                if (mask_bits == 0) { break; } // check if all pixels escaped

                // update values
                N = _mm256_sub_ps(N, _mm256_and_ps(mask, _mm256_set1_ps(1.0f)));
                x = _mm256_add_ps(_mm256_sub_ps(x2, y2), x0);
                y = _mm256_add_ps(_mm256_add_ps(xy, xy), y0);
            }

            // Store results
            float N_results[8];
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
        // Precompute y0 for the entire row (constant for all ix)
        float y0_scalar = (iy - HEIGHT / 2.0f) * 3.0f / (HEIGHT * zoom) + offsetY;
        __m256d y0 = _mm256_set1_pd(y0_scalar);

        for (int ix = 0; ix < WIDTH; ix += 4) {
            // Initialize x0 with 8 consecutive x-coordinates
            // order is reversed because it fills the vector from high to low memory.
            __m256d x0 = _mm256_set_pd(
                (ix + 3 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 2 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 1 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX,
                (ix + 0 - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX
            );

            __m256d x = x0;
            __m256d y = y0;
            __m256d N = _mm256_setzero_pd();

            for (size_t iter = 0; iter < MAX_ITER; iter++) {
                __m256d x2 = _mm256_mul_pd(x, x);
                __m256d y2 = _mm256_mul_pd(y, y);
                __m256d xy = _mm256_mul_pd(x, y);
                __m256d r2 = _mm256_add_pd(x2, y2);

                // Mask for pixels that haven't escaped
                __m256d mask  = _mm256_cmp_pd(r2, _m256d_R2_MAX, _CMP_LT_OQ);
                int mask_bits = _mm256_movemask_pd(mask);
                if (mask_bits == 0) { break; } // check if all pixels escaped

                // update values
                N = _mm256_sub_pd(N, _mm256_and_pd(mask, _mm256_set1_pd(1.0f)));
                x = _mm256_add_pd(_mm256_sub_pd(x2, y2), x0);
                y = _mm256_add_pd(_mm256_add_pd(xy, xy), y0);
            }

            // Store results
            double N_results[8];
            _mm256_store_pd(N_results, N);

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
