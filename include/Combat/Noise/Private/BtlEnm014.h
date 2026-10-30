#ifndef COMBAT_NOISE_PRIVATE_BTLENM014_H
#define COMBAT_NOISE_PRIVATE_BTLENM014_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatEnemyActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"

typedef struct Enm014Spawn {
    /* 0x00 */ struct BtlEnm014* unk_00;
    /* 0x04 */ s32               unk_04;
    /* 0x08 */ u16               unk_08;
    /* 0x0A */ u16               unk_0A;
} Enm014Spawn; // Size: 0x0C

typedef union Enm014Chase {
    s32 word;
    struct {
        s16 lo;
        s16 hi;
    } half;
} Enm014Chase; // Size: 0x4

typedef struct BtlEnm014 {
    /* 0x000 */ Ov003EnemyActor base;
    /* 0x1C0 */ s16             unk_1C0;
    /* 0x1C2 */ s16             unk_1C2;
    /* 0x1C4 */ void (*unk_1C4)(struct BtlEnm014*);
    /* 0x1C8 */ void (*unk_1C8)(struct BtlEnm014*);
    /* 0x1CC */ s32         unk_1CC;
    /* 0x1D0 */ s32         unk_1D0;
    /* 0x1D4 */ s16         unk_1D4;
    /* 0x1D6 */ s16         unk_1D6;
    /* 0x1D8 */ s32         unk_1D8;
    /* 0x1DC */ s32         unk_1DC;
    /* 0x1E0 */ Enm014Chase unk_1E0;
    /* 0x1E4 */ s32         unk_1E4;
    /* 0x1E8 */ s32         unk_1E8;
    /* 0x1EC */ s32         unk_1EC;
} BtlEnm014; // Size: 0x1F0

extern const TaskHandle Tsk_BtlEnm014_Eff;

BinIdentifier*         func_ov012_021256c0(s32);
const SpriteAnimEntry* func_ov012_021256d0(void);
u16                    func_ov012_021256dc(s32);
void                   func_ov012_021256f0(s32, BtlEnm014*, s32);
void                   func_ov012_02125768(BtlEnm014*, void (*)(BtlEnm014*));
void                   func_ov012_02125790(s32, BtlEnm014*, void*, void (*)(BtlEnm014*));
void                   func_ov012_021257c4(BtlEnm014*);
void                   func_ov012_021257e8(BtlEnm014*);
void                   func_ov012_02125810(BtlEnm014*);
void                   func_ov012_0212582c(BtlEnm014*);
void                   func_ov012_02125858(BtlEnm014*);
void                   func_ov012_021258e8(void);
void                   func_ov012_02125958(BtlEnm014*);

#endif // COMBAT_NOISE_PRIVATE_BTLENM014_H
