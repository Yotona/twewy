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

/// A sub-struct starting at 0x100 inside the task data. Only the halfword fields the
/// decompiled functions clear are named. The field offsets have to be 0xC0/0xC4 (not
/// 0x1C0/0x1C4) because the original reaches them as base+0x100 then +0xC0/+0xC4, and
/// that fold is what the codegen depends on.
typedef struct Enm006SpriteBlock {
    u8             pad[0xC0];
    /* 0xC0 */ u16 unk_C0;
    /* 0xC2 */ u16 unk_C2;
    /* 0xC4 */ u16 unk_C4;
} Enm006SpriteBlock; // Size: 0xC6

/// The main `BtlEnm006` task data. Only offsets confirmed by the disassembly are named;
/// the gaps are explicit padding and the struct grows as more functions land.
/// Size: 0x1FC (from the Tsk_BtlEnm006_RG TaskHandle).
struct BtlEnm006 {
    u8                            pad00[0x80];
    /* 0x80 */ u16                unk_80; // asset/variant index
    /* 0x82 */ u16                pad82;
    /* 0x84 */ u16                unk_84; // engine bits 0..1
    /* 0x86 */ u16                pad86;
    u8                            pad88[0x100 - 0x88];
    /* 0x100 */ Enm006SpriteBlock sprite;
    u8                            pad1C6[0x1C8 - 0x1C6];
    /* 0x1C8 */ void*             unk_1C8;
    /* 0x1CC */ s32               unk_1CC;
    u8                            pad1D0[0x1FC - 0x1D0];
};

/// Arguments handed to the `Tsk_BtlEnm006_*` task constructors.
typedef struct Enm006Spawn {
    /* 0x00 */ BtlEnm006* unk_00;
    /* 0x04 */ void*      unk_04;
    /* 0x08 */ s32        unk_08;
    /* 0x0C */ s32        unk_0C;
} Enm006Spawn;

#endif /* COMBAT_NOISE_PRIVATE_BTLENM006_H */
