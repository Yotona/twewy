#ifndef RECT_H
#define RECT_H

/**
 * @file Rect.h
 * @brief Rectangle intersection, hit-testing, and union helpers.
 */

#include <nitro/types.h>

/**
 * @brief Computes the intersection of two rectangles in {x, y, width, height} format.
 * @param out Output rectangle (x, y, width, height) receiving the overlapping region.
 * @param a First input rectangle (x, y, width, height).
 * @param b Second input rectangle (x, y, width, height).
 * @return TRUE if the rectangles overlap (the intersection has positive width and height), FALSE otherwise.
 *
 * @details
 * Returns FALSE without writing to `out` if either rectangle has a
 * non-positive width or height. Otherwise clips `a` against `b` and writes
 * the resulting intersection to `out`, also in {x, y, width, height} format.
 */
BOOL Rect_Intersect(s32* out, const s32* a, const s32* b);

/**
 * @brief Tests whether a point lies within a rectangle.
 * @param rect Rectangle in {left, top, right, bottom} format.
 * @param x X coordinate of the point to test.
 * @param y Y coordinate of the point to test.
 * @return TRUE if (x, y) lies within [left, right] x [top, bottom], FALSE otherwise.
 */
BOOL Rect_ContainsPoint(const s32* rect, s32 x, s32 y);

/**
 * @brief Computes the smallest rectangle that contains both input rectangles.
 * @param out Output rectangle (left, top, right, bottom) receiving the union.
 * @param a First input rectangle (left, top, right, bottom).
 * @param b Second input rectangle (left, top, right, bottom).
 *
 * @details
 * Each output bound is the min (left/top) or max (right/bottom) of the
 * corresponding bounds of `a` and `b`.
 */
void Rect_Union(s32* out, const s32* a, const s32* b);

#endif // RECT_H
