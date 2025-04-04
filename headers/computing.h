#ifndef _MANDELBROT_COMPUTING_H__
#define _MANDELBROT_COMPUTING_H__

#include <SFML/Graphics.hpp>
#include <x86intrin.h>
#include <cmath>
#include <vector>

typedef void (ComputingFuncPtr)(std::vector<sf::Uint8>& pixels);

ComputingFuncPtr computeMandelbrot;
ComputingFuncPtr SIMD_m256s_computeMandelbrot;
ComputingFuncPtr SIMD_m256d_computeMandelbrot;
/*
void computeMandelbrot(std::vector<sf::Uint8>& pixels);
void SIMD_m256s_computeMandelbrot(std::vector<sf::Uint8>& pixels);
void SIMD_m256d_computeMandelbrot(std::vector<sf::Uint8>& pixels);
*/
#endif
