#ifndef COMBAT_CORE_COMBATMIC_H
#define COMBAT_CORE_COMBATMIC_H

#include <nitro/types.h>

typedef struct BtlMicInput BtlMicInput;

// Request consumed by the engine's asynchronous microphone sampler.
typedef struct BtlMicSampling {
    /* 0x00 */ u32 type;
    /* 0x04 */ s8* buffer;
    /* 0x08 */ u32 size;
    /* 0x0C */ u32 rate;
    /* 0x10 */ u32 loop;
    /* 0x14 */ void (*callback)(s32 result, BtlMicInput* input);
    /* 0x18 */ BtlMicInput* callbackArg;
} BtlMicSampling; // Size: 0x1C

struct BtlMicInput {
    /* 0x00 */ s8             samples[32];
    /* 0x20 */ BtlMicSampling sampling;
    /* 0x3C */ s32            detected;
    /* 0x40 */ u8             cooldown;
    /* 0x41 */ u8             pad_41[3];
}; // Size: 0x44

void func_ov003_020a50f4(s32 result, BtlMicInput* input);

#endif
