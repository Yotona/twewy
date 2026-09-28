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
    u8                    pad9A[0x9C - 0x9A];
    /* 0x9C */ s16        unk_9C;
    /* 0x9E */ s16        unk_9E;
    u8                    padA0[0xA2 - 0xA0];
    /* 0xA2 */ s16        unk_A2;
    u8                    padA4[0xAC - 0xA4];
    /* 0xAC */ s32        unk_AC; // the position the waypoint seek was launched from
    /* 0xB0 */ s32        unk_B0;
    /* 0xB4 */ s32        unk_B4;
    u8                    padB8[0xC0 - 0xB8];
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

/// A third overlapping view, for the s16 at 0x1F0 and the one at 0x1F2. It gets its own type
/// rather than a second field on `Enm006SpriteAlt`: with two fields MWCC places the second one
/// 0x38 bytes early. The pair here is *contiguous*, unlike that one, and `func_ov010_02128820`
/// reads both through a single `base+0x100` pointer, so they have to share a type.
typedef struct Enm006SpriteAlt2 {
    u8             pad00[0xF0];
    /* 0xF0 */ s16 unk_F0;
    /* 0xF2 */ s16 unk_F2;
} Enm006SpriteAlt2;

/// A fourth overlapping view, for the u16 at 0x1F6: `func_ov010_02128e80` reaches it as
/// base+0x100 then +0xF6, the same two-step form as the two views above. One field only,
/// for the reason given on `Enm006SpriteAlt2`.
typedef struct Enm006SpriteAlt3 {
    u8             pad00[0xF6];
    /* 0xF6 */ u16 unk_F6;
} Enm006SpriteAlt3;

/// A fifth overlapping view, for the pair of s16s at 0x1F6 / 0x1F8 that
/// `func_ov010_02127764` copies into each other. Reached as base+0x100 then the two halfword
/// offsets, so it needs its own type rather than a field on `BtlEnm006` -- whose own
/// `unk_1F8` is an s32 and would fold the address to a single `str r0, [r5, #0x1f8]`.
typedef struct Enm006SpriteAlt4 {
    u8             pad00[0xF6];
    /* 0xF6 */ u16 unk_F6;
    /* 0xF8 */ u16 unk_F8;
} Enm006SpriteAlt4;

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
    u8             pad6C[0x70 - 0x6C];
    /* 0x70 */ s16 unk_70; // 4.12 outline radius, scaled to 16.16 by func_ov010_0212847c
    u8             pad72[0x80 - 0x72];
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
    u8                            padC4[0xC8 - 0xC4];
    /* 0xC8 */ s16                unk_C8;
    u8                            padCA[0xD4 - 0xCA];
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
    u8                            pad1F0[0x1F4 - 0x1F0];
    /* 0x1F4 */ u8                unk_1F4; // bit 2: the waypoint reflection flag
    /* 0x1F5 */ u8                unk_1F5; // bit 0 mirrors the sprite
    u8                            pad1F6[0x1F8 - 0x1F6];
    /* 0x1F8 */ s32               unk_1F8; // result of the last follower spawn, or -1 for "none"
};

/// The `Tsk_BtlEnm006_Swirl` task data. This is a *different* struct from `BtlEnm006`: 0xA0 bytes
/// rather than 0x1FC, and a `s32[8]` at 0x80 where `BtlEnm006` has a single `u16`. Confusing the
/// two shifts everything, so it gets its own type. 0x04 holds a CombatSprite; it is left as
/// padding because its size is unknown and naming it would fix the offsets below.
typedef struct Enm006Swirl {
    /* 0x00 */ BtlEnm006* unk_00; // the owning instance
    u8                    pad04[0x64 - 0x04];
    /* 0x64 */ s16        unk_64; // one-shot "has fired" flag
    u8                    pad66[0x68 - 0x66];
    /* 0x68 */ s32        unk_68; // position.x, biased by 0x20000
    /* 0x6C */ s32        unk_6C; // position.y
    /* 0x70 */ s32        unk_70;
    /* 0x74 */ s32        unk_74; // accumulator, clamped to 0x4000
    /* 0x78 */ s32        unk_78; // wrap flag
    /* 0x7C */ u16        unk_7C; // angle, clamped to 0x200
    /* 0x7E */ s16        unk_7E;
    /* 0x80 */ s32        unk_80[8];
} Enm006Swirl; // Size: 0xA0 (from the Tsk_BtlEnm006_Swirl TaskHandle)

/// The `Tsk_BtlEnm006_Swlo` task data. 0x34 bytes. `unk_00` is a mode flag, not a pointer: it
/// selects which of `unk_08` / `unk_0C` the computed position is written through.
typedef struct Enm006Swlo {
    /* 0x00 */ void*      unk_00; // mode: non-zero selects unk_0C
    /* 0x04 */ void*      unk_04;
    /* 0x08 */ void*      unk_08;
    /* 0x0C */ BtlEnm006* unk_0C;
    /* 0x10 */ void*      unk_10;
    /* 0x14 */ s32        unk_14;
    /* 0x18 */ s32        unk_18;
    /* 0x1C */ s32        unk_1C;
    u8                    pad20[0x2C - 0x20];
    /* 0x2C */ u16        unk_2C; // clamped to 0xC000
    /* 0x2E */ u16        unk_2E;
    /* 0x30 */ u8         unk_30;
    u8                    pad31[0x34 - 0x31];
} Enm006Swlo; // Size: 0x34 (from the Tsk_BtlEnm006_Swlo TaskHandle)

/// The `Tsk_BtlEnm006_DeadEff` task data. 0x6C bytes, and the CombatSprite is at offset *zero*
/// here (the sibling constructors put theirs at 0x04). It is left as padding for the same reason
/// as everywhere else: its size is unknown and naming it would fix the offsets below.
typedef struct Enm006DeadEff {
    u8             pad00[0x60];
    /* 0x60 */ s32 unk_60; // position.x
    /* 0x64 */ s32 unk_64; // position.y
    /* 0x68 */ s32 unk_68; // position.z
} Enm006DeadEff;           // Size: 0x6C (from the Tsk_BtlEnm006_DeadEff TaskHandle)

/// Arguments handed to the `Tsk_BtlEnm006_*` task constructors.
typedef struct Enm006Spawn {
    /* 0x00 */ BtlEnm006* unk_00;
    /* 0x04 */ void*      unk_04;
    /* 0x08 */ s32        unk_08;
    /* 0x0C */ s32        unk_0C;
} Enm006Spawn;

#endif /* COMBAT_NOISE_PRIVATE_BTLENM006_H */
