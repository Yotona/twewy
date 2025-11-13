#include <NitroSDK/fx/fx_division.h>
#include <NitroSDK/fx/fx_vector.h>

void Vec_Add(const Vec* a, const Vec* b, Vec* dest) {
    dest->x = a->x + b->x;
    dest->y = a->y + b->y;
    dest->z = a->z + b->z;
}

void Vec_Sub(const Vec* a, const Vec* b, Vec* dest) {
    dest->x = a->x - b->x;
    dest->y = a->y - b->y;
    dest->z = a->z - b->z;
}

s32 Vec_DotProduct(const Vec* a, const Vec* b) {
    return ((s64)a->x * b->x + (s64)a->y * b->y + (s64)a->z * b->z + (1 << 11)) >> 12;
}

void Vec_CrossProduct(const Vec* a, const Vec* b, Vec* dest) {
    s32 x = (s64)a->y * b->z - (s64)a->z * b->y + (1 << 11) >> 12;
    s32 y = (s64)a->z * b->x - (s64)a->x * b->z + (1 << 11) >> 12;
    s32 z = (s64)a->x * b->y - (s64)a->y * b->x + (1 << 11) >> 12;

    dest->x = x;
    dest->y = y;
    dest->z = z;
}

s32 Vec_Magnitude(const Vec* v) {
    s64 temp = (s64)v->x * v->x;
    temp += (s64)v->y * v->y;
    temp += (s64)v->z * v->z;
    temp <<= 2;

    REG_SQRT_CNT   = 1;
    REG_SQRT_PARAM = temp;

    while (REG_SQRT_CNT & 0x8000) {
    }

    return ((s32)REG_SQRT_RESULT + 1) >> 1;
}

void Vec_Normalize(const Vec* src, Vec* dest) {
    s64 temp = (s64)src->x * src->x;
    temp += (s64)src->y * src->y;
    temp += (s64)src->z * src->z;

    REG_DIV_CNT = 2;
    SetDiv_Numer64Denom64(1LL << 56, temp);

    REG_SQRT_CNT   = 1;
    REG_SQRT_PARAM = (temp << 2);

    while (REG_SQRT_CNT & 0x8000) {
    }
    s32 sqrt = REG_SQRT_RESULT;

    while (REG_DIV_CNT & 0x8000) {
    }
    s64 div = REG_DIV_RESULT;

    div *= sqrt;

    dest->x = (div * src->x + (1LL << 44)) >> 45;
    dest->y = (div * src->y + (1LL << 44)) >> 45;
    dest->z = (div * src->z + (1LL << 44)) >> 45;
}

void Vec_MultAdd(s32 a, const Vec* v1, const Vec* v2, Vec* pDest) {
    pDest->x = v2->x + (s32)(((s64)a * v1->x) >> 12);
    pDest->y = v2->y + (s32)(((s64)a * v1->y) >> 12);
    pDest->z = v2->z + (s32)(((s64)a * v1->z) >> 12);
}

s32 Vec_Distance(const Vec* v1, const Vec* v2) {
    s64 temp;
    s32 diff;

    diff = v1->x - v2->x;
    temp = (s64)diff * diff;

    diff = v1->y - v2->y;
    temp += (s64)diff * diff;

    diff = v1->z - v2->z;
    temp += (s64)diff * diff;

    temp <<= 2;

    REG_SQRT_CNT   = 1;
    REG_SQRT_PARAM = temp;

    while (REG_SQRT_CNT & 0x8000) {
    }
    return ((s32)REG_SQRT_RESULT + 1) >> 1;
}