#ifndef COMBAT_NOISE_PRIVATE_BTLENM006_H
#define COMBAT_NOISE_PRIVATE_BTLENM006_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"

/// One of the `Tsk_BtlEnm006` asset variants.
typedef struct Enm006Variant {
    /* 0x00 */ BinIdentifier*   binIden;
    /* 0x04 */ SpriteAnimEntry* animTable;
    /* 0x08 */ u16              unk_08;
    /* 0x0A */ u16              unk_0A;
    /* 0x0C */ u16              unk_0C;
    /* 0x0E */ u16              unk_0E;
} Enm006Variant; // Size: 0x10

typedef struct BtlEnm006 BtlEnm006;

/// A sub-struct starting at 0x100 inside the task data. The offsets below have to be
/// relative to this base (0x84/0x88/0xC0/0xC2/0xC4, not 0x184/0x188/0x1C0/...) because the
/// original reaches the 0xC0 group as base+0x100 then +0xC0/+0xC4, and that fold is what the
/// codegen depends on.
typedef struct Enm006SpriteBlock {
    u8                    pad00[0x84];
    /* 0x84 */ BtlEnm006* twin; // the paired instance, as in the sibling overlays
    /* 0x88 */ void*      unk_88;
    /* 0x8C */ u16        unk_8C;
    u8                    pad8E[0x98 - 0x8E];
    /* 0x98 */ s16        unk_98;
    /* 0x9C */ s16        unk_9C;
    u8                    pad9A[0xC0 - 0x9A];
    /* 0xC0 */ s16        unk_C0;
    /* 0xC2 */ s16        unk_C2;
    /* 0xC4 */ s16        unk_C4;
    u8                    padC6[0xC8 - 0xC6];
} Enm006SpriteBlock; // Size: 0xC8

/// A second, *overlapping* view of the same 0x100 base. `func_ov010_02128e0c` reaches the s16 at
/// 0x1E8 as base+0x100 then +0xE8, a different addressing form from the outer struct's own
/// 0x1DC accesses. One memory range, two views -- so it needs its own type rather than a field
/// on `BtlEnm006`.
typedef struct Enm006SpriteAlt {
    u8             pad00[0xE8];
    /* 0xE8 */ u16 unk_E8;
} Enm006SpriteAlt;

/// A third overlapping view, for the s16 at 0x1F0. It gets its own type rather than a second
/// field on `Enm006SpriteAlt`: with two fields MWCC places the second one 0x38 bytes early.
typedef struct Enm006SpriteAlt2 {
    u8             pad00[0xF0];
    /* 0xF0 */ s16 unk_F0;
} Enm006SpriteAlt2;

/// The main `BtlEnm006` task data. Only offsets confirmed by the disassembly are named; the
/// gaps are explicit padding and the struct grows as more functions land.
/// Size: 0x1FC (from the Tsk_BtlEnm006_RG TaskHandle).
struct BtlEnm006 {
    u8             pad00[0x8];
    /* 0x8 */ s16  unk_8;  // 4.12 fixed point
    u8             pad0A[0x24 - 0xA];
    /* 0x24 */ s32 unk_24; // non-zero mirrors the sprite horizontally
    /* 0x28 */ s32 unk_28; // position.x
    /* 0x2C */ s32 unk_2C; // position.y
    /* 0x30 */ s32 unk_30; // position.z
    u8             pad34[0x38 - 0x34];
    /* 0x38 */ s32 unk_38;
    u8             pad3C[0x40 - 0x3C];
    /* 0x40 */ s32 unk_40; // 4.12 fixed point
    u8             pad44[0x54 - 0x44];
    /* 0x54 */ s32 unk_54; // engine bits
    u8             pad58[0x60 - 0x58];
    /* 0x60 */ s32 unk_60;
    /* 0x64 */ s32 unk_64;
    /* 0x68 */ s32 unk_68;
    u8             pad6C[0x80 - 0x6C];
    /* 0x80 */ u16 unk_80; // asset/variant index
    /* 0x82 */ u16 pad82;
    // 0x84 holds a CombatSprite; it is not named as a field because its size is unknown and
    // naming it would fix the padding below. The three functions that touch it cast instead.
    u8                            pad84[0x8C - 0x84];
    /* 0x8C */ s16                unk_8C;
    u8                            pad8E[0x9A - 0x8E];
    /* 0x9A */ s16                unk_9A;
    u8                            pad9C[0xB4 - 0x9C];
    /* 0xB4 */ s32                unk_B4;
    u8                            padB8[0xC0 - 0xB8];
    /* 0xC0 */ s32                unk_C0;
    u8                            padC4[0xD4 - 0xC4];
    /* 0xD4 */ s32                unk_D4;
    /* 0xD8 */ s32                unk_D8;
    u8                            padDC[0x100 - 0xDC];
    /* 0x100 */ Enm006SpriteBlock sprite; // 0xC8 bytes, so it ends exactly on 0x1C8
    u8                            pad1C8[0x1C8 - 0x1C8];
    /* 0x1C8 */ void*             unk_1C8;
    /* 0x1CC */ s32               unk_1CC;
    /* 0x1D0 */ s32               unk_1D0;
    /* 0x1D4 */ s32               unk_1D4;
    /* 0x1D8 */ s32               unk_1D8;
    /* 0x1DC */ s32               unk_1DC;
    /* 0x1E0 */ s32               unk_1E0;
    /* 0x1E4 */ s32               unk_1E4;
    u8                            pad1E8[0x1EC - 0x1E8];
    /* 0x1EC */ s32               unk_1EC;
    u8                            pad1F0[0x1F5 - 0x1F0];
    /* 0x1F5 */ u8                unk_1F5; // bit 0 mirrors the sprite
    u8                            pad1F6[0x1FC - 0x1F6];
};

/// Arguments handed to the `Tsk_BtlEnm006_*` task constructors.
typedef struct Enm006Spawn {
    /* 0x00 */ BtlEnm006* unk_00;
    /* 0x04 */ void*      unk_04;
    /* 0x08 */ s32        unk_08;
    /* 0x0C */ s32        unk_0C;
} Enm006Spawn;

#endif /* COMBAT_NOISE_PRIVATE_BTLENM006_H */
