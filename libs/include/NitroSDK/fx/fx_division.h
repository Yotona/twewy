/**
 * @file fx_division.h
 * @brief Header for FX division operations, providing fixed-point division, inverse, and square root functions for Nintendo
 * DS.
 *
 * This module offers functions for performing fixed-point arithmetic operations
 * using the Nintendo DS hardware division and square root units. It includes
 * synchronous and asynchronous operations for division, inverse calculation,
 * and square root computation with appropriate rounding for 20-bit and 32-bit
 * fixed-point formats.
 */

#ifndef FX_DIVISION_H
#define FX_DIVISION_H

#include <registers.h>
#include <types.h>

/**
 * @brief Performs synchronous fixed-point division.
 *
 * Initiates an asynchronous division operation and waits for completion,
 * returning the result. The division uses 32-bit fixed-point arithmetic
 * with appropriate rounding.
 *
 * @param numer Numerator value (32-bit fixed-point).
 * @param denom Denominator value (32-bit fixed-point).
 * @return The division result (32-bit fixed-point).
 */
s32 FX_Divide(s32 numer, s32 denom);

/**
 * @brief Performs synchronous 64-bit division result retrieval.
 *
 * Initiates an asynchronous division and returns the raw 64-bit result
 * without rounding. Useful for operations requiring full precision.
 *
 * @param numer Numerator value (32-bit).
 * @param denom Denominator value (32-bit).
 * @return The raw 64-bit division result.
 */
s64 FX_Divide64(s32 numer, s32 denom);

/**
 * @brief Calculates the multiplicative inverse of a fixed-point value.
 *
 * Computes 1/denom using the division hardware. Returns the result
 * in fixed-point format with rounding.
 *
 * @param denom The value to invert (32-bit fixed-point).
 * @return The inverse value (32-bit fixed-point).
 */
s32 FX_Inverse(s32 denom);

/**
 * @brief Calculates the square root of a fixed-point value.
 *
 * Computes the square root using the hardware square root unit.
 * Returns 0 for non-positive inputs.
 *
 * @param value The value to take the square root of (32-bit fixed-point).
 * @return The square root result (32-bit fixed-point), or 0 if value <= 0.
 */
s32 FX_Sqrt(s32 value);

/**
 * @brief Retrieves the raw 64-bit division result.
 *
 * Waits for any pending division operation to complete and returns
 * the unrounded 64-bit result from the division registers.
 *
 * @return The raw 64-bit division result.
 */
s64 FX_GetDivResultFx64c(void);

/**
 * @brief Retrieves the rounded 32-bit division result.
 *
 * Waits for division completion and returns the result rounded
 * to 32-bit fixed-point format (20-bit fractional precision).
 *
 * @return The rounded 32-bit division result.
 */
s32 FX_GetDivResult(void);

/**
 * @brief Initiates asynchronous inverse calculation.
 *
 * Starts the hardware division unit to compute the inverse of the
 * given denominator. The result can be retrieved later using
 * FX_GetDivResult().
 *
 * @param denom The denominator value to invert (32-bit).
 */
void FX_InvAsync(u32 denom);

/**
 * @brief Retrieves the rounded square root result.
 *
 * Waits for square root operation completion and returns the
 * result rounded to 32-bit fixed-point format.
 *
 * @return The rounded square root result (32-bit fixed-point).
 */
s32 FX_GetSqrtResult(void);

/**
 * @brief Initiates asynchronous division operation.
 *
 * Starts the hardware division unit to compute numer/denom.
 * The result can be retrieved later using FX_GetDivResult().
 *
 * @param numer Numerator value (32-bit).
 * @param denom Denominator value (32-bit).
 */
void FX_DivAsync(u32 numer, u32 denom);

/**
 * @brief Sets the numerator and denominator registers for 64-bit by 64-bit division.
 *
 * Configures the division hardware registers to prepare for a 64-bit numerator
 * divided by 64-bit denominator operation. This is a low-level function used
 * internally by other division functions.
 *
 * @param numer 64-bit numerator value.
 * @param denom 64-bit denominator value.
 */
static inline void SetDiv_Numer64Denom64(u64 numer, u64 denom) {
    REG_DIV_NUMER = numer;
    REG_DIV_DENOM = denom;
}

/**
 * @brief Sets the numerator and denominator registers for 64-bit by 32-bit division.
 *
 * Configures the division hardware registers for operations where the numerator
 * is 64-bit and the denominator is 32-bit. This is commonly used for fixed-point
 * division operations.
 *
 * @param numer 64-bit numerator value.
 * @param denom 32-bit denominator value.
 */
static inline void SetDiv_Numer64Denom32(u64 numer, u32 denom) {
    REG_DIV_NUMER = numer;
    REG_DIV_DENOM = denom;
}

#endif // FX_DIVISION_H