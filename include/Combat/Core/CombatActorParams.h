#ifndef COMBAT_CORE_COMBATACTORPARAMS_H
#define COMBAT_CORE_COMBATACTORPARAMS_H

#include "Combat/Core/CombatEnemyActor.h"
#include <nitro/types.h>

/// Per-actor battle parameters, `data_ov003_020e70ac[group][spriteIndex]`: stats, hit box and drop table.
/// The data tables fill it through `raw`.
typedef struct Ov003ActorParams {
    union {
        u16 raw[0x1E];
        struct {
            /* 0x00 */ u8              unk_00;
            /* 0x01 */ u8              unk_01;
            /* 0x02 */ u8              unk_02;
            /* 0x03 */ u8              unk_03;
            /* 0x04 */ u8              unk_04;
            /* 0x05 */ s8              unk_05;
            /* 0x06 */ s8              unk_06;
            /* 0x07 */ u8              unk_07;
            /* 0x08 */ u16             unk_08;
            /* 0x0A */ s16             unk_0A;
            /* 0x0C */ s16             unk_0C;
            /* 0x0E */ s16             unk_0E;
            /* 0x10 */ s16             unk_10;
            /* 0x12 */ s16             unk_12;
            /* 0x14 */ u16             unk_14;
            /* 0x16 */ Ov003CutinFlags flags16;
            /* 0x18 */ s8              unk_18;
            /* 0x19 */ s8              unk_19;
            /* 0x1A */ u8              unk_1A[0x1C - 0x1A];
            /* 0x1C */ s16             unk_1C;
            /* 0x1E */ s16             unk_1E;
            /* 0x20 */ s16             unk_20;
            /* 0x22 */ s16             unk_22;
            /* 0x24 */ s16             unk_24;
            /* 0x26 */ s16             unk_26;
            /* 0x28 */ u16             unk_28;
            /* 0x2A */ u16             unk_2A;
            /* 0x2C */ u16             dropItems[4];
            /* 0x34 */ s16             dropRates[4];
        };
    };
} Ov003ActorParams; // Size: 0x3C

extern const Ov003ActorParams* const* data_ov003_020e70ac[61];

#endif
