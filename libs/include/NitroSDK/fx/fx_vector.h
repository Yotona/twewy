/**
 * @file fx_vector.h
 * @brief Header for FX vector operations, providing 3D vector manipulation functions for Nintendo DS.
 *
 * This module offers a comprehensive set of functions for performing operations
 * on 3D vectors used in 3D graphics and calculations. It includes basic arithmetic
 * operations (addition, subtraction), dot and cross products, magnitude, normalization,
 * distance calculations, and scaled addition. All operations use fixed-point arithmetic
 * suitable for the Nintendo DS hardware.
 */

#ifndef NITROSDK_FX_VECTOR_H
#define NITROSDK_FX_VECTOR_H

#include <types.h>

/**
 * @typedef Vec
 * @brief 3D vector struct for fixed-point coordinates.
 *
 * This struct represents a 3D vector with X, Y, and Z components,
 * all stored as 32-bit fixed-point numbers with 12 bits of fractional precision.
 * Used for positions, directions, and other 3D quantities in graphics calculations.
 */
typedef struct {
    s32 x; /**< X component of the vector. */
    s32 y; /**< Y component of the vector. */
    s32 z; /**< Z component of the vector. */
} Vec;

/**
 * @brief Adds two 3D vectors component-wise.
 *
 * Computes the vector sum dest = a + b, where each component is added separately.
 * All vectors use fixed-point arithmetic.
 *
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 * @param dest Pointer to the destination vector for the result.
 */
void Vec_Add(const Vec* a, const Vec* b, Vec* dest);

/**
 * @brief Subtracts two 3D vectors component-wise.
 *
 * Computes the vector difference dest = a - b, where each component is subtracted separately.
 * All vectors use fixed-point arithmetic.
 *
 * @param a Pointer to the minuend vector.
 * @param b Pointer to the subtrahend vector.
 * @param dest Pointer to the destination vector for the result.
 */
void Vec_Sub(const Vec* a, const Vec* b, Vec* dest);

/**
 * @brief Computes the dot product of two 3D vectors.
 *
 * Calculates the scalar dot product a ? b with appropriate rounding for fixed-point arithmetic.
 * The result is scaled to maintain precision.
 *
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 * @return The dot product result (32-bit fixed-point).
 */
s32 Vec_DotProduct(const Vec* a, const Vec* b);

/**
 * @brief Computes the cross product of two 3D vectors.
 *
 * Calculates the vector cross product dest = a ~ b, which produces a vector perpendicular
 * to both input vectors. Uses fixed-point arithmetic with rounding.
 *
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 * @param dest Pointer to the destination vector for the result.
 */
void Vec_CrossProduct(const Vec* a, const Vec* b, Vec* dest);

/**
 * @brief Calculates the magnitude (length) of a 3D vector.
 *
 * Computes the Euclidean norm ||v|| using the hardware square root unit.
 * Returns the magnitude in fixed-point format.
 *
 * @param v Pointer to the vector.
 * @return The magnitude of the vector (32-bit fixed-point).
 */
s32 Vec_Magnitude(const Vec* v);

/**
 * @brief Normalizes a 3D vector to unit length.
 *
 * Scales the input vector so that its magnitude becomes 1.0, producing a unit vector
 * in the same direction. Uses hardware division and square root for accuracy.
 *
 * @param src Pointer to the source vector.
 * @param dest Pointer to the destination vector for the normalized result.
 */
void Vec_Normalize(const Vec* src, Vec* dest);

/**
 * @brief Performs scaled vector addition.
 *
 * Computes dest = v2 + a * v1, where a is a scalar multiplier.
 * Useful for linear combinations and interpolation operations.
 *
 * @param a Scalar multiplier (32-bit fixed-point).
 * @param v1 Pointer to the vector to be scaled.
 * @param v2 Pointer to the vector to be added.
 * @param pDest Pointer to the destination vector for the result.
 */
void Vec_MultAdd(s32 a, const Vec* v1, const Vec* v2, Vec* pDest);

/**
 * @brief Calculates the Euclidean distance between two 3D vectors.
 *
 * Computes the distance ||v1 - v2|| between two points in 3D space.
 * Uses the hardware square root unit for efficient calculation.
 *
 * @param v1 Pointer to the first vector (point).
 * @param v2 Pointer to the second vector (point).
 * @return The distance between the vectors (32-bit fixed-point).
 */
s32 Vec_Distance(const Vec* v1, const Vec* v2);

#endif // NITROSDK_FX_VECTOR_H