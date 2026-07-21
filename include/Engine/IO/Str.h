#ifndef STR_H
#define STR_H

/**
 * @file Str.h
 * @brief Minimal string helper functions (length, copy, validation, formatting).
 */

#include <nitro/types.h>

/**
 * @brief Computes the length of a null-terminated string.
 * @param str The string to measure.
 * @return The number of bytes before the null terminator.
 */
s32 Str_Len(const char* str);

/**
 * @brief Copies a null-terminated string, including its terminator.
 * @param dst Destination buffer, must be large enough to hold src.
 * @param src Null-terminated source string.
 * @return The length of src (excluding the null terminator).
 */
s32 Str_Copy(char* dst, const char* src);

/**
 * @brief Validates that a string is well-formed, single/double-byte
 * (Shift-JIS-style) safe, and null-terminated within a given length.
 *
 * @param str The string to validate.
 * @param maxLen Maximum number of bytes to scan before giving up, or 0 for no limit.
 * @return TRUE if a null terminator was found before maxLen was reached, FALSE otherwise.
 */
BOOL Str_Validate(const char* str, s32 maxLen);

/**
 * @brief Formats a string printf-style into a temporary heap buffer.
 *
 * @param fmt Format string, followed by its variadic arguments.
 * @return Pointer to the formatted string.
 * Returns a static error message if allocation failed or the result
 * would exceed the internal 0x400 byte buffer limit.
 */
char* Str_SPrintf(const char* fmt, ...);

#endif // STR_H
