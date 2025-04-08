#ifndef _MANDELBROT_COMPUTING_H__
#define _MANDELBROT_COMPUTING_H__

#include <SFML/Graphics.hpp>
#include <x86intrin.h>
#include <cmath>
#include <vector>
#include <assert.h>

typedef void (ComputingFuncPtr)(std::vector<sf::Uint8>& pixels);

ComputingFuncPtr computeMandelbrot;
ComputingFuncPtr Loop_Unroll_compute_Mandelbrot;
ComputingFuncPtr SIMD_m256s_computeMandelbrot;
ComputingFuncPtr SIMD_m256d_computeMandelbrot;

#endif
