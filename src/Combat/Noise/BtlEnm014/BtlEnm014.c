#include "Combat/Noise/Private/BtlEnm014.h"
#include "Combat/Core/Combat.h"
#include "Save.h"

#include <nitro/mi/cpumem.h>

typedef struct Enm014Variant {
    /* 0x00 */ BinIdentifier*   binIden;
    /* 0x04 */ SpriteAnimEntry* animTable;
    /* 0x08 */ u16              unk_08;
    /* 0x0A */ u16              unk_0A;
    /* 0x0C */ u16              unk_0C;
    /* 0x0E */ u16              unk_0E;
} Enm014Variant; // Size: 0x10

extern void func_ov003_020c3efc(void*, void*);
extern void func_ov003_020c427c(void*);
extern void func_ov003_020c4cc4(void*, s32);
extern s32  func_ov003_020c5bfc(void*);
extern s32  func_ov003_020c72b4(void*, s32, s32);
extern void func_ov003_020ccf58(s32, void*);
extern s32  func_ov003_020ccfc8(void*, s32);
extern s32  func_ov003_020ccfec(void*);

extern char data_ov012_02127990[28];

extern char           data_ov012_021279ac[28];
extern char           data_ov012_021279c8[28];
extern char           data_ov012_021279e4[28];
const BinIdentifier   data_ov012_02127894;
const BinIdentifier   data_ov012_0212789c;
const BinIdentifier   data_ov012_021278a4;
const BinIdentifier   data_ov012_021278ac;
SpriteAnimEntry const data_ov012_021278b4[8][2];

const u16 data_ov012_0212783c[4] = {0x10, 0x16, 0x16, 0x16};

const Enm014Variant data_ov012_02127844 = {&data_ov012_02127894, &data_ov012_021278b4[0][0], 0, 0x10, 0xD8, 0};

const SpriteAnimEntry data_ov012_02127854[2] = {
    {   4,    6,    5, 0},
    {0x13, 0x15, 0x14, 0},
};

const Enm014Variant data_ov012_02127864 = {&data_ov012_0212789c, &data_ov012_021278b4[0][0], 0, 0x16, 0xF0, 0};

const Enm014Variant data_ov012_02127874 = {&data_ov012_021278ac, &data_ov012_021278b4[0][0], 0, 0x16, 0x104, 0};

const Enm014Variant data_ov012_02127884 = {&data_ov012_021278a4, &data_ov012_021278b4[0][0], 0, 0x16, 0x104, 0};

const BinIdentifier data_ov012_02127894 = {3, data_ov012_02127990};

const BinIdentifier data_ov012_0212789c = {3, data_ov012_021279ac};

const BinIdentifier data_ov012_021278a4 = {3, data_ov012_021279c8};

const BinIdentifier data_ov012_021278ac = {3, data_ov012_021279e4};

const SpriteAnimEntry data_ov012_021278b4[8][2] = {
    {         {1, 3, 2, 0},          {1, 3, 2, 1}},
    {         {7, 9, 8, 0},          {7, 9, 8, 1}},
    {   {0xA, 0xC, 0xB, 0},    {0xA, 0xC, 0xB, 1}},
    {   {0xA, 0xC, 0xB, 2},    {0xA, 0xC, 0xB, 3}},
    {   {0xA, 0xC, 0xB, 4},    {0xA, 0xC, 0xB, 0}},
    {   {0xA, 0xC, 0xB, 5},    {0xA, 0xC, 0xB, 2}},
    {   {0xD, 0xF, 0xE, 0},    {0xD, 0xF, 0xE, 1}},
    {{0x10, 0x12, 0x11, 0}, {0x10, 0x12, 0x11, 1}},
};

Enm014Variant* data_ov012_02127980[4] = {
    &data_ov012_02127844,
    &data_ov012_02127864,
    &data_ov012_02127884,
    &data_ov012_02127874,
};

char data_ov012_02127990[28] = "Apl_Hor/Grp_BtlEnm014.bin";

char data_ov012_021279ac[28] = "Apl_Hor/Grp_BtlEnm014a.bin";

char data_ov012_021279c8[28] = "Apl_Hor/Grp_BtlEnm014b.bin";

char data_ov012_021279e4[28] = "Apl_Hor/Grp_BtlEnm014c.bin";

BinIdentifier* func_ov012_021256c0(s32 index) {
    return (BinIdentifier*)((u8*)&data_ov012_02127894 + index * 8);
}

const SpriteAnimEntry* func_ov012_021256d0(void) {
    return data_ov012_02127854;
}

u16 func_ov012_021256dc(s32 index) {
    return data_ov012_0212783c[index];
}

void func_ov012_021256f0(s32 engine, BtlEnm014* data, s32 arg2) {
    Enm014Spawn spawn;
    MI_CpuSet(&spawn, 0, sizeof(spawn));
    spawn.unk_00 = data;
    spawn.unk_04 = engine;
    spawn.unk_08 = (u16)arg2;

    TaskPool* pool;
    if (engine == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(pool, &Tsk_BtlEnm014_Eff, NULL, 0, NULL, &spawn);
}

void func_ov012_02125768(BtlEnm014* data, void (*fn)(BtlEnm014*)) {
    func_ov003_020c427c(data);
    data->unk_1C4 = fn;
    data->unk_1C2 = 0;
    data->unk_1C0 = 0;
}

void func_ov012_02125790(s32 engine, BtlEnm014* data, s32 arg2, s32 arg3) {
    data->unk_1CC = 1;
    data->unk_1C8 = arg3;
    func_ov003_020c3efc(data, (void*)arg2);
    func_ov003_020ccf58(engine, data);
}

void func_ov012_021257c4(BtlEnm014* data) {
    if (func_ov003_020c5bfc(data) != 0) {
        return;
    }
    func_ov012_02125768(data, (void (*)(BtlEnm014*))data->unk_1C8);
}

void func_ov012_021257e8(BtlEnm014* data) {
    if (func_ov003_020ccfc8(data, 4) != 0) {
        return;
    }
    func_ov012_02125768(data, (void (*)(BtlEnm014*))data->unk_1C8);
}

void func_ov012_02125810(BtlEnm014* data) {
    if (func_ov003_020ccfec(data) == 0) {
        data->unk_1CC = 0;
    }
}

void func_ov012_0212582c(BtlEnm014* data) {
    if (func_ov003_020c72b4(data, 0, 7) != 0) {
        return;
    }
    func_ov012_02125768(data, (void (*)(BtlEnm014*))data->unk_1C8);
}

void func_ov012_02125858(BtlEnm014* data) {
    BtlEnm014* boss;

    if (data->unk_084.sprite.bits_0_1 == 0) {
        boss = data_ov003_020e71b8->unk3D898;
    } else {
        boss = data_ov003_020e71b8->unk3D89C;
    }
    data->actor.unk_62 = 100;
    if (data->unk_084.flags46 & 1) {
        if (data->actor.position.x < boss->actor.position.x) {
            goto hit;
        }
    }
    if (!(data->unk_084.flags46 & 1)) {
        if (data->actor.position.x > boss->actor.position.x) {
            goto hit;
        }
    }
    return;

hit:
    data->actor.unk_62 = 50;
    if (data->unk_080 == 3) {
        data->actor.unk_62 = 0;
    }
}

void func_ov012_021258e8(void) {
    if (data_ov003_020e71b8->unk3D88E != 0x90) {
        return;
    }
    if (gSaveData.battleClearTime <= 0x654) {
        return;
    }
    data_ov003_020e71b8->unk3D875 = 2;
    data_ov003_020e71b8->unk3D878 |= 0x200000;
    data_ov003_020e71b8->unk3D878 |= 0x20000000;
}

void func_ov012_02125958(BtlEnm014* data) {
    if (data->unk_1D4 == 0 && data->actor.unk_5A != 0 && data->actor.unk_62 != 0x64) {
        func_ov012_021256f0(data->unk_084.sprite.bits_0_1, data, 0);
        func_ov003_020c4cc4(data, 0x21E);
    }
    data->unk_1D4 = data->actor.unk_5A;
}
