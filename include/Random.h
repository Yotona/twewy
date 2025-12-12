#ifndef RANDOM_H
#define RANDOM_H

/**
 * @file Random.h
 * @brief Random number generator functions.
 */

#include <types.h>

/**
 * @brief Sets the random number generator seed.
 * @param seed The new seed value, or default if 0.
 * @return The previous seed value.
 */
u32 RNG_SetSeed(u32 seed);

/**
 * @brief Generates the next random number scaled to the given range.
 * @param scale The upper bound (exclusive) for the random number.
 * @return A random number between 0 and scale-1.
 */
u16 RNG_Next(u32 scale);

#endif