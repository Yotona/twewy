
#ifndef SQRT_H
#define SQRT_H

#include <types.h>

/**
 * @brief Calculates the integer square root of a number using Newton's method.
 *
 * @param n The input number for which to calculate the square root
 * @return The integer square root of n, or 0 if n <= 0
 *
 * @details
 * This function computes the square root using an optimized Newton-Raphson iteration:
 * 1. First, it finds an initial guess by doubling until it exceeds the square root
 * 2. Then it refines the guess iteratively using the formula: guess = (guess + n/guess) / 2
 * 3. The iteration stops when the guess stops improving (converges)
 * 4. Returns the last guess that was smaller than the previous one
 */
s32 Sqrti(s32 n);

#endif // SQRT_H