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
    u8                    pad8C[0xC0 - 0x8C];
    /* 0xC0 */ s16        unk_C0;
    /* 0xC2 */ s16        unk_C2;
    /* 0xC4 */ s16        unk_C4;
    u8                    padC6[0xC8 - 0xC6];
} Enm006SpriteBlock; // Size: 0xC8

/// The main `BtlEnm006` task data. Only offsets confirmed by the disassembly are named; the
/// gaps are explicit padding and the struct grows as more functions land.
/// Size: 0x1FC (from the Tsk_BtlEnm006_RG TaskHandle).
struct BtlEnm006 {
    u8                            pad00[0x24];
    /* 0x24 */ s32                unk_24; // non-zero mirrors the sprite horizontally
    /* 0x28 */ s32                unk_28; // position.x
    /* 0x2C */ s32                unk_2C; // position.y
    /* 0x30 */ s32                unk_30; // position.z
    u8                            pad34[0x38 - 0x34];
    /* 0x38 */ s32                unk_38;
    u8                            pad3C[0x80 - 0x3C];
    /* 0x80 */ u16                unk_80; // asset/variant index
    /* 0x82 */ u16                pad82;
    /* 0x84 */ u16                unk_84; // engine bits 0..1
    /* 0x86 */ u16                pad86;
    u8                            pad88[0x100 - 0x88];
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
    u8                            pad1E8[0x1FC - 0x1E8];
};

/// Arguments handed to the `Tsk_BtlEnm006_*` task constructors.
typedef struct Enm006Spawn {
    /* 0x00 */ BtlEnm006* unk_00;
    /* 0x04 */ void*      unk_04;
    /* 0x08 */ s32        unk_08;
    /* 0x0C */ s32        unk_0C;
} Enm006Spawn;

#endif /* COMBAT_NOISE_PRIVATE_BTLENM006_H */
