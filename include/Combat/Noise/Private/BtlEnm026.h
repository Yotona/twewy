#ifndef COMBAT_NOISE_PRIVATE_BTLENM026_H
#define COMBAT_NOISE_PRIVATE_BTLENM026_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatEnemyActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"

typedef struct Enm026IconSpawn {
    /* 0x00 */ struct BtlEnm026* unk_00;
    /* 0x04 */ s16               unk_04;
    /* 0x06 */ u16               unk_06;
    /* 0x08 */ void*             unk_08;
} Enm026IconSpawn; // Size: 0x0C

typedef struct BtlEnm026 {
    /* 0x000 */ union {
        Ov003EnemyActor base;
        Ov003EnemyActor;
    };
    /* 0x1C0 */ s16 unk_1C0;
    /* 0x1C2 */ s16 unk_1C2;
    /* 0x1C4 */ void (*unk_1C4)(struct BtlEnm026*);
    /* 0x1C8 */ s32 unk_1C8;
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ s32 unk_1D0;
    /* 0x1D4 */ s16 unk_1D4;
    /* 0x1D6 */ s16 unk_1D6;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s16 unk_1DC;
    /* 0x1DE */ u16 unk_1DE;
    /* 0x1E0 */ s32 unk_1E0;
    /* 0x1E4 */ s32 unk_1E4;
    /* 0x1E8 */ s32 unk_1E8;
    /* 0x1EC */ s32 unk_1EC;
    /* 0x1F0 */ u16 unk_1F0;
} BtlEnm026; // Size: 0x1F4

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

void                    func_ov015_02125a64(BtlEnm026*, s32);
void                    func_ov015_02125a8c(s32, BtlEnm026*, void*, s32);
s32                     func_ov015_02125b08(BtlEnm026*);
s32                     func_ov015_02125c94(BtlEnm026*);
void                    func_ov015_02125d0c(BtlEnm026*, s32, void*, void*);
void                    func_ov015_02125e5c(BtlEnm026*);
void                    func_ov015_02125f48(BtlEnm026*);
void                    func_ov015_02127690(BtlEnm026*);
void                    func_ov015_021276c4();
void                    func_ov015_021276c8();
void                    func_ov015_02127758(BtlEnm026*);
void                    func_ov015_021277a4(BtlEnm026*);
void                    func_ov015_0212786c(BtlEnm026*);
void                    func_ov015_021278b0(BtlEnm026*);
void                    func_ov015_021278f8(BtlEnm026*);
void                    func_ov015_02127950(BtlEnm026*);
void                    func_ov015_02127974(BtlEnm026*);
void                    func_ov015_021279c0(BtlEnm026*);
void                    func_ov015_02127a14(BtlEnm026*);
void                    func_ov015_02127a60(BtlEnm026*);
void                    func_ov015_02127a74(BtlEnm026*);
void                    func_ov015_02127ac4(BtlEnm026*);
void                    func_ov015_02127bc0(BtlEnm026*);
void                    func_ov015_02127bd0(BtlEnm026*);
void                    func_ov015_02127c18(BtlEnm026*);
void                    func_ov015_02127c68(BtlEnm026*);
void                    func_ov015_02127ca0(BtlEnm026*);
void                    func_ov015_02127d64(void);
void                    func_ov015_02127da4(BtlEnm026*);
void                    func_ov015_02127e5c(BtlEnm026*);
void                    func_ov015_02127e7c(BtlEnm026*);
void                    func_ov015_02127f90(BtlEnm026*);
extern const TaskHandle Tsk_BtlEnm026_Icon;
extern const TaskHandle Tsk_BtlEnm026_RG;
extern Enm026State      data_ov015_02128500;

#endif // COMBAT_NOISE_PRIVATE_BTLENM026_H
