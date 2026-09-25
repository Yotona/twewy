#ifndef COMBAT_FUSION_BTLFUSION_H
#define COMBAT_FUSION_BTLFUSION_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"

/// Task data for `Tsk_BtlPlayerLast`.
typedef struct BtlPlayerLast {
    /* 0x00 */ CombatActor actor;
    /* 0x7C */ void (*unk_7C)(struct BtlPlayerLast*);
    /* 0x80 */ s16 unk_80;
    /* 0x82 */ s16 unk_82;
} BtlPlayerLast; // Size: 0x84

/// A pooled aura sprite, linked through the trailing node.
typedef struct BtlAuraSprite {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ u8           node[0xC];
} BtlAuraSprite; // Size: 0x6C

/// Task data for `Tsk_BtlAuraLast`.
typedef struct BtlAuraLast {
    /* 0x000 */ CombatSprite  unk_000;
    /* 0x060 */ CombatSprite  unk_060;
    /* 0x0C0 */ CombatSprite  unk_0C0;
    /* 0x120 */ u8            unk_120[0x8];
    /* 0x128 */ u8            unk_128[0x8];
    /* 0x130 */ BtlAuraSprite sprites[16];
    /* 0x7F0 */ s16           unk_7F0;
    /* 0x7F2 */ s16           unk_7F2;
    /* 0x7F4 */ void (*unk_7F4)(struct BtlAuraLast*);
    /* 0x7F8 */ s16 unk_7F8;
    /* 0x7FA */ s16 unk_7FA;
    /* 0x7FC */ s32 unk_7FC;
    /* 0x800 */ s32 unk_800;
    /* 0x804 */ s32 unk_804;
    /* 0x808 */ s32 unk_808;
    /* 0x80C */ s16 unk_80C;
    /* 0x80E */ s16 unk_80E;
} BtlAuraLast; // Size: 0x810

extern const TaskHandle Tsk_BtlPlayerLast;
extern const TaskHandle Tsk_BtlAuraLast;

#endif // COMBAT_FUSION_BTLFUSION_H
