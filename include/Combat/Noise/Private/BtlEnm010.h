#ifndef COMBAT_NOISE_PRIVATE_BTLENM010_H
#define COMBAT_NOISE_PRIVATE_BTLENM010_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatEnemyActor.h"
#include "Combat/Core/CombatSprite.h"

typedef struct BtlEnm010RG {
    /* 0x000 */ Ov003EnemyActor base;
    /* 0x1C0 */ s16             unk_1C0;
    /* 0x1C2 */ s16             unk_1C2;
    /* 0x1C4 */ s16             unk_1C4;
    /* 0x1C6 */ s16             unk_1C6;
    /* 0x1C8 */ void (*unk_1C8)(struct BtlEnm010RG*);
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ s32 unk_1D0;
    /* 0x1D4 */ s32 unk_1D4;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s32 unk_1DC;
    /* 0x1E0 */ s32 unk_1E0;
    /* 0x1E4 */ s32 unk_1E4;
    /* 0x1E8 */ s32 unk_1E8;
    /* 0x1EC */ s32 unk_1EC;
    /* 0x1F0 */ s32 unk_1F0;
    /* 0x1F4 */ s16 unk_1F4;
    /* 0x1F6 */ s16 unk_1F6;
    /* 0x1F8 */ s16 unk_1F8;
    /* 0x1FA */ s16 unk_1FA;
    /* 0x1FC */ s32 unk_1FC;
    /* 0x200 */ s32 unk_200;
    /* 0x204 */ s32 unk_204;
} BtlEnm010RG; // Size: 0x208

typedef struct BtlEnm010UG {
    /* 0x000 */ Ov003EnemyActor base;
    /* 0x1C0 */ s16             unk_1C0;
    /* 0x1C2 */ s16             unk_1C2;
    /* 0x1C4 */ s16             unk_1C4;
    /* 0x1C6 */ s16             unk_1C6;
    /* 0x1C8 */ void (*unk_1C8)(struct BtlEnm010UG*);
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ s32 unk_1D0;
    /* 0x1D4 */ s32 unk_1D4;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s32 unk_1DC;
    /* 0x1E0 */ s32 unk_1E0;
    /* 0x1E4 */ s32 unk_1E4;
    /* 0x1E8 */ s32 unk_1E8;
    /* 0x1EC */ s32 unk_1EC;
    /* 0x1F0 */ s32 unk_1F0;
    /* 0x1F4 */ s16 unk_1F4[1]; // per-slot cooldowns, see func_ov011_0212bd3c
    /* 0x1F6 */ s16 unk_1F6;    // pattern step for func_ov011_02129f80
    /* 0x1F8 */ s16 unk_1F8;
    /* 0x1FA */ s16 unk_1FA;
    /* 0x1FC */ s16 unk_1FC;
    /* 0x1FE */ s16 unk_1FE;
    /* 0x200 */ s16 unk_200;
    /* 0x202 */ s16 unk_202;
    /* 0x204 */ s16 unk_204;
    /* 0x206 */ u8  unk_206;
    /* 0x207 */ u8  unk_207;
    /* 0x208 */ s32 unk_208; // task id of the last projectile
    /* 0x20C */ s32 unk_20C;
    /* 0x210 */ s32 unk_210;
} BtlEnm010UG; // Size: 0x214

/// One of `Tsk_BtlEnm010_Tatt`'s four 0x88-byte arm records.
typedef struct BtlEnm010TattRec {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s16          unk_60; // frame counter
    /* 0x62 */ s16          unk_62; // frame limit
    /* 0x64 */ s16          unk_64; // state
    /* 0x66 */ u16          unk_66;
    /* 0x68 */ s32          unk_68; // displacement from the Tatt origin
    /* 0x6C */ s32          unk_6C;
    /* 0x70 */ u16          unk_70; // heading
    /* 0x72 */ u16          unk_72; // reversed heading
    /* 0x74 */ s32          unk_74; // range
    /* 0x78 */ s32          unk_78; // affine scale
    /* 0x7C */ s32          unk_7C; // target displacement
    /* 0x80 */ s32          unk_80;
    /* 0x84 */ u16          unk_84; // flags: bit 0 side, 1 live, 2, 3 arc, 4 retract, 5 fire
    /* 0x86 */ u16          unk_86;
} BtlEnm010TattRec;                 // Size: 0x88

/// A two-halfword pair, for the aggregate copies whose reference shape is two `ldrh`/two
/// `strh` in address order (the scalar spelling reverses one side or the other).
typedef struct BtlEnm010Pair16 {
    u16 lo;
    u16 hi;
} BtlEnm010Pair16;

typedef struct BtlEnm010Tatt {
    /* 0x000 */ Ov003EnemyActor* unk_000; // the RG or UG that spawned it
    /* 0x004 */ BtlEnm010TattRec rec[4];
    /* 0x224 */ void (*unk_224)(struct BtlEnm010Tatt*);
    /* 0x228 */ s16             unk_228;
    /* 0x22A */ s16             unk_22A;
    /* 0x22C */ s16             unk_22C;
    /* 0x22E */ u16             unk_22E;
    /* 0x230 */ s32             unk_230; // owner position at spawn
    /* 0x234 */ s32             unk_234;
    /* 0x238 */ s32             unk_238;
    /* 0x23C */ s32             unk_23C; // screen origin, 4.12
    /* 0x240 */ s32             unk_240;
    /* 0x244 */ BtlEnm010Pair16 unk_244; // copy of the owner's actor.unk_04/unk_06
    /* 0x248 */ BtlEnm010Pair16 unk_248; // copy of the owner's actor.unk_08/unk_0A
    /* 0x24C */ u8              unk_24C; // bit 0 mirrored, bit 1 sub engine
    /* 0x24D */ u8              unk_24D[3];
} BtlEnm010Tatt;                         // Size: 0x250

void func_ov011_021256c0(u16 arg0);
void func_ov011_02125714(void);
void func_ov011_02125750(s32 arg0, CombatSprite* arg1, s32 arg2);
void func_ov011_021258b4(s32 arg0, CombatSprite* arg1, s32 arg2);
s32  func_ov011_02125b98(Ov003EnemyActor* owner, s32 arg1);
s32  func_ov011_02127c84(Ov003EnemyActor* owner);
s32  func_ov011_02127f6c(Ov003EnemyActor* owner, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u16 arg5, u16 arg6, s32 arg7,
                         s16 arg8);
s32  func_ov011_021282b8(Ov003EnemyActor* owner, s32 arg1, s32 arg2, s32 arg3, u16 arg4, s32 arg5, s16 arg6);
s32  func_ov011_02128718(Ov003EnemyActor* owner);
s32  func_ov011_02128c44(Ov003EnemyActor* owner);

#endif /* COMBAT_NOISE_PRIVATE_BTLENM010_H */