#ifndef COMBAT_COMBATATTACK_H
#define COMBAT_COMBATATTACK_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatList.h"

typedef struct Ov003AttackInfo {
    /* 0x00 */ s16 unk_00; // base attack power
    /* 0x02 */ u8  unk_02; // reaction id, copied to `Ov003Global.unk3D906`/`unk3D90A`
    /* 0x03 */ u8  unk_03; // hit reaction, copied to the actor's `unk_10`
    /* 0x04 */ u8  unk_04; // hit stun, copied to the actor's `unk_5A`
    /* 0x05 */ u8  unk_05; // half width, pixels
    /* 0x06 */ u8  unk_06; // half height, pixels
    /* 0x07 */ u8  unk_07; // half depth, pixels
    /* 0x08 */ s32 flags;  // bits 1..2 blend mode, bits 3..4 variant, bits 15..18 attack kind
} Ov003AttackInfo;         // Size: 0xC

typedef union Ov003UnitStatus {
    u16 raw;
    struct {
        u16 unk_00 : 1;
        u16 unk_01 : 1;
        u16 unk_02 : 1;
        u16 unk_03 : 1;
        u16 unk_04 : 1;
        u16 unk_05 : 1;
        u16 unk_06 : 1;
        u16 unk_07 : 1;
        u16 unk_08 : 1;
        u16 unk_09 : 7;
    };
} Ov003UnitStatus;

typedef struct Ov003Unit {
    /* 0x000 */ CombatActor    actor;
    /* 0x07C */ s32            unk_07C;
    /* 0x080 */ s32            unk_080;
    /* 0x084 */ u8             unk_084[0xE4 - 0x084];
    /* 0x0E4 */ s32            unk_0E4; // engine index of this unit's visuals
    /* 0x0E8 */ u8             unk_0E8[0x178 - 0x0E8];
    /* 0x178 */ CombatListNode node;
    /* 0x184 */ u8             unk_184[0x192 - 0x184];
    /* 0x192 */ s16            unk_192;
    /* 0x194 */ s16            unk_194;
    /* 0x196 */ union {
        u16             unk_196;
        Ov003UnitStatus status;
    };
} Ov003Unit; // Size: 0x198

/// Hit counter, payload and configured maximum consecutive hits (0xFF = unlimited).
typedef struct Ov003AttackStreak {
    /* 0x00 */ u8              unk_00[0x8];
    /* 0x08 */ s16             hits;
    /* 0x0A */ u8              unk_0A[0x2];
    /* 0x0C */ Ov003AttackArgs args;
    /* 0x14 */ u8              maxHits;
    /* 0x15 */ u8              unk_15[3];
} Ov003AttackStreak; // Size: 0x18

// Applies the staged attack and returns its processing result after restoring X velocity.
s32 func_ov003_020a6b84(Ov003AttackStreak* streak, u16 attackId, Ov003Unit* unit);

#endif
