#include <NitroSDK/fx/fx_division.h>

s32 FX_Divide(s32 numer, s32 denom) {
    FX_DivAsync(numer, denom);
    return FX_GetDivResult();
}

s64 FX_Divide64(s32 numer, s32 denom) {
    FX_DivAsync(numer, denom);
    while (REG_DIV_CNT & 0x8000) {
    }
    return REG_DIV_RESULT;
}

s32 FX_Inverse(s32 denom) {
    FX_InvAsync(denom);
    return FX_GetDivResult();
}

s32 FX_Sqrt(s32 value) {
    if (value > 0) {
        REG_SQRT_CNT   = 1;
        REG_SQRT_PARAM = (u64)value << 32;
        return FX_GetSqrtResult();
    }
    return 0;
}

s64 FX_GetDivResultFx64c(void) {
    while (REG_DIV_CNT & 0x8000) {
    }
    return REG_DIV_RESULT;
}

s32 FX_GetDivResult(void) {
    while (REG_DIV_CNT & 0x8000) {
    }

    return (REG_DIV_RESULT + (1 << 19) >> 20);
}

void FX_InvAsync(u32 denom) {
    REG_DIV_CNT = 1;
    SetDiv_Numer64Denom32(0x1000LL << 32, denom);
}

s32 FX_GetSqrtResult(void) {
    while (REG_SQRT_CNT & 0x8000) {
    }
    return (REG_SQRT_RESULT + (1 << 9) >> 10);
}

void FX_DivAsync(u32 numer, u32 denom) {
    REG_DIV_CNT = 1;
    SetDiv_Numer64Denom32((u64)numer << 32, denom);
}