#ifndef MSL_ARM_MATH_H
#define MSL_ARM_MATH_H

#include <types.h>

s32 __double_huge[];
#define HUGE_VAL (*(double*)__double_huge)

s32 __float_huge[];
#define INFINITY (*(float*)__float_huge)

s32 __float_nan[];
#define NAN (*(float*)__float_nan)

double fabs(double x);

double nan(const char* x);

static inline long double fabsl(long double x) {
    return (long double)fabs((double)x);
}

#endif // MSL_ARM_MATH_H