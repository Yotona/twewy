#ifndef COMBAT_NOISE_PRIVATE_BTLENM026_H
#define COMBAT_NOISE_PRIVATE_BTLENM026_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"

/// One of the `Tsk_BtlEnm026` asset variants.
typedef struct Enm026Variant {
    /* 0x00 */ BinIdentifier*   binIden;
    /* 0x04 */ SpriteAnimEntry* animTable;
    /* 0x08 */ u16              unk_08;
    /* 0x0A */ u16              unk_0A;
    /* 0x0C */ u32              unk_0C;
} Enm026Variant; // Size: 0x10

/// Lookup entry mapping an enemy id to a `Tsk_BtlEnm026` variant index.
typedef struct Enm026IdPair {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} Enm026IdPair;

/// Spawn parameters passed to the `Tsk_BtlEnm026_RG` / `Tsk_BtlEnm026_UG` tasks.
typedef struct Enm026Spawn {
    /* 0x00 */ void* unk_00;
    /* 0x04 */ u16   unk_04;
    /* 0x06 */ u16   unk_06;
    /* 0x08 */ s32   unk_08;
    /* 0x0C */ s32   unk_0C;
    /* 0x10 */ s32   unk_10;
    /* 0x14 */ u16   unk_14;
    /* 0x16 */ u16   unk_16;
} Enm026Spawn; // Size: 0x18

/// Spawn parameters passed to the `Tsk_BtlEnm026_Icon` task.
typedef struct Enm026IconSpawn {
    /* 0x00 */ struct BtlEnm026* unk_00;
    /* 0x04 */ s16               unk_04;
    /* 0x06 */ u16               unk_06;
    /* 0x08 */ void*             unk_08;
} Enm026IconSpawn; // Size: 0x0C

/// Per-particle spawn parameters consumed by `func_ov003_020c3cec`.
typedef struct Enm026ParticleParams {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
} Enm026ParticleParams; // Size: 0x14

/// Task data for the `Tsk_BtlEnm026_RG` / `Tsk_BtlEnm026_UG` tasks.
typedef struct BtlEnm026 {
    /* 0x000 */ CombatActor       actor;
    /* 0x07C */ u32               unk_07C;
    /* 0x080 */ u16               unk_080;
    /* 0x082 */ u16               unk_082;
    /* 0x084 */ CombatSprite      unk_084;
    /* 0x0E4 */ u8                unk_0E4[0x60];
    /* 0x144 */ s32               unk_144;
    /* 0x148 */ s32               unk_148;
    /* 0x14C */ s32               unk_14C;
    /* 0x150 */ s32               unk_150;
    /* 0x154 */ u8                unk_154[0x30];
    /* 0x184 */ struct BtlEnm026* unk_184;
    /* 0x188 */ struct BtlEnm026* unk_188;
    /* 0x18C */ u16               unk_18C;
    /* 0x18E */ u8                unk_18E[0x8];
    /* 0x196 */ u16               unk_196;
    /* 0x198 */ s16               unk_198;
    /* 0x19A */ s16               unk_19A;
    /* 0x19C */ s16               unk_19C;
    /* 0x19E */ s16               unk_19E;
    /* 0x1A0 */ s16               unk_1A0;
    /* 0x1A2 */ s16               unk_1A2;
    /* 0x1A4 */ u8                unk_1A4[0x4];
    /* 0x1A8 */ s16               unk_1A8;
    /* 0x1AA */ u16               unk_1AA;
    /* 0x1AC */ s32               unk_1AC;
    /* 0x1B0 */ s32               unk_1B0;
    /* 0x1B4 */ s32               unk_1B4;
    /* 0x1B8 */ u8                unk_1B8[0x8];
    /* 0x1C0 */ s16               unk_1C0;
    /* 0x1C2 */ s16               unk_1C2;
    /* 0x1C4 */ void (*unk_1C4)(struct BtlEnm026*);
    /* 0x1C8 */ s32 unk_1C8;
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ s32 unk_1D0;
    /* 0x1D4 */ s16 unk_1D4;
    /* 0x1D6 */ s16 unk_1D6;
    /* 0x1D8 */ s16 unk_1D8;
    /* 0x1DA */ s16 unk_1DA;
    /* 0x1DC */ s16 unk_1DC;
    /* 0x1DE */ u16 unk_1DE;
    /* 0x1E0 */ s32 unk_1E0;
    /* 0x1E4 */ s32 unk_1E4;
    /* 0x1E8 */ s32 unk_1E8;
    /* 0x1EC */ s32 unk_1EC;
    /* 0x1F0 */ u16 unk_1F0;
} BtlEnm026; // Size: 0x1F4

/// Pair of per-variant state callbacks (`NULL` when unused). The table also
/// holds argument-less stubs, so the members use unspecified parameters.
typedef struct Enm026StateFns {
    /* 0x00 */ void (*unk_00)();
    /* 0x04 */ void (*unk_04)();
} Enm026StateFns;

/// Task data for the `Tsk_BtlEnm026_Icon` task.
typedef struct BtlEnm026Icon {
    /* 0x000 */ CombatSprite unk_00;
    /* 0x060 */ CombatSprite unk_60;
    /* 0x0C0 */ CombatSprite unk_C0;
    /* 0x120 */ BtlEnm026*   unk_120;
    /* 0x124 */ s16          unk_124;
    /* 0x126 */ u16          unk_126;
    /* 0x128 */ void*        unk_128;
    /* 0x12C */ u16          unk_12C;
    /* 0x12E */ u16          unk_12E;
    /* 0x130 */ s32          unk_130;
} BtlEnm026Icon; // Size: 0x134

/// Shared spawn/defeat state for the `Tsk_BtlEnm026` tasks.
typedef struct Enm026State {
    /* 0x00 */ s32 initialized;
    /* 0x04 */ s32 variant;
    /* 0x08 */ s32 flag08;
    /* 0x0C */ s8  counter0C;
    /* 0x0D */ u8  pad_0D[0x3];
    /* 0x10 */ s32 result10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 timer18;
    /* 0x1A */ s16 seconds1A;
    /* 0x1C */ s16 counter1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ u16 unk_28;
    /* 0x2A */ u8  pad_2A[0x16];
} Enm026State; // Size: 0x40

#endif         // COMBAT_NOISE_PRIVATE_BTLENM026_H
