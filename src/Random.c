#include "Random.h"

u32 RNG_GetSeed(void);

static u16 RngSeed[2] = {0x917, 0x1972};

u32 RNG_SetSeed(u32 seed) {
    u32 oldSeed = RNG_GetSeed();

    if (seed == 0) {
        seed = 0x19720917;
    }

    RngSeed[0] = (u16)seed;
    RngSeed[1] = (u16)(((u32)seed & 0xFFFF0000) >> 16);

    return oldSeed;
}

u32 RNG_GetSeed(void) {
    return (RngSeed[1] << 0x10) | RngSeed[0];
}

u16 RNG_Next(u32 scale) {
    u32 high = RngSeed[1];
    u32 low  = RngSeed[0];

    u32 next = high + ((high << 18) >> 16);

    low = (next >> 16) + (low << 1);
    low += (high + (low >> 16));

    scale = (((low << 16) >> 16) * scale) >> 16;

    RngSeed[1] = (u16)next;
    RngSeed[0] = (u16)low;

    return scale;
}
