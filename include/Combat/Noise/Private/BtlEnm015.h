#ifndef COMBAT_NOISE_PRIVATE_BTLENM015_H
#define COMBAT_NOISE_PRIVATE_BTLENM015_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"

/// One of the `Tsk_BtlEnm015` asset variants.
typedef struct Enm015Variant {
    /* 0x00 */ BinIdentifier*   binIden;
    /* 0x04 */ SpriteAnimEntry* animTable;
    /* 0x08 */ u8               unk_08;
    /* 0x09 */ u8               unk_09;
    /* 0x0A */ u16              unk_0A;
    /* 0x0C */ u32              unk_0C;
} Enm015Variant; // Size: 0x10

/// Spawn parameters passed to the `Tsk_BtlEnm015` tasks.
typedef struct Enm015Spawn {
    /* 0x00 */ void* unk_00;
    /* 0x04 */ u16   unk_04;
    /* 0x06 */ u16   unk_06;
    /* 0x08 */ s32   unk_08;
    /* 0x0C */ s32   unk_0C;
    /* 0x10 */ s32   unk_10;
    /* 0x14 */ u16   unk_14;
    /* 0x16 */ u16   unk_16;
} Enm015Spawn; // Size: 0x18

/// Task data for the `Tsk_BtlEnm015_RG` / `Tsk_BtlEnm015_UG` tasks.
typedef struct BtlEnm015 {
    /* 0x000 */ CombatActor  actor;
    /* 0x07C */ u8           unk_07C[0x4];
    /* 0x080 */ u16          unk_080;
    /* 0x082 */ u16          unk_082;
    /* 0x084 */ CombatSprite unk_084;
    /* 0x0E4 */ u8           unk_0E4[0x60];
    /* 0x144 */ s32          unk_144;
    /* 0x148 */ s32          unk_148;
    /* 0x14C */ s32          unk_14C;
    /* 0x150 */ s32          unk_150;
    /* 0x154 */ u8           unk_154[0x38];
    /* 0x18C */ u16          unk_18C;
    /* 0x18E */ u8           unk_18E[0xA];
    /* 0x198 */ s16          unk_198;
    /* 0x19A */ s16          unk_19A;
    /* 0x19C */ s16          unk_19C;
    /* 0x19E */ s16          unk_19E;
    /* 0x1A0 */ s16          unk_1A0;
    /* 0x1A2 */ s16          unk_1A2;
    /* 0x1A4 */ u8           unk_1A4[0x8];
    /* 0x1AC */ s32          unk_1AC;
    /* 0x1B0 */ s32          unk_1B0;
    /* 0x1B4 */ s32          unk_1B4;
    /* 0x1B8 */ u8           unk_1B8[0x8];
    /* 0x1C0 */ s16          unk_1C0;
    /* 0x1C2 */ s16          unk_1C2;
    /* 0x1C4 */ void (*unk_1C4)(struct BtlEnm015*);
    /* 0x1C8 */ s32 unk_1C8;
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ s16 unk_1D0;
    /* 0x1D2 */ s16 unk_1D2;
    /* 0x1D4 */ s16 unk_1D4;
    /* 0x1D6 */ s16 unk_1D6;
    /* 0x1D8 */ s16 unk_1D8;
    /* 0x1DA */ s16 unk_1DA;
    /* 0x1DC */ s32 unk_1DC;
    /* 0x1E0 */ s32 unk_1E0;
    /* 0x1E4 */ s32 unk_1E4;
    /* 0x1E8 */ u8  unk_1E8[0x4];
} BtlEnm015; // Size: 0x1EC

/// Task data for the `Tsk_BtlEnm015_Eff` task.
typedef struct BtlEnm015Eff {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s32          unk_60;
    /* 0x64 */ s32          unk_64;
    /* 0x68 */ s32          unk_68;
    /* 0x6C */ BtlEnm015*   unk_6C;
} BtlEnm015Eff; // Size: 0x70

/// Task data for the `Tsk_BtlEnm015_EffStamp` task.
typedef struct BtlEnm015Stamp {
    /* 0x00 */ BtlEnm015* unk_00;
    /* 0x04 */ s32        unk_04;
    /* 0x08 */ u16        unk_08;
} BtlEnm015Stamp; // Size: 0x0C

extern Enm015Variant* data_ov013_02127540[3];
extern s32            data_ov013_02127640;
extern s32            data_ov013_02127644;

extern const TaskHandle Tsk_BtlEnm015_Eff;
extern const TaskHandle Tsk_BtlEnm015_EffStamp;
extern const TaskHandle Tsk_BtlEnm015_EffStampSub;
extern const TaskHandle Tsk_BtlEnm015_RG;
extern const TaskHandle Tsk_BtlEnm015_Shake;
extern const TaskHandle Tsk_BtlEnm015_UG;

#endif // COMBAT_NOISE_PRIVATE_BTLENM015_H
