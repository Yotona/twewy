#ifndef COMBAT_CORE_COMBATACTORPARAMS_H
#define COMBAT_CORE_COMBATACTORPARAMS_H

#include <nitro/types.h>

typedef struct Ov003ActorSpriteParams {
    union {
        u16 unk_00[0x1E];
        struct {
            /* 0x00 */ u16 reserved[0x16];
            /* 0x2C */ u16 dropItems[4];
            /* 0x34 */ s16 dropRates[4];
        };
    };
} Ov003ActorSpriteParams; // Size: 0x3C

extern const Ov003ActorSpriteParams* const* data_ov003_020e70ac[61];

#endif
