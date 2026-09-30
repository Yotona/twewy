#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm010.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm010Bss {
    /* 0x00 */ Task* task;
    /* 0x04 */ u8    tail[0x1C];
} BtlEnm010Bss;

typedef struct BtlEnm010AnmSlot {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
} BtlEnm010AnmSlot;

typedef struct BtlEnm010AnmMgr {
    /* 0x00 */ void*            unk_00;
    /* 0x04 */ s32              unk_04;
    /* 0x08 */ void*            unk_08;
    /* 0x0C */ s32              unk_0C;
    /* 0x10 */ s32              unk_10;
    /* 0x14 */ BtlEnm010AnmSlot unk_14[4];
    /* 0x34 */ s32              unk_34;
    /* 0x38 */ u16              unk_38;
    /* 0x3A */ u16              pad_3A;
} BtlEnm010AnmMgr;

typedef struct Ov011Rec_PPWW {
    void* w0;
    void* w1;
    u32   w2;
    u32   w3;
} Ov011Rec_PPWW;

typedef struct Ov011Rec_PPPP {
    void* w0;
    void* w1;
    void* w2;
    void* w3;
} Ov011Rec_PPPP;

extern const BinIdentifier data_ov011_0212be28[3];
extern const BinIdentifier data_ov011_0212be40;

#define ENM010_POOL (&data_ov003_020e71b8->unk_10118)

s32 func_ov011_021259d0(s32 arg0, Task* task, s32 arg2, s32 cmd);
s32 func_ov011_02125a08(BtlEnm010AnmMgr* data, u16* arg1);
s32 func_ov011_02125b88(BtlEnm010AnmMgr* data);

extern s32  func_ov003_020cb32c(BtlEnm010AnmMgr* data, void* binId);
extern s32  func_ov003_020cb348(BtlEnm010AnmMgr* data, u16 index);
extern void func_ov003_020cb304(BtlEnm010AnmMgr* data, u16 index);
extern s32  func_ov003_020cb368(void* binId);

extern void func_ov003_020cb128(s32* p, void* a, s32 b);

extern void func_ov003_020cb200(BtlEnm010AnmMgr* data, u16 index);
extern s32  func_ov011_02125a08(BtlEnm010AnmMgr* data, u16* arg1);

extern void func_ov003_020cb194(void* p);

extern void func_ov003_020cb130(void* dst, void* a, u16 b, s32 c, void* e);

extern s32 func_ov003_020cb150(void* p);

typedef struct BtlEnm010Pair {
    s32 max;
    s32 sec;
} BtlEnm010Pair;

const BinIdentifier data_ov011_0212bdd8 = {3, "Apl_Suy/Grp_BtlEnm010_EffRange.bin"};

const u8 data_ov011_0212bde0[8] = {1, 0, 3, 0, 2, 0, 0, 0};

const Ov011Rec_PPWW data_ov011_0212bde8 = {data_ov011_0212be28, data_ov011_0212bde0, 0x00080000, 0x00000180};

const Ov011Rec_PPWW data_ov011_0212bdf8 = {data_ov011_0212be28, data_ov011_0212bde0, 0x00070000, 0x00000180};

const Ov011Rec_PPWW data_ov011_0212be08 = {&data_ov011_0212be40, data_ov011_0212bde0, 0x00070000, 0x00000180};

const Ov011Rec_PPWW data_ov011_0212be18 = {data_ov011_0212be28, data_ov011_0212bde0, 0x00090000, 0x00000180};

const BinIdentifier data_ov011_0212be28[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotIdleDamage.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotIdleDamage.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotIdleDamage.bin"},
};

const BinIdentifier data_ov011_0212be40 = {3, "Apl_Suy/Grp_BtlEnm010c_MotIdleDamage.bin"};

const BinIdentifier data_ov011_0212be48 = {3, "Apl_Suy/Grp_BtlEnm010_EffShot.bin"};

const BinIdentifier data_ov011_0212be50 = {3, "Apl_Suy/Grp_BtlEnm010_EffTattoo.bin"};

const BinIdentifier data_ov011_0212be58[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotLaser.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotLaser.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotLaser.bin"},
};

const BinIdentifier data_ov011_0212be70 = {3, "Apl_Suy/Grp_BtlEnm010c_MotLaser.bin"};

const BinIdentifier data_ov011_0212be78[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotTurn.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotTurn.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotTurn.bin"},
};

const BinIdentifier data_ov011_0212be90 = {3, "Apl_Suy/Grp_BtlEnm010c_MotTurn.bin"};

const BinIdentifier data_ov011_0212be98[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotRushOut.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotRushOut.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotRushOut.bin"},
};

const BinIdentifier data_ov011_0212beb0 = {3, "Apl_Suy/Grp_BtlEnm010c_MotRushOut.bin"};

const BinIdentifier data_ov011_0212beb8[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotMove.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotMove.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotMove.bin"},
};

const BinIdentifier data_ov011_0212bed0 = {3, "Apl_Suy/Grp_BtlEnm010c_MotMove.bin"};

const BinIdentifier data_ov011_0212bed8[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotTattoo.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotTattoo.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotTattoo.bin"},
};

const BinIdentifier data_ov011_0212bef0 = {3, "Apl_Suy/Grp_BtlEnm010c_MotTattoo.bin"};

const BinIdentifier data_ov011_0212bef8[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotBreath.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotBreath.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotBreath.bin"},
};

const BinIdentifier data_ov011_0212bf10 = {3, "Apl_Suy/Grp_BtlEnm010c_MotBreath.bin"};

const BinIdentifier data_ov011_0212bf18[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotMoveRush.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotMoveRush.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotMoveRush.bin"},
};

const BinIdentifier data_ov011_0212bf30 = {3, "Apl_Suy/Grp_BtlEnm010c_MotMoveRush.bin"};

const BinIdentifier data_ov011_0212bf38[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotSword.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotSword.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotSword.bin"},
};

const BinIdentifier data_ov011_0212bf50 = {3, "Apl_Suy/Grp_BtlEnm010c_MotSword.bin"};

const BinIdentifier data_ov011_0212bf58[3] = {
    {3,  "Apl_Suy/Grp_BtlEnm010_MotRushIn.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010a_MotRushIn.bin"},
    {3, "Apl_Suy/Grp_BtlEnm010b_MotRushIn.bin"},
};

const BinIdentifier data_ov011_0212bf70 = {3, "Apl_Suy/Grp_BtlEnm010c_MotRushIn.bin"};

const u16 data_ov011_0212bf78[3] = {4, 4, 4};

const u16 data_ov011_0212bf7e[3] = {448, 128, 112};

const u8 data_ov011_0212bf84[8] = {1, 0, 3, 0, 2, 0, 0, 0};

const u8 data_ov011_0212bf8c[8] = {1, 0, 3, 0, 2, 0, 0, 0};

const u8 data_ov011_0212bf94[8] = {1, 0, 3, 0, 2, 0, 0, 0};

const u8 data_ov011_0212bf9c[8] = {1, 0, 3, 0, 2, 0, 0, 0};

const TaskHandle data_ov011_0212bfa4 = {"Task_BtlEnm010_AnmMgr",
                                        (s32(*)(struct TaskPool*, struct Task*, void*, s32))func_ov011_021259d0, 60};

const u8 data_ov011_0212bfb0[16] = {1, 0, 3, 0, 2, 0, 0, 0, 1, 0, 3, 0, 2, 0, 1, 0};

const u8 data_ov011_0212bfc0[16] = {1, 0, 3, 0, 2, 0, 0, 0, 1, 0, 3, 0, 2, 0, 1, 0};

const u8 data_ov011_0212bfd0[16] = {1, 0, 3, 0, 2, 0, 0, 0, 1, 0, 3, 0, 2, 0, 1, 0};

const u16 data_ov011_0212bfe0[10] = {288, 328, 332, 384, 512, 420, 312, 336, 332, 332};

const u16 data_ov011_0212bff4[10] = {7, 4, 4, 4, 4, 4, 4, 4, 4, 4};

const u8 data_ov011_0212c008[24] = {1, 0, 3, 0, 2, 0, 0, 0, 1, 0, 3, 0, 2, 0, 1, 0, 1, 0, 3, 0, 2, 0, 2, 0};

const u8 data_ov011_0212c020[32] = {1, 0, 3, 0, 2, 0, 0, 0, 1, 0, 3, 0, 2, 0, 1, 0,
                                    1, 0, 3, 0, 2, 0, 2, 0, 1, 0, 3, 0, 2, 0, 3, 0};

const u8 data_ov011_0212c040[32] = {1, 0, 3, 0, 2, 0, 0, 0, 1, 0, 3, 0, 2, 0, 1, 0,
                                    1, 0, 3, 0, 2, 0, 2, 0, 1, 0, 3, 0, 2, 0, 3, 0};

const u8 data_ov011_0212c060[48] = {1, 0, 3, 0, 2, 0, 0, 0, 4, 0, 6, 0, 5, 0, 0, 0, 4, 0, 6, 0, 5, 0, 1, 0,
                                    4, 0, 6, 0, 5, 0, 2, 0, 4, 0, 6, 0, 5, 0, 3, 0, 4, 0, 6, 0, 5, 0, 4, 0};

const u8 data_ov011_0212c090[48] = {1, 0, 3, 0, 2, 0, 0, 0, 1, 0, 3, 0, 2, 0, 1, 0, 1, 0, 3, 0, 2, 0, 2, 0,
                                    1, 0, 3, 0, 2, 0, 3, 0, 1, 0, 3, 0, 2, 0, 4, 0, 1, 0, 3, 0, 2, 0, 5, 0};

const u8 data_ov011_0212c0c0[88] = {1, 0, 3, 0, 2, 0, 0, 0, 1, 0, 3, 0, 2, 0, 1, 0, 1, 0, 3, 0, 2, 0, 2, 0, 1, 0, 3,  0, 2, 0,
                                    3, 0, 1, 0, 3, 0, 2, 0, 4, 0, 1, 0, 3, 0, 2, 0, 5, 0, 1, 0, 3, 0, 2, 0, 6, 0, 1,  0, 3, 0,
                                    2, 0, 7, 0, 1, 0, 3, 0, 2, 0, 8, 0, 1, 0, 3, 0, 2, 0, 9, 0, 1, 0, 3, 0, 2, 0, 10, 0};

Ov011Rec_PPPP data_ov011_0212c460 = {&data_ov011_0212bdf8, &data_ov011_0212bde8, &data_ov011_0212be18, &data_ov011_0212be08};

const void* data_ov011_0212cadc[3] = {data_ov011_0212bf8c, data_ov011_0212c0c0, data_ov011_0212c090};

const void* data_ov011_0212cae8[3] = {&data_ov011_0212bdd8, &data_ov011_0212be50, &data_ov011_0212be48};

const void* data_ov011_0212caf4[10] = {data_ov011_0212c060, data_ov011_0212bf9c, data_ov011_0212c040, data_ov011_0212bfd0,
                                       data_ov011_0212bf94, data_ov011_0212bfb0, data_ov011_0212bf84, data_ov011_0212c008,
                                       data_ov011_0212bfc0, data_ov011_0212c020};

const void* data_ov011_0212cb1c[4][10] = {
    {&data_ov011_0212be28, &data_ov011_0212beb8, &data_ov011_0212bef8, &data_ov011_0212be78, &data_ov011_0212bf38,
     &data_ov011_0212bf58, &data_ov011_0212be98, &data_ov011_0212bf18, &data_ov011_0212bed8, &data_ov011_0212be58},
    {&data_ov011_0212be28, &data_ov011_0212beb8, &data_ov011_0212bef8, &data_ov011_0212be78, &data_ov011_0212bf38,
     &data_ov011_0212bf58, &data_ov011_0212be98, &data_ov011_0212bf18, &data_ov011_0212bed8, &data_ov011_0212be58},
    {&data_ov011_0212be28, &data_ov011_0212beb8, &data_ov011_0212bef8, &data_ov011_0212be78, &data_ov011_0212bf38,
     &data_ov011_0212bf58, &data_ov011_0212be98, &data_ov011_0212bf18, &data_ov011_0212bed8, &data_ov011_0212be58},
    {&data_ov011_0212be40, &data_ov011_0212bed0, &data_ov011_0212bf10, &data_ov011_0212be90, &data_ov011_0212bf50,
     &data_ov011_0212bf70, &data_ov011_0212beb0, &data_ov011_0212bf30, &data_ov011_0212bef0, &data_ov011_0212be70},
};

BtlEnm010Bss data_ov011_0212cca0;

void func_ov011_021256c0(u16 arg0) {
    u16   param              = arg0;
    u32   zero               = 0;
    Task* t                  = EasyTask_CreateTask(ENM010_POOL, &data_ov011_0212bfa4, 0, 0, zero, &param);
    data_ov011_0212cca0.task = t;
}

void func_ov011_02125714(void) {
    Task* t = EasyTask_GetTaskById(ENM010_POOL, data_ov011_0212cca0.task);
    if (t != NULL) {
        t->flags |= 0x10;
    }
}

void func_ov011_02125750(s32 arg0, CombatSprite* arg1, s32 arg2) {
    s32              flag = (arg1->flags46 & 1) ? 1 : 0;
    void*            bin;
    BtlEnm010AnmMgr* data;
    s32              slot;
    s32              size;
    u16              phase;

    CombatSprite_Release(arg1);
    data = EasyTask_GetTaskData(ENM010_POOL, data_ov011_0212cca0.task);
    bin  = data_ov011_0212cb1c[data->unk_38][arg2];
    if (func_ov003_020cb32c(data, bin) == 0) {
        slot = 0;
        do {
            if (func_ov003_020cb348(data, (u16)slot) <= 1) {
                break;
            }
            slot++;
        } while (slot < 2);
        func_ov003_020cb304(data, (u16)slot);
        if (slot == 0) {
            size = 0;
        } else {
            size = data->unk_34 - func_ov003_020cb368(bin);
        }
        func_ov003_020cb128(&data->unk_14[slot].unk_00, bin, size);
        func_ov003_020cb200(data, (u16)slot);
    }
    phase = data_ov011_0212bff4[arg2];
    if (data->unk_38 != 3) {
        phase = phase + data->unk_38;
    }
    CombatSprite_LoadFromTable(arg0, arg1, (const BinIdentifier*)bin, (const SpriteAnimEntry*)data_ov011_0212caf4[arg2], 0,
                               phase, data_ov011_0212bfe0[arg2]);
    if (flag == 1) {
        arg1->flags46 |= 1;
    }
}

void func_ov011_021258b4(s32 arg0, CombatSprite* arg1, s32 arg2) {
    void*            bin;
    BtlEnm010AnmMgr* data;
    s32              slot;
    s32              size;
    u16              elem;
    u16              pal;

    data = EasyTask_GetTaskData(ENM010_POOL, data_ov011_0212cca0.task);
    bin  = data_ov011_0212cae8[arg2];
    if (func_ov003_020cb32c(data, bin) == 0) {
        slot = 2;
        do {
            if (func_ov003_020cb348(data, (u16)slot) <= 1) {
                break;
            }
            slot++;
        } while (slot < 4);
        func_ov003_020cb304(data, (u16)slot);
        if (slot == 2) {
            size = data->unk_34;
        } else {
            s32 base = data->unk_04;
            size     = base - func_ov003_020cb368(bin);
        }
        func_ov003_020cb128(&data->unk_14[slot].unk_00, bin, size);
        func_ov003_020cb200(data, (u16)slot);
    }
    elem = data_ov011_0212bf78[arg2];
    pal  = data_ov011_0212bf7e[arg2];
    CombatSprite_LoadFromTable(arg0, arg1, (const BinIdentifier*)bin, (const SpriteAnimEntry*)data_ov011_0212cadc[arg2], 0,
                               elem, pal);
}

s32 func_ov011_021259d0(s32 arg0, Task* task, s32 arg2, s32 cmd) {
    BtlEnm010AnmMgr* data = task->data;

    switch (cmd) {
        case 0:
            return func_ov011_02125a08(data, arg2);
        case 3:
            return func_ov011_02125b88(data);
        default:
            return 1;
    }
}

s32 func_ov011_02125a08(BtlEnm010AnmMgr* data, u16* arg1) {
    BtlEnm010Pair p10 = {0, 0};
    BtlEnm010Pair p3  = {0, 0};
    s32           i;
    s32           size;

    MI_CpuSet(data, 0, 0x3C);
    for (i = 0; i < 10; i++) {
        size = func_ov003_020cb368(data_ov011_0212cb1c[arg1[0]][i]);
        if ((u32)size > (u32)p10.max) {
            p10.sec = p10.max;
            p10.max = size;
        } else if ((u32)size > (u32)p10.sec) {
            p10.sec = size;
        }
    }
    for (i = 0; i < 3; i++) {
        size = func_ov003_020cb368(data_ov011_0212cae8[i]);
        if ((u32)size > (u32)p3.max) {
            p3.sec = p3.max;
            p3.max = size;
        } else if ((u32)size > (u32)p3.sec) {
            p3.sec = size;
        }
    }
    func_ov003_020cb128(&data->unk_14[0].unk_00, data_ov011_0212cb1c[arg1[0]][0], 0);
    func_ov003_020cb128(&data->unk_14[1].unk_00, 0, 0);
    func_ov003_020cb128(&data->unk_14[2].unk_00, 0, 0);
    func_ov003_020cb128(&data->unk_14[3].unk_00, 0, 0);
    func_ov003_020cb130(data, &data->unk_14[0], 4, p10.max + p10.sec + p3.max + p3.sec, "BtlEnm010_AnmMgr");
    func_ov003_020cb150(data);
    func_ov003_020cb200(data, 0);
    data->unk_38 = arg1[0];
    data->unk_34 = p10.max + p10.sec;
    return 1;
}

s32 func_ov011_02125b88(BtlEnm010AnmMgr* data) {
    func_ov003_020cb194(data);
    return 1;
}
