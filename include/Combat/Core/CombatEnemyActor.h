#ifndef COMBAT_CORE_COMBATENEMYACTOR_H
#define COMBAT_CORE_COMBATENEMYACTOR_H

#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatList.h"
#include "Combat/Core/CombatSprite.h"

typedef union Ov003CutinFlags {
    /* 0x0 */ u16 raw;
    struct {
        /* bit 0 */ u16       b0   : 1;
        /* bit 1 */ u16       b1   : 1;
        /* bit 2 */ u16       b2   : 1;
        /* bit 3 */ u16       b3   : 1;
        /* bit 4 */ u16       b4   : 1;
        /* bit 5 */ u16       b5   : 1;
        /* bit 6 */ u16       b6   : 1;
        /* bit 7 */ u16       b7   : 1;
        /* bit 8 */ u16       b8   : 1;
        /* bit 9 */ u16       b9   : 1;
        /* bit 10 */ u16      b10  : 1;
        /* bit 11 */ u16      b11  : 1;
        /* bits 12..15 */ u16 rest : 4;
    } bits;
} Ov003CutinFlags; // Size: 0x2

/// The battle actor every cut-in, enemy shot and noise enemy starts with: what `func_ov003_020c41e8` resets.
/// Types built on it open with an `Ov003EnemyActor base;` member, and `&x->base` is what the shared actor functions
/// take. Code that treats it as an attack target used to call it `Ov003Unit`.
typedef struct Ov003EnemyActor {
    /* 0x000 */ CombatActor             actor;
    /* 0x07C */ s32                     group; // index into data_ov003_020e70ac
    /* 0x080 */ u16                     spriteIndex;
    /* 0x082 */ u16                     id;    // running cut-in instance id
    /* 0x084 */ CombatSprite            sprite;
    /* 0x0E4 */ CombatSprite            subSprite;
    /* 0x144 */ Ov003Collider           collider;
    /* 0x178 */ CombatListNode          node;
    /* 0x184 */ struct Ov003EnemyActor* partner; // the actor this one is paired with (UG/RG twin, cut-in partner)
    /* 0x188 */ void*                   unk_188; // per-type pointer: a twin's shared state, a spawn list, ...
    /* 0x18C */ u16                     flags18C;
    /* 0x18E */ s16                     unk_18E;
    /* 0x190 */ s16                     unk_190;
    /* 0x192 */ s16                     unk_192;
    /* 0x194 */ s16                     unk_194;
    /* 0x196 */ Ov003CutinFlags         flags196; // { ...; hasPalette : 1 @0x100; ... }
    /* 0x198 */ s16                     unk_198;
    /* 0x19A */ s16                     unk_19A;
    /* 0x19C */ s16                     unk_19C;
    /* 0x19E */ s16                     unk_19E;
    /* 0x1A0 */ s16                     unk_1A0;
    /* 0x1A2 */ s16                     unk_1A2;
    /* 0x1A4 */ s16                     unk_1A4;
    /* 0x1A6 */ s16                     unk_1A6;
    /* 0x1A8 */ s16                     unk_1A8;
    /* 0x1AA */ s16                     unk_1AA;
    /* 0x1AC */ s32                     unk_1AC;
    /* 0x1B0 */ s32                     unk_1B0;
    /* 0x1B4 */ s32                     unk_1B4;
    /* 0x1B8 */ s32                     unk_1B8;
    /* 0x1BC */ s32                     unk_1BC;
} Ov003EnemyActor; // Size: 0x1C0

#endif             // COMBAT_CORE_COMBATENEMYACTOR_H
