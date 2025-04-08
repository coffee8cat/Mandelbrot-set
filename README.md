# Mandelbrot Set

The Mandelbrot set is the set of points C in the complex plane for which the recurrence relation z(n+1) = z(n)^2 + C at z(0) = 0 defines a bounded sequence. In other words, it is the set of C for which there exists a real R such that the inequality
|z(n)| < R holds for all natural n.

## Task
 Drawing the Mandelbrot set with graphical output, followed by optimization via SIMD calculations.

## Calculations

For each pixel in the displayed window, a sequence of points on the complex plane is calculated, starting from the point C corresponding to the pixel, using the recursive formula z(n+1) = z(n)^2 + C, until the iteration number N exceeds MAX_ITER = 255 or z(N) leaves the circle with the center at the origin of fixed radius (10). The resulting number N is converted to RGBA in a certain way to display the Mandelbrot set, RGBA are written to an array for subsequent rendering.

## Optimizations

To determine the efficiency of the optimization, testing was carried out using the rdtsc intrinsic to count the number of processor cycles for calculations for a given number (100) of frames. Two variants of calculations via SIMD were tested: for float and double on 256-bit registers. The results are represented in the tables and diagrams:

### acceleration testing results

Accelaration compared with basic version (No Optimization) compiled with O0:
Flages | No Optimization | Loop Unrolling x4 | SIMD 256 float | SIMD 256 double |
|-----|-----|-----|-----|-----|
| O0 | 1.0   | 0.884 | 2.911  | 1.657 |
| O1 | 2.661 | 2.899 | 14.625 | 8.358 |
| O2 |2.566  | 3.333 | 14.551 | 8.276 |
| O3 |2.567  | 5.099 | 14.344 | 8.353 |

Accelaration compared with basic version compiled with the matching flags for each column:
Flages | Loop Unrolling x4 | SIMD 256 float | SIMD 256 double |
|-----|-----|-----|-----|
| O0 | 0.884| 2.911 | 1.657 |
| O1 | 1.089| 5.496 | 3.141 |
| O2 | 1.299| 5.671 | 3.226 |
| O3 | 1.987| 5.588 | 3.254 |

![picture](readme_pic//optimization_comparison_loop_unroll.png)

![picture](readme_pic//optimization_comparison_SIMD256double.png)
![picture](readme_pic//optimization_comparison_SIMD256float.png)

Final comparison of optimizations compiled with O3:

![picture](readme_pic//optimization_comparison_O0.png)
![picture](readme_pic//optimization_comparison_O3.png)
The greatest performance gain is observed in the version with float (32 bits) - at each iteration of the calculation cycle, calculations are performed for 8 pixels at once, while for double (64 bits) - 4, without optimization - only one.

## Graphical mode

In the running application, you can navigate the image using the keys <kbd>W</kbd> <kbd>A</kbd> <kbd>S</kbd> <kbd>D</kbd>, and also zoom in and out using <kbd>E</kbd> and <kbd>Q</kbd>.

![picture](readme_pic//Mandelbrot-set.png)
![picture](readme_pic//Mandelbrot-set-zoomed.png)
