#include "Combat/Noise/Boss01.h"
#include "EasyFade.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Math/Random.h"
#include "Engine/Resources/ResourceMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <types.h>

#include <NitroSDK/fx.h>
#include <NitroSDK/mi/cpumem.h>

/* Forward declaration for the main boss/object struct used in this overlay. */
typedef struct Ov016Obj Ov016Obj;

/* Compact sprite parameter bundle used for simple animation setup. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
} Ov016SpriteParams;

/* Runtime sprite instance with extended state and animation fields. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u8  pad_08[0x14];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u8  pad_1E[0x2];
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u8  pad_22[0x4];
    /* 0x26 */ u16 unk26;
    /* 0x28 */ u16 unk28;
    /* 0x2A */ u16 unk2A;
    /* 0x2C */ s32 unk2C;
} Ov016Sprite;

/* Animation configuration for spawning or initializing a sprite. */
typedef struct {
    /* 0x00 */ Ov016SpriteParams        params;
    /* 0x08 */ const void*              unk8;
    /* 0x0C */ const Ov016SpriteParams* unkC;
    /* 0x10 */ u16                      unk10;
    /* 0x12 */ u16                      unk12;
    /* 0x14 */ u16                      unk14;
    /* 0x16 */ u16                      unk16;
} Ov016AnimConfig;

/* Global overlay state block for ov003 (shared world state). */
typedef struct {
    /* 0x00000 */ TaskPool unk_00000;
    /* 0x00080 */ u8       unk_00080[0x808C - 0x80];
    /* 0x0808C */ TaskPool taskPool;
    /* 0x0810C */ char     unk_0810C[0x3D350 - 0x810C];
    /* 0x3D350 */ Data*    unk3D350;
    /* 0x3D354 */ u8       pad_3D7CC[0x478];
    /* 0x3D7CC */ s32      unk3D7CC;
    /* 0x3D7D0 */ u8       pad_3D824[0x54];
    /* 0x3D824 */ s32      unk3D824;
    /* 0x3D828 */ u8       pad_3D878[0x50];
    /* 0x3D878 */ u32      unk3D878;
    /* 0x3D87C */ u8       pad_3D898[0x1C];
    /* 0x3D898 */ void*    unk3D898;
    /* 0x3D89C */ void*    unk3D89C;
    /* 0x3D8A0 */ void*    unk3D8A0;
    /* 0x3D8A4 */ u8       pad_3D8AC[0x8];
    /* 0x3D8AC */ void*    unk3D8AC;
    /* 0x3D8B0 */ u8       pad_3D8B4[0x4];
    /* 0x3D8B4 */ void*    unk3D8B4;
} Ov003Global;

/* Simple quad of u16 values, often used for dimensions or indices. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
} Ov016U16Quad;

#define OV003_LIST_3D8AC ((void*)((u8*)data_ov003_020e71b8 + 0x3D8AC))
#define OV003_LIST_3D8B4 ((void*)((u8*)data_ov003_020e71b8 + 0x3D8B4))

struct Ov016Obj {
    /* 0x000 */ u16 unk0;
    /* 0x002 */ u16 unk2;
    /* 0x004 */ u16 unk4;
    /* 0x006 */ u16 unk6;
    /* Sprite/state header and linkage. */
    /* 0x008 */ u8        pad_08[0x18 - 0x08];
    /* 0x018 */ Ov016Obj* unk18;
    /* 0x01C */ u8        pad_1C[0x20 - 0x1C];
    /* Primary position/velocity and movement parameters. */
    /* 0x020 */ s32 unk20;
    /* 0x024 */ s32 unk24;
    /* 0x028 */ s32 unk28;
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ s32 unk30;
    /* 0x034 */ s16 unk34;
    /* 0x036 */ s16 unk36;
    /* 0x038 */ s32 unk38;
    /* 0x03C */ s32 unk3C;
    /* 0x040 */ s32 unk40;
    /* 0x044 */ s32 unk44;
    /* 0x048 */ s32 unk48;
    /* 0x04C */ s32 unk4C;
    /* 0x050 */ u8  pad_50[0x54 - 0x50];
    /* Flags and timers used by AI/phase logic. */
    /* 0x054 */ s32 unk54;
    /* 0x058 */ s32 unk58;
    /* 0x05C */ s16 unk5C;
    /* 0x05E */ s16 unk5E;
    /* Trajectory and spawn configuration. */
    /* 0x060 */ s32 unk60;
    /* 0x064 */ s32 unk64;
    /* 0x068 */ s32 unk68;
    /* 0x06C */ s16 unk6C;
    /* 0x06E */ s16 unk6E;
    /* 0x070 */ s32 unk70;
    /* 0x074 */ s16 unk72;
    /* 0x074 */ s16 unk74;
    /* 0x076 */ s16 unk76;
    /* 0x078 */ s16 unk78;
    /* 0x07A */ s16 unk7A;
    /* 0x07C */ s16 unk7C;
    /* 0x07E */ s16 unk7E;
    /* 0x080 */ s16 unk80;
    /* 0x082 */ s16 unk82;
    /* 0x084 */ s16 unk84;
    /* 0x086 */ s16 unk86;
    /* 0x088 */ s16 unk88;
    /* 0x08A */ s16 unk8A;
    /* 0x08C */ s16 unk8C;
    /* 0x08E */ u16 unk8E;
    /* Render state and sprite instances are interleaved here. */
    /* 0x090 */ u8  pad_90[0x96 - 0x90];
    /* 0x096 */ s16 unk96;
    /* 0x098 */ u8  pad_98[0xA4 - 0x98];
    /* 0x0A4 */ s16 unkA4;
    /* 0x0A6 */ s16 unkA6;
    /* 0x0A8 */ u8  pad_A8[0xB4 - 0xA8];
    /* 0x0B4 */ s32 unkB4;
    /* 0x0B8 */ u8  pad_B8[0xC0 - 0xB8];
    /* 0x0C0 */ s16 unkC0;
    /* 0x0C2 */ s16 unkC2;
    /* 0x0C4 */ s16 unkC4;
    /* 0x0C6 */ u8  pad_C6[0xC8 - 0xC6];
    /* 0x0C8 */ s16 unkC8;
    /* 0x0CA */ u16 unkCA;
    /* 0x0CC */ u8  pad_CC[0xEC - 0x0CC];
    /* 0x0EC */ s32 unkEC;
    /* 0x0F0 */ u8  pad_F0[0x108 - 0x0F0];
    /* 0x108 */ s32 unk108;
    /* 0x10C */ u8  pad_10C[0x14C - 0x10C];
    /* 0x14C */ s32 unk14C;
    /* 0x150 */ u8  pad_150[0x184 - 0x150];
    /* Relationships to other objects and task data. */
    /* 0x184 */ Ov016Obj* unk184;
    /* 0x188 */ void*     unk188;
    /* 0x18C */ u16       unk18C;
    /* 0x18E */ s16       unk18E;
    /* 0x190 */ s32       unk190;
    /* 0x194 */ s32       unk194;
    /* 0x198 */ u8        pad_198[0x1A4 - 0x198];
    /* 0x1A4 */ s16       unk1A4;
    /* 0x1A6 */ s16       unk1A6;
    /* 0x1A8 */ u8        pad_1A8[0x1C0 - 0x1A8];
    /* AI state callbacks and counters. */
    /* 0x1C0 */ s16 unk1C0;
    /* 0x1C2 */ u16 unk1C2;
    /* 0x1C4 */ s16 unk1C4;
    /* 0x1C6 */ s16 unk1C6;
    /* 0x1C8 */ void (*unk1C8)(Boss01*);
    /* 0x1CC */ void (*unk1CC)(Boss01*);
    /* 0x1D0 */ s32       unk1D0;
    /* 0x1D4 */ s32       unk1D4;
    /* 0x1D8 */ s32       unk1D8;
    /* 0x1DC */ s32       unk1DC;
    /* 0x1E0 */ Ov016Obj* unk1E0;
    /* 0x1E4 */ Ov016Obj* unk1E4;
    /* 0x1E8 */ s32       unk1E8;
    /* 0x1EC */ s32       unk1EC;
    /* 0x1F0 */ s32       unk1F0;
    /* 0x1F4 */ u8        pad_1F4[0x284 - 0x1F4];
    /* Boss core data and sub-state callback. */
    /* 0x284 */ u16 unk284;
    /* 0x286 */ u16 unk286;
    /* 0x288 */ u16 unk288;
    /* 0x28A */ u16 unk28A;
    /* 0x28C */ void (*unk28C)(Ov016Obj*);
    /* 0x290 */ s16 unk290;
    /* 0x292 */ s16 unk292;
    /* 0x294 */ s32 unk294;
};

void func_02026590(void*, s32, u16);                                                         /* extern */
void func_020265d4(void*, s32, u16);                                                         /* extern */

void      func_ov003_020824a0(void*, u16, s32);                                              /* extern */
void      func_ov003_020826c8(void*, s32);                                                   /* extern */
void      func_ov003_02082724(void*, s16, s16);                                              /* extern */
void      func_ov003_02082730(void*, s32);                                                   /* extern */
void      func_ov003_02082750(void*, s32);                                                   /* extern */
void      func_ov003_02082848(void*, s32);                                                   /* extern */
void      func_ov003_02082940(void*, s32, const void*);                                      /* extern */
void      func_ov003_02082998(void*, void*);                                                 /* extern */
void      func_ov003_02082a04(s32, void*, const BinIdentifier*, const void*, s32, s32, s32); /* extern */
void      func_ov003_02082a90(s16, void*, const void*, s32, s32, s32, s32, s32);             /* extern */
void      func_ov003_02082b0c(void*);                                                        /* extern */
void      func_ov003_02082b64(void*);                                                        /* extern */
void      func_ov003_02082cc4(void*);                                                        /* extern */
void      func_ov003_02082f1c(void*, s32);                                                   /* extern */
s32       func_ov003_02082f2c(void*);                                                        /* extern */
void      func_ov003_02083000(s32, void*);                                                   /* extern */
void      func_ov003_02083074(void*);                                                        /* extern */
void      func_ov003_02083118(void*, s32);                                                   /* extern */
void      func_ov003_020831e4(s32, s32);                                                     /* extern */
void      func_ov003_02083bfc(void*, s32, s32);                                              /* extern */
void      func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);                               /* extern */
s16       func_ov003_020843b0(s32, s32);                                                     /* extern */
u16       func_ov003_020843ec(s32, s32, s32);                                                /* extern */
void*     func_ov003_0208495c(void*);                                                        /* extern */
void*     func_ov003_02084984(void*);                                                        /* extern */
void      func_ov003_02086404(s32, void*, u16);                                              /* extern */
void      func_ov003_020864dc(void*, u16);                                                   /* extern */
s32       func_ov003_0208656c();                                                             /* extern */
void      func_ov003_02086654();                                                             /* extern */
void      func_ov003_02087f28(s32, s32);                                                     /* extern */
void      func_ov003_020880e4(s32, void*, void*);                                            /* extern */
void      func_ov003_0208810c(void*, void*);                                                 /* extern */
s32       func_ov003_02088130();                                                             /* extern */
s32       func_ov003_020c37f8(void*);                                                        /* extern */
void      func_ov003_020c38e8(void*);                                                        /* extern */
s32       func_ov003_020c3908(void*);                                                        /* extern */
s32       func_ov003_020c3960(s32*, s32);                                                    /* extern */
TaskPool* func_ov003_020c3a10();                                                             /* extern */
s32       func_ov003_020c3aac(s32);                                                          /* extern */
s32       func_ov003_020c3bf0(void*);                                                        /* extern */
s32       func_ov003_020c3c28();                                                             /* extern */
void      func_ov003_020c3c44(void*, s32, s32);                                              /* extern */
void*     func_ov003_020c3ca4(void*);                                                        /* extern */
void      func_ov003_020c3cec(void*, void*, void*, void*);                                   /* extern */
void      func_ov003_020c3ec0(void*);                                                        /* extern */
void      func_ov003_020c3efc(void*, void*);                                                 /* extern */
void      func_ov003_020c427c();                                                             /* extern */
void      func_ov003_020c4988(void*);                                                        /* extern */
void      func_ov003_020c49c8(void*);                                                        /* extern */
void      func_ov003_020c4a34(void*);                                                        /* extern */
void      func_ov003_020c4a74(void*);                                                        /* extern */
void      func_ov003_020c4c1c(void*);                                                        /* extern */
void      func_ov003_020c4c5c(void*);                                                        /* extern */
void      func_ov003_020c4cc4(void*, s32);                                                   /* extern */
void      func_ov003_020c4ff4();                                                             /* extern */
void      func_ov003_020c50bc();                                                             /* extern */
void      func_ov003_020c5a00(void*);                                                        /* extern */
s32       func_ov003_020c5a28(void*, void*);                                                 /* extern */
void      func_ov003_020c5ac0(void*, s32, void*, s32);                                       /* extern */
void      func_ov003_020c5b0c(s32, void*);                                                   /* extern */
s32       func_ov003_020c5b2c(s32, void*, s32, s32, s32);                                    /* extern */
s32       func_ov003_020c5bc4(u16, void*, s32, s32, s32);                                    /* extern */
s32       func_ov003_020c6f40();                                                             /* extern */
s32       func_ov003_020c9008();                                                             /* extern */
void      func_ov003_020c9064(void*);                                                        /* extern */
s32       func_ov003_020c9338(void*);                                                        /* extern */
s32       func_ov003_020cb3c4(s32, s32);                                                     /* extern */
s32       func_ov003_020cc5fc(s32, s32);                                                     /* extern */
s32       func_ov009_020fa71c(void*);                                                        /* extern */

s32   Boss01_Eff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);                 /* static */
void  Boss01_MoveState_FlyAcross(Boss01* boss);                                              /* static */
s32   Boss01_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);                  /* static */
void  func_ov016_021266f4(Boss01* boss);                                                     /* static */
void  func_ov016_02126854(Boss01* boss);                                                     /* static */
void  func_ov016_02126c24(Boss01* boss);                                                     /* static */
void  func_ov016_02126d38(Boss01* boss);                                                     /* static */
s32   Boss01_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);                  /* static */
s32   Boss01s_FlyRG_Curve_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);        /* static */
s32   Boss01s_FlyUG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);              /* static */
s32   Boss01s_LandRG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);             /* static */
s32   Boss01s_LandUG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);             /* static */
s32   BtlEff_Bomb_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);                /* static */
void  BtlEff_Bomb_CreateTask(s32 arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);  /* static */
s32   BtlEff_Fall_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);                /* static */
void  BtlEff_Fall_CreateTask(UnkStruct_Boss01* arg0, Boss01* arg1, s32 arg2);                /* static */
s32   BtlObs_Bus_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);                 /* static */
void* BtlObs_Bus_CreateTask(void* bossUG);
s32   BtlObs_RG_CreateTask(u16 arg0, void* arg1);                                            /* static */
void  Boss01s_FlyRG_Curve_CreateTask(void);                                                  /* static */
void  Boss01s_FlyUG_CreateTask(void);                                                        /* static */
void  Boss01s_LandRG_CreateTask(void);                                                       /* static */
void  Boss01s_LandUG_CreateTask(void);                                                       /* static */

extern Ov003Global* data_ov003_020e71b8;
extern void*        data_ov009_021234cc;
extern void*        data_ov009_02123540;

/* Resource table entries used by animations/sprites. */
static const BinIdentifier data_ov016_02128f88 = {3, "Apl_Mor/Grp_Boss01b.bin"};
static const BinIdentifier data_ov016_02128f90 = {3, "Apl_Mor/Grp_Boss01c.bin"};
static const BinIdentifier data_ov016_02128f98 = {3, "Apl_Mor/Grp_Boss01s.bin"};
static const BinIdentifier data_ov016_0212909c = {3, "Apl_Mor/Grp_BtlEff_Obs.bin"};

/* Default animation config for a boss sprite state. */
static const Ov016AnimConfig data_ov016_02128f70 = {
    {1, 3, 2, 0},
    &data_ov016_02128f88, (const Ov016SpriteParams*)&data_ov016_02128f70, 0, 4, 0xD0, 0
};

/* Frame/animation parameter tables. */
static const u16 data_ov016_02129010[] = {1, 3, 2, 0, 1, 3, 2, 3};

static const u16 data_ov016_0212903c[] = {1, 3, 2, 4, 1, 3, 2, 5, 1, 3, 2, 6};

static const u16 data_ov016_02129054[] = {1, 3, 2, 1};
static const u16 data_ov016_02129078[] = {1, 3, 2, 2};
static const u16 data_ov016_021290a4[] = {4, 6, 5, 0};

static const u16 data_ov016_021290e4[] = {1, 3, 2, 0, 1, 3, 2, 1, 1, 3, 2, 2};

static const u16 data_ov016_02129120[] = {1, 3, 2, 0, 4, 6, 5, 0, 7, 9, 8, 0, 0x5000, 0};

/* Boss resource selection by index. */
static const BinIdentifier* data_ov016_021291ac[3] = {&data_ov016_02128f88, &data_ov016_02128f88, &data_ov016_02128f90};

/* Global overlay state flags/counters. */
static s32 data_ov016_021291d8 = 1;
static s32 data_ov016_021292c0[8];

/* Configure a sprite instance for a boss animation variant. */
void func_ov016_021256c0(Ov016Sprite* arg0, s32 arg1, s32 arg2) {
    u16                      sp0;
    u16                      temp_r2;
    u16                      var_r1;
    const Ov016SpriteParams* temp_r6;

    temp_r6 = data_ov016_02128f70.unkC;
    if (arg1 == 2) {
        func_ov003_02082940(arg0, arg1, &data_ov016_02128f90);
    } else {
        func_ov003_02082940(arg0, arg1, &data_ov016_02128f88);
    }
    arg0->unk0  = (u16)(arg0->unk0 & ~0xC00);
    arg0->unk1C = (u16)temp_r6->unk0;
    arg0->unk26 = (u16)temp_r6->unk4;
    arg0->unk28 = (u16)temp_r6->unk2;
    arg0->unk20 = 4;
    arg0->unk2A = (s16)(temp_r6->unk6 + 1);
    arg0->unk2C = 0;
    arg0->unk4  = 0x80;
    arg0->unk6  = 0x60;
    sp0         = arg0->unk2;
    temp_r2     = sp0;
    if (arg2 != 1) {
        var_r1 = ((u32)(temp_r2 << 0x16) >> 0x1B) & ~8;
    } else {
        var_r1 = ((u32)(temp_r2 << 0x16) >> 0x1B) | 8;
    }
    sp0        = (temp_r2 & ~0x3E0) | ((u32)(var_r1 << 0x1B) >> 0x16);
    arg0->unk2 = sp0;
}

/// MARK: Boss01_Eff

typedef struct {
    /* 0x0 */ Vec unk_0;
    /* 0xC */ s16 unk_C;
    /* 0xE */ s16 unk_E;
} Boss01_Eff_Args;

typedef struct {
    /* 0x00 */ char            unk_00[0x60];
    /* 0x60 */ Boss01_Eff_Args unk_60;
    /* 0x70 */ s32             unk_70;
    /* 0x74 */ s32             unk_74;
    /* 0x78 */ s32             unk_78;
    /* 0x7C */ s32             unk_7C;
} Boss01_Eff; // Size: 0x80

static const TaskHandle Tsk_Boss01_Eff = {"Tsk_Boss01_Eff", Boss01_Eff_RunTask, 0x80};

s32 Boss01_Eff_Init(TaskPool* pool, Task* task, void* targs) {
    Boss01_Eff*      temp_r4 = task->data;
    Boss01_Eff_Args* args    = (Boss01_Eff_Args*)targs;

    MI_CpuSet(temp_r4, 0, 0x80);

    temp_r4->unk_60 = *args;

    func_ov003_02082a90(temp_r4->unk_60.unk_C, temp_r4, data_ov016_021291ac[temp_r4->unk_60.unk_C], 1, 3, 2, 4,
                        (s32)(u16)(RNG_Next(5) + 1));
    func_ov003_02082750(temp_r4, RNG_Next(2));
    temp_r4->unk_7C = (RNG_Next(1) + 0x1666);
    temp_r4->unk_78 = -(RNG_Next(0x219B) + 0x666);
    return 1;
}

s32 Boss01_Eff_Update(TaskPool* pool, Task* task, void* args) {
    Boss01_Eff* temp_r4 = task->data;

    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    switch (temp_r4->unk_74) {
        case 0:
            temp_r4->unk_60.unk_0.x += temp_r4->unk_7C;
            temp_r4->unk_60.unk_0.z += temp_r4->unk_78;
            temp_r4->unk_78 += 286;

            if (temp_r4->unk_78 > 737) {
                temp_r4->unk_70++;
            } else {
                temp_r4->unk_70 = 0;
                temp_r4->unk_74++;
            }

        case 1:
            temp_r4->unk_60.unk_0.x += temp_r4->unk_7C;
            temp_r4->unk_60.unk_0.z += temp_r4->unk_78;

            temp_r4->unk_78 -= 205;
            if (temp_r4->unk_78 < 0) {
                temp_r4->unk_78 = (RNG_Next(0xB35) + 0xCCC);
                if (temp_r4->unk_7C > 0) {
                    temp_r4->unk_7C = -(RNG_Next(0x1001) + 0x800);
                } else {
                    temp_r4->unk_7C = (RNG_Next(0x1001) + 0x800);
                }
            }
            if (temp_r4->unk_60.unk_0.z >= 0) {
                return 0;
            }

        default:
            func_ov003_02082b0c(temp_r4);
            return 1;
    }
}

s32 Boss01_Eff_Render(TaskPool* pool, Task* task, void* args) {
    s16       spA, sp8;
    Ov016Obj* temp_r4 = task->data;

    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk60, temp_r4->unk64, temp_r4->unk68);
    func_ov003_02082724(temp_r4, spA, sp8);
    func_ov003_02082b64(temp_r4);
    return 1;
}

s32 Boss01_Eff_Destroy(TaskPool* pool, Task* task, void* args) {
    func_ov003_02082cc4(task->data);
    return 1;
}

s32 Boss01_Eff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02128fac = {
        .initialize = Boss01_Eff_Init,
        .update     = Boss01_Eff_Update,
        .render     = Boss01_Eff_Render,
        .cleanup    = Boss01_Eff_Destroy,
    };
    return data_ov016_02128fac.iter[stage](pool, task, args);
}

void Boss01_Eff_CreateTask(Boss01* boss) {
    Boss01_Eff_Args args;

    if (boss->unk_1A4 > boss->unk_1A6) {
        u32 temp_r4 = (boss->unk_84 << 0x1E) >> 0x1E;
        if (boss->unk_24 == 0) {
            args.unk_0.x = boss->unk_28 + ((RNG_Next(25) + 24) << 0xC);
        } else {
            args.unk_0.x = boss->unk_28 - ((RNG_Next(25) + 24) << 0xC);
        }
        args.unk_0.y = boss->unk_2C;
        args.unk_0.z = boss->unk_30 + ((RNG_Next(129) - 64) << 0xC);
        args.unk_C   = temp_r4;
        args.unk_E   = boss->unk_CA & 1;
        EasyTask_CreateTask(func_ov003_020c3ca4(boss), &Tsk_Boss01_Eff, 0, 0, 0, &args);
        boss->unk_1A4 = 0;
        boss->unk_1A6 = (RNG_Next(11) + 10);
    }
    boss->unk_1A4++;
}

s32 Boss01_RG_IsInBounds(Boss01* boss) {
    BOOL var_r3 = TRUE;
    BOOL var_ip = ((boss->unk_24 == 0) && (boss->unk_28 < -0x80000));

    if (var_ip == FALSE) {
        BOOL var_ip_2 = ((boss->unk_24 != 0) && (boss->unk_28 > (data_ov003_020e71b8->unk3D824 + 0x80000)));

        if (var_ip_2 == FALSE) {
            var_r3 = FALSE;
        }
    }
    return var_r3;
}

void func_ov016_02125bb8(Boss01* boss) {
    boss->unk_1D4 = (boss->unk_24 != 0) ? 0x4000 : -0x4000;
}

void Boss01_SetMoveState(Boss01* boss, void (*arg1)(Boss01*)) {
    func_ov003_020c427c();
    boss->unk_1CC = arg1;
    boss->unk_1C8 = 0;
    boss->unk_1C0 = 0;
}

void Boss01_MoveState_WaitToFly(Boss01* boss) {
    if (*data_ov016_021292c0 == 5) {
        Boss01_SetMoveState(boss, Boss01_MoveState_FlyAcross);
    }
}

void func_ov016_02125c20(Boss01* boss, s32 arg1) {
    s32       temp_r0;
    s32       temp_r0_2;
    s32       temp_r0_3;
    s32       temp_r0_4;
    Ov016Obj* var_r4;

    temp_r0 = boss->unk_1D8;
    if ((temp_r0 != 0) && (arg1 != 0)) {
        return;
    }
    if ((temp_r0 == 0) && (arg1 == 0)) {
        return;
    }
    boss->unk_1D8 = arg1;
    var_r4        = func_ov003_0208495c(OV003_LIST_3D8AC);
    if (var_r4 == NULL) {
        return;
    }
    do {
        temp_r0_2 = var_r4->unk7C;
        switch (temp_r0_2) {
            case 7:
                temp_r0_3 = var_r4->unk54;
                if (arg1 != 0) {
                    var_r4->unk54 = (s32)(temp_r0_3 | 0x30);
                } else {
                    var_r4->unk54 = (s32)(temp_r0_3 & ~0x10 & ~0x20);
                }
                break;
            case 34:
                temp_r0_4 = var_r4->unk54;
                if (arg1 == 0) {
                    var_r4->unk54 = (s32)(temp_r0_4 | 0x30);
                } else {
                    var_r4->unk54 = (s32)(temp_r0_4 & ~0x10 & ~0x20);
                }
                break;
            default:
                if (arg1 == 0) {
                    var_r4->unk54 = (s32)(var_r4->unk54 | 0x30);
                } else {
                    data_ov003_020e71b8->unk3D8A0 = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, boss->unk_1DC);
                    var_r4->unk54                 = (s32)(var_r4->unk54 & ~0x10 & ~0x20);
                }
                break;
        }
        var_r4 = func_ov003_02084984((u8*)var_r4 + 0x178);
    } while (var_r4 != NULL);
}

void Boss01_MoveState_FlyAcross(Boss01* boss) {
    void*   temp_r4 = data_ov003_020e71b8->unk3D89C;
    Boss01* temp_r6 = boss->unk_184;
    Boss01* temp_r5 = temp_r6->unk_188;

    if (boss->unk_1C0 == 0) {
        if (temp_r6->unk_28 < 0) {
            boss->unk_28  = -0x80000;
            boss->unk_1D4 = 0x4000;
        } else {
            boss->unk_28  = (data_ov003_020e71b8->unk3D824 + 0x80000);
            boss->unk_1D4 = -0x4000;
        }

        boss->unk_2C = (s32)((u8*)temp_r4 + 0x2C);
        boss->unk_30 = -0x40000;
        func_ov003_020c4c1c(boss);
        func_ov003_020c4988(boss);
        if (temp_r5 != NULL) {
            boss->unk_1DC = BtlObs_RG_CreateTask(temp_r5->unk_002, (u8*)data_ov003_020e71b8->unk3D89C + 4);
            s32 temp_r9   = I2F(temp_r5->unk_76);

            s32 var_r0_2 = (temp_r5->unk_76 > 0) ? temp_r9 + 0.5f : temp_r9 - 0.5f;
            if (var_r0_2 > 0x80000) {
                f32 var_r0_3 = (temp_r5->unk_76 > 0) ? (temp_r9 + 0.5f) : (temp_r9 - 0.5f);
                boss->unk_30 = ((-(s32)var_r0_3) - 0x40000);
            }

            s32 temp_r2 = temp_r5->unk_76 * (temp_r5->unk_74 << 0xD);
            s32 temp_r1 = (s32)(temp_r2 + ((u32)(temp_r2 >> 5) >> 0x1A)) >> 6;
            s32 var_r1  = (s32)(temp_r1 + ((u32)(temp_r1 >> 4) >> 0x1B)) >> 5;
            if (var_r1 > 0x2000) {
                var_r1 = 0x2000;
            }
            if (boss->unk_24 == 0) {
                boss->unk_1D4 += var_r1;
            } else {
                boss->unk_1D4 -= var_r1;
            }
        }
    }
    boss->unk_28 += boss->unk_1D4;
    s32 var_r0_4 = ((u8*)temp_r4 + 0x28) - boss->unk_28;
    if (var_r0_4 < 0) {
        var_r0_4 = 0 - var_r0_4;
    }
    if (var_r0_4 < 0x80000) {
        func_ov016_02125c20(boss, 1);
    } else {
        func_ov016_02125c20(boss, 0);
    }
    if (EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, &boss->unk_1DC) != 0) {
        Boss01* temp_r0_2 = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, boss->unk_1DC);
        if (!(temp_r0_2->unk_54 & 2)) {
            temp_r0_2->unk_28 = boss->unk_28;
            temp_r0_2->unk_2C = boss->unk_2C;

            s32 var_r0_5      = (temp_r0_2->unk_72 > 0) ? I2F(temp_r0_2->unk_72) + 0.5f : I2F(temp_r0_2->unk_72) - 0.5f;
            temp_r0_2->unk_30 = (s32)(boss->unk_30 + var_r0_5);
        } else {
            func_ov016_02125bb8(boss);
        }
    } else {
        func_ov016_02125bb8(boss);
    }

    s32 var_r1_2 = boss->unk_1D4;
    s32 var_r0_6 = ((u8*)temp_r4 + 0x28) - boss->unk_28;
    if (var_r0_6 < 0) {
        var_r0_6 = -var_r0_6;
    }
    if (var_r1_2 < 0) {
        var_r1_2 = -var_r1_2;
    }
    if (var_r0_6 < var_r1_2) {
        func_ov003_020c5b0c(232, boss);
    }
    Boss01_Eff_CreateTask(boss);
    if (Boss01_RG_IsInBounds(boss) != 0) {
        func_ov003_020c4a34(boss);
        if (EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, &boss->unk_1DC) != 0) {
            Boss01* temp_r0_5 = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, boss->unk_1DC);
            if (temp_r0_5->unk_54 & 2) {
                boss->unk_188 = NULL;
            } else {
                temp_r0_5->unk_18C |= 0x1000;
                func_ov003_020c4ff4();
            }
        } else {
            boss->unk_188 = NULL;
        }

        boss->unk_28 = (boss->unk_28 < 0) ? -0x80000 : (data_ov003_020e71b8->unk3D7CC + 0x80000);

        *data_ov016_021292c0 = 2;
        Boss01_SetMoveState(boss, Boss01_MoveState_WaitToFly);
        return;
    }
    boss->unk_1C0++;
}

/// MARK: Boss01_RG

typedef struct {
    /* 0x000 */ char unk_000[0x28];
    /* 0x028 */ s32  unk_28;
    /* 0x02C */ s32  unk_2C;
    /* 0x030 */ s32  unk_30;
    /* 0x034 */ s16  unk_34;
    /* 0x036 */ s16  unk_36;
    /* 0x038 */ char unk_38[0x3C - 0x38];
    /* 0x03C */ s32  unk_3C;
    /* 0x040 */ char unk_40[0x54 - 0x40];
    /* 0x054 */ s32  unk_54;
    /* 0x058 */ char unk_58[0x84 - 0x58];
    /* 0x084 */ s32  unk_84;
    /* 0x088 */ char unk_88[0x18C - 0x88];
    /* 0x18C */ u16  unk_18C;
    /* 0x18E */ char unk_18E[0x1CC - 0x18E];
    /* 0x1CC */ void (*unk_1CC)(void*);
    /* 0x1D0 */ s32  unk_1D0;
    /* 0x1D4 */ char unk_1D4[0x1DC - 0x1D4];
    /* 0x1DC */ s32  unk_1DC;
} Boss01_RG; // Size: 0x1E0

static const TaskHandle Tsk_Boss01_RG = {"Tsk_Boss01_RG", Boss01_RG_RunTask, sizeof(Boss01_RG)};

void func_ov016_021260b4(Boss01* boss) {
    if (func_ov003_020c6f40() == 0) {
        boss->unk_1D0 = 0;
    }
}

s32 Boss01_RG_Init(TaskPool* pool, Task* task, void* args) {
    Ov016Sprite subroutine;
    Boss01_RG*  bossRG = task->data;

    MI_CpuSet(bossRG, 0, sizeof(Boss01_RG));
    func_ov003_020c3efc(bossRG, args);
    func_ov016_021256c0(&subroutine, 1, 0);
    func_ov003_02082998(&bossRG->unk_84, &subroutine);
    func_ov003_020c4c1c(bossRG);
    bossRG->unk_54 |= 0x10000000;
    bossRG->unk_3C  = 0;
    bossRG->unk_28  = -0x80000;
    bossRG->unk_2C  = 0;
    bossRG->unk_30  = -0x40000;
    bossRG->unk_1DC = -1;
    bossRG->unk_1D0 = 1;
    Boss01_SetMoveState((Boss01*)bossRG, Boss01_MoveState_WaitToFly);
    return 1;
}

s32 Boss01_RG_Update(TaskPool* pool, Task* task, void* args) {
    Boss01_RG* bossRG = task->data;

    if (func_ov003_020c3bf0(bossRG) != 0) {
        return 1;
    }

    s32 temp_r0 = func_ov003_02082f2c(bossRG);
    if ((temp_r0 != 1) && (temp_r0 == 3)) {
        Boss01_SetMoveState((Boss01*)bossRG, func_ov016_021260b4);
    }
    if (bossRG->unk_1CC != NULL) {
        bossRG->unk_1CC(bossRG);
    }
    func_ov003_02083000(1, bossRG);
    func_ov003_02082b0c(&bossRG->unk_84);
    return bossRG->unk_1D0;
}

s32 Boss01_RG_Render(TaskPool* pool, Task* task, void* args) {
    s16        spA, sp8;
    Boss01_RG* bossRG = task->data;

    if (!(bossRG->unk_18C & 1)) {
        func_ov003_02084348(1, &spA, &sp8, bossRG->unk_28, bossRG->unk_2C, bossRG->unk_30);
        func_ov003_02082724(&bossRG->unk_84, spA, sp8);
        func_ov003_02082730(&bossRG->unk_84, 0x7FF3FFFF);
        func_ov003_02082b64(&bossRG->unk_84);
        bossRG->unk_34 = spA;
        bossRG->unk_36 = sp8 + 80;
    }
    return 1;
}

s32 Boss01_RG_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01_RG* bossRG = task->data;

    func_ov003_02082cc4(&bossRG->unk_84);
    func_ov003_020c4a34(bossRG);
    return 1;
}

s32 Boss01_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02128fc8 = {
        .initialize = Boss01_RG_Init,
        .update     = Boss01_RG_Update,
        .render     = Boss01_RG_Render,
        .cleanup    = Boss01_RG_Destroy,
    };
    return data_ov016_02128fc8.iter[stage](pool, task, args);
}

/* Render a shadow/secondary sprite using actor header data. */
void func_ov016_021262d8(Ov016Obj* arg0) {
    s16   temp_r7;
    s32   temp_r4;
    s32   temp_r6;
    u16   temp_r1_2;
    u16   var_r0;
    u32   temp_r0_2;
    u32   temp_r0_3;
    void* temp_r5;

    if (func_ov003_02088130() == 0) {
        return;
    }
    Data* temp_r0 = data_ov003_020e71b8->unk3D350;

    void* var_r8 = Data_GetPackEntryData(temp_r0, 66);

    temp_r4 = arg0->unk1D8;

    if (arg0->unkCA & 1) {
        var_r0 = OamMgr_AllocAffineGroup(&g_OamMgr[DISPLAY_EXTENDED], 0, arg0->unk1DC, arg0->unk1DC, 1);
    } else {
        var_r0 = OamMgr_AllocAffineGroup(&g_OamMgr[DISPLAY_EXTENDED], 0, arg0->unk1DC, arg0->unk1DC, 0);
    }

    arg0->unk8E = (u16)((arg0->unk8E & ~0x3E0) | ((u32)(var_r0 << 0x1B) >> 0x16));
    temp_r7     = arg0->unk96;
    temp_r6     = arg0->unkB4;
    temp_r5     = arg0->unkC0;
    temp_r0_2   = 0 - temp_r7;
    arg0->unkB4 = temp_r4;
    arg0->unkC0 = var_r8;
    arg0->unk96 = (s16)((s32)(temp_r0_2 + (temp_r0_2 >> 0x1F)) >> 1);
    arg0->unk8E = (u16)((arg0->unk8E & ~1) | 1);
    arg0->unk8E = (u16)(arg0->unk8E & ~2);
    arg0->unk8E = (u16)((arg0->unk8E & ~0xC) | 4);
    func_ov003_02082848(arg0 + 0x84, 8);
    arg0->unkB4 = temp_r6;
    arg0->unkC0 = temp_r5;
    arg0->unk96 = temp_r7;
    arg0->unk8E = (u16)(arg0->unk8E & ~1);
    arg0->unk8E = (u16)(arg0->unk8E & ~2);
    arg0->unk8E = (u16)(arg0->unk8E & ~0xC);
    temp_r1_2   = arg0->unk8E;
    temp_r0_3   = temp_r1_2 << 0x16;
    if (arg0->unkCA & 1) {
        arg0->unk8E = (u16)((temp_r1_2 & ~0x3E0) | ((u32)((u16)((temp_r0_3 >> 0x1B) | 8) << 0x1B) >> 0x16));
        return;
    }
    arg0->unk8E = (u16)((temp_r1_2 & ~0x3E0) | ((u32)((u16)((temp_r0_3 >> 0x1B) & ~8) << 0x1B) >> 0x16));
}

s32 func_ov016_02126468(Boss01* boss) {
    s32 temp_lr;
    s32 var_ip;
    s32 var_ip_2;
    s32 var_r3;

    temp_lr = boss->unk_24;
    var_r3  = 1;
    var_ip  = 0;
    if ((temp_lr == 0) && ((s32)boss->unk_28 < -0x80000)) {
        var_ip = 1;
    }
    if (var_ip == 0) {
        var_ip_2 = 0;
        if ((temp_lr != 0) && ((s32)boss->unk_28 > (s32)(data_ov003_020e71b8->unk3D7CC + 0x80000))) {
            var_ip_2 = 1;
        }
        if (var_ip_2 == 0) {
            var_r3 = 0;
        }
    }
    return var_r3;
}

/* Spawn an obstacle effect sequence. */
void func_ov016_021264d8(void*) {
    u8 subroutine[0x14];

    func_ov003_020c3c44(subroutine, 7, 3);
    func_ov003_020c3cec(&data_ov009_02123540, &data_ov009_021234cc, subroutine, subroutine);
}

/* Validate potential target based on collision and status flags. */
s32 func_ov016_02126514(Boss01* boss, UnkStruct_Boss01* arg1) {
    if ((func_ov003_020c5a28(boss, (u8*)arg1 + 4) != 0) && (arg1->unk_44 == 0) && (arg1->unk_48 == 0) && (arg1->unk_34 == 0)) {
        return 1;
    }
    return 0;
}

/* Validate target based on grounded/airborne state. */
s32 func_ov016_02126554(Boss01* boss, UnkStruct_Boss01* arg1, s32 arg2) {
    if ((arg2 != 0) && (func_ov009_020fa71c(arg1) == 0)) {
        return 0;
    }
    if ((arg2 == 0) && (func_ov009_020fa71c(arg1) != 0)) {
        return 0;
    }
    return func_ov016_02126514(boss, arg1);
}

/* Pick a random eligible target from the active list. */
UnkStruct_Boss01* func_ov016_021265a8(Boss01* boss, s32 arg1) {
    s32               temp_r5;
    s32               var_r9;
    UnkStruct_Boss01* var_r4;
    UnkStruct_Boss01* var_r8;

    var_r4  = NULL;
    temp_r5 = RNG_Next(func_ov003_020cc5fc(0, 0x19));
    var_r8  = func_ov003_0208495c(OV003_LIST_3D8B4);
    var_r9  = 0;
    if (var_r8 != NULL) {
    loop_1:
        if (func_ov016_02126554(boss, var_r8, arg1) != 0) {
            if (var_r9 < temp_r5) {
                var_r4 = var_r8;
                goto block_5;
            }
            return var_r8;
        }
    block_5:
        var_r8 = func_ov003_02084984((u8*)var_r8 + 0xB8);
        var_r9 += 1;
        if (var_r8 == NULL) {
            /* Duplicate return node #6. Try simplifying control flow for better match */
            return var_r4;
        }
        goto loop_1;
    }
    return var_r4;
}

/* Select a preferred target, favoring a previous selection. */
UnkStruct_Boss01* func_ov016_02126638(Boss01* boss) {
    s16               temp_r1_2;
    UnkStruct_Boss01* temp_r1;
    UnkStruct_Boss01* temp_r4;
    UnkStruct_Boss01* var_r0;

    temp_r4   = func_ov016_021265a8(boss, 0);
    var_r0    = func_ov016_021265a8(boss, 1);
    temp_r1_2 = boss->unk_6E;
    if ((s32)boss->unk_6C <= (s32)((s32)(temp_r1_2 + ((u32)temp_r1_2 >> 0x1F)) >> 1)) {
        temp_r1 = boss->unk_1E4;
        if (temp_r1 != NULL) {
            return temp_r1;
        }
    }
    if (var_r0 == NULL) {
        var_r0 = temp_r4;
    }
    return var_r0;
}

void Boss01_SetAiState(Boss01* boss, void (*arg1)(Boss01*)) {
    func_ov003_020c427c();
    boss->unk_1C8 = arg1;
    boss->unk_1C4 = 0;
    boss->unk_1C0 = 0;
    boss->unk_1EC = 0;
}

void func_ov016_021266b4(Boss01* boss) {
    if (func_ov003_020cb3c4(0, 7) == 0) {
        *data_ov016_021292c0 = 1;
        Boss01_SetAiState(boss, func_ov016_021266f4);
    }
}

void func_ov016_021266f4(Boss01* boss) {
    switch (boss->unk_1C4) {
        case 0: {
            if (boss->unk_1C0 <= 180) {
                boss->unk_1C0++;
            } else {
                boss->unk_1C0 = 0;
                boss->unk_1C4++;
            }
        } break;

        case 1: {
            boss->unk_1C4++;
        } break;

        case 2: {
            Boss01_SetAiState(boss, func_ov016_02126c24);
        } break;

        case 3: {
            EasyFade_FadeBothDisplays(FADER_LINEAR, -16, 0x400);
            if (EasyFade_IsFading() == FALSE) {
                boss->unk_1C4++;
            }
        } break;

        case 4: {
            if (boss->unk_1C0 == 0) {
                EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x400);
                func_ov003_020c3ec0(boss);
                Boss01s_LandRG_CreateTask();
                Boss01s_LandUG_CreateTask();
                Boss01s_FlyRG_Curve_CreateTask();
                Boss01s_FlyUG_CreateTask();
                for (s32 i = 0; i < 4; i++) {
                    func_ov016_021264d8(boss);
                }
            }
            if (EasyFade_IsFading() == FALSE) {
                *data_ov016_021292c0 = 2;
                Boss01_SetAiState(boss, func_ov016_02126854);
            } else {
                boss->unk_1C0++;
            }
        } break;
    }
}

void func_ov016_0212682c(Boss01* boss) {
    if (*data_ov016_021292c0 == 2) {
        Boss01_SetAiState(boss, func_ov016_02126854);
    }
}

void func_ov016_02126854(Boss01* boss) {
    s32               sp8;
    s32               var_r3;
    s16               temp_r1_2;
    s32               temp_r3;
    s32               temp_r3_2;
    s32               var_r0;
    u32               temp_r1;
    UnkStruct_Boss01* temp_r0_2;
    Ov016Obj*         temp_r4;

    temp_r4 = data_ov003_020e71b8->unk3D898;
    if (boss->unk_1C0 == 0) {
        func_ov003_020c4c5c(boss);
        boss->unk_54  = (s32)(boss->unk_54 | 0x10);
        boss->unk_1DC = 0x666;
        boss->unk_2C  = (s32)temp_r4->unk2C;
        boss->unk_30  = -0x200000;
        var_r0        = 0x3000;
        if (boss->unk_24 == 0) {
            var_r0 = -0x3000;
        }
        boss->unk_1D0 = var_r0;
        if ((boss->unk_188 == 0) && (boss->unk_1E0 != NULL)) {
            func_ov003_02086654();
            boss->unk_1E8 = 0;
            boss->unk_1E0 = NULL;
        }
        if (boss->unk_1E8 != 0) {
            boss->unk_1EC = (RNG_Next(2) == 0) ? 1 : 0;
            if (boss->unk_1EC != 0) {
                temp_r3       = FX_Divide(0x2800, boss->unk_1E0->unk_4C) >> 0xC;
                temp_r1       = temp_r3 * (boss->unk_1E0->unk_4C * temp_r3);
                boss->unk_1F0 = (s32)((temp_r3 * 0x2800) - ((s32)(temp_r1 + (temp_r1 >> 0x1F)) >> 1));
            }
        }
    }
    boss->unk_28 = (s32)(boss->unk_28 + boss->unk_1D0);
    if (boss->unk_1E8 != 0) {
        sp8       = boss->unk_28;
        temp_r1_2 = boss->unk_1E0->unk_74;
        func_ov003_02083bfc((u8*)boss + 0x2C, ((s32)(temp_r1_2 + ((u32)temp_r1_2 >> 0x1F)) >> 1) << 0xC, boss->unk_28);
        boss->unk_1E0->unk_2C = boss->unk_28;
        boss->unk_1E0->unk_30 = boss->unk_2C;
        boss->unk_1E0->unk_34 = boss->unk_30;
        if ((boss->unk_1EC != 0) &&
            (((temp_r3_2 = boss->unk_1D0, (temp_r3_2 > 0)) && ((s32)boss->unk_28 > (s32)(temp_r4->unk28 - boss->unk_1F0))) ||
             ((temp_r3_2 < 0) && ((s32)boss->unk_28 < (s32)(temp_r4->unk28 + boss->unk_1F0)))) &&
            (func_ov003_020c3960(&sp8, boss->unk_1E0->unk_74 << 0xC) == 0))
        {
            var_r3 = 0x2800;
            if (boss->unk_24 == 0) {
                var_r3 = -0x2800;
            }
            func_ov003_020c5ac0(boss, 0xE5, (u8*)boss->unk_1E0 + 4, var_r3);

            if (boss->unk_1E0 == boss->unk_1E4) {
                BtlEff_Fall_CreateTask(boss->unk_1E0, boss, 231);
                boss->unk_1E0->unk_34 = -0x120000;
                boss->unk_1E4         = 0;
            } else {
                temp_r0_2 = boss->unk_1E0;
                if (func_ov009_020fa71c(temp_r0_2) != 0) {
                    BtlEff_Fall_CreateTask(temp_r0_2, boss, 230);
                } else {
                    BtlEff_Fall_CreateTask(temp_r0_2, boss, 401);
                }
            }
            boss->unk_1E0 = NULL;
            boss->unk_188 = 0;
            boss->unk_1E8 = 0;
        }
    }
    if (func_ov016_02126468(boss) != 0) {
        UnkStruct_Boss01* temp_r0_3 = func_ov016_02126638(boss);
        if (boss->unk_1E8 != 0) {
            *data_ov016_021292c0 = 2;
            Boss01_SetAiState(boss, func_ov016_02126854);
            return;
        }
        if ((temp_r0_3 == boss->unk_1E4) && (temp_r0_3 != NULL)) {
            if ((s32)boss->unk_28 < 0) {
                *data_ov016_021292c0 = 2;
                Boss01_SetAiState(boss, func_ov016_02126854);
                return;
            }
            *data_ov016_021292c0 = 3;
            Boss01_SetAiState(boss, func_ov016_02126c24);
            return;
        }
        if (func_ov003_0208495c(OV003_LIST_3D8B4) != NULL) {
            *data_ov016_021292c0 = 3;
            Boss01_SetAiState(boss, func_ov016_02126c24);
            return;
        }
        if (RNG_Next(2) != 0) {
            *data_ov016_021292c0 = 2;
            Boss01_SetAiState(boss, func_ov016_02126854);
            return;
        }
        *data_ov016_021292c0 = 3;
        Boss01_SetAiState(boss, func_ov016_02126c24);
        return;
    }
    boss->unk_1C0++;
}

void func_ov016_02126c24(Boss01* boss) {
    Ov016Obj* temp_r5 = data_ov003_020e71b8->unk3D898;

    if (boss->unk_1C0 == 0) {
        func_ov003_020c4c5c(boss);
        boss->unk_54  = (s32)(boss->unk_54 | 0x10);
        boss->unk_1DC = 0xCCD;
        boss->unk_2C  = (s32)temp_r5->unk2C;
        boss->unk_30  = -0x12C000;
        if (boss->unk_24 == 0) {
            boss->unk_1D0 = -0x4000;
        } else {
            boss->unk_1D0 = 0x4000;
        }
        if (*data_ov016_021292c0 == 1) {
            func_ov003_020c4cc4(boss, 498);
        }
    }
    boss->unk_28 += boss->unk_1D0;
    if (func_ov016_02126468(boss) != 0) {
        if (*data_ov016_021292c0 == 1) {
            Boss01_SetAiState(boss, func_ov016_021266f4);
            boss->unk_1C4 = 3;
            return;
        }
        *data_ov016_021292c0 = 4;
        Boss01_SetAiState(boss, func_ov016_02126d38);
        return;
    }
    boss->unk_1C0++;
}

void func_ov016_02126d38(Boss01* boss) {
    UnkStruct_Boss01* temp_r4 = data_ov003_020e71b8->unk3D898;

    if (boss->unk_1C0 == 0) {
        boss->unk_1E0 = func_ov016_02126638(boss);
        func_ov003_020c4c5c(boss);
        boss->unk_54 &= ~0x10;
        boss->unk_1DC = 0x1000;
        boss->unk_1D4 = 0;
        if (boss->unk_1E0 != NULL) {
            boss->unk_2C  = (s32)boss->unk_1E0->unk_30;
            s32 temp_r0_3 = boss->unk_1E0->unk_76 << 0xC;

            f32 var_r0;
            if ((s32)boss->unk_1E0->unk_76 > 0) {
                var_r0 = (f32)temp_r0_3 + 0.5f;
            } else {
                var_r0 = (f32)temp_r0_3 - 0.5f;
            }
            boss->unk_30 = -(s32)var_r0 - 0x8000;
        } else {
            boss->unk_2C = (s32)temp_r4->unk_2C;
            boss->unk_30 = -0x40000;
        }

        s32 val = -0x4000;
        if (boss->unk_24 != 0) {
            val = 0x4000;
        }

        boss->unk_1D0 = val;

        func_ov003_020c4cc4(boss, 0x1F2);
    }
    boss->unk_28 = (s32)(boss->unk_28 + boss->unk_1D0);
    if (boss->unk_1E0 != NULL) {
        if (boss->unk_1E8 != 0) {
            boss->unk_1E0->unk_2C = (s32)boss->unk_28;
            boss->unk_1E0->unk_30 = (s32)boss->unk_2C;
            boss->unk_1E0->unk_34 = -0x8000;
        }

        if ((boss->unk_28 >= (s32)(boss->unk_1E0->unk_2C - 0x4000)) &&
            (boss->unk_28 < (s32)(boss->unk_1E0->unk_2C + 0x4000)) && (func_ov016_02126514(boss, boss->unk_1E0) != 0))
        {
            func_ov003_020c5a00((u8*)boss->unk_1E0 + 4);
            boss->unk_1E0->unk_5C = 6;
            boss->unk_1E8         = 1;
            boss->unk_188         = (void*)boss->unk_1E0;
            func_ov003_020c4cc4(boss, 0x1F3);

            s32 temp_r2_3 = boss->unk_1E0->unk_76 * (boss->unk_1E0->unk_74 << 0xD);
            s32 temp_r1_2 = (s32)(temp_r2_3 + ((u32)(temp_r2_3 >> 5) >> 0x1A)) >> 6;
            s32 var_r1    = (s32)(temp_r1_2 + ((u32)(temp_r1_2 >> 4) >> 0x1B)) >> 5;

            if (var_r1 > 0x2000) {
                var_r1 = 0x2000;
            }
            if (boss->unk_24 == 0) {
                boss->unk_1D0 += var_r1;
            } else {
                boss->unk_1D0 -= var_r1;
            }
        }
    }
    if (boss->unk_1D4 == 0) {
        boss->unk_1D4 = func_ov003_020c5b2c(0xE4, boss, boss->unk_28, boss->unk_2C, boss->unk_30);
    }
    Boss01_Eff_CreateTask(boss);
    if (func_ov016_02126468(boss) != 0) {
        boss->unk_54 |= 0x10;
        if (boss->unk_1E8 == 0) {
            boss->unk_1E0 = NULL;
        }
        if ((boss->unk_1E4 != 0) && (boss->unk_1E0 == boss->unk_1E4)) {
            *data_ov016_021292c0 = 2;
            Boss01_SetAiState(boss, func_ov016_0212682c);
            return;
        }
        *data_ov016_021292c0 = 5;
        Boss01_SetAiState(boss, func_ov016_0212682c);
        return;
    }
    boss->unk_1C0++;
}

/// MARK: Boss01_UG

typedef struct {
    /* 0x000 */ char unk_000[0x28];
    /* 0x028 */ s32  unk_28;
    /* 0x02C */ s32  unk_2C;
    /* 0x030 */ s32  unk_30;
    /* 0x034 */ s16  unk_34;
    /* 0x036 */ s16  unk_36;
    /* 0x038 */ s16  unk_38;
    /* 0x03A */ s16  unk_3A;
    /* 0x03C */ s32  unk_3C;
    /* 0x040 */ char unk_040[0x54 - 0x40];
    /* 0x054 */ s32  unk_54;
    /* 0x058 */ char unk_58[0x84 - 0x58];
    /* 0x084 */ s32  unk_84;
    /* 0x088 */ char unk_88[0xCA - 0x88];
    /* 0x0CA */ u16  unk_CA;
    /* 0x0CC */ char unk_CC[0x18C - 0xCC];
    /* 0x18C */ u16  unk_18C;
    /* 0x18E */ char unk_18E[0x1C0 - 0x18E];
    /* 0x1C0 */ s16  unk_1C0;
    /* 0x1C2 */ char unk_1C2[0x1C8 - 0x1C2];
    /* 0x1C8 */ void (*unk_1C8)(void*);
    /* 0x1CC */ s32              unk_1CC;
    /* 0x1D0 */ char             unk_1D0[0x1D8 - 0x1D0];
    /* 0x1D8 */ PaletteResource* unk_1D8;
    /* 0x1DC */ s32              unk_1DC;
    /* 0x1E0 */ char             unk_1E0[0x1E4 - 0x1E0];
    /* 0x1E4 */ void*            unk_1E4;
} Boss01_UG; // Size: 0x1F4

static const TaskHandle Tsk_Boss01_UG = {"Tsk_Boss01_UG", Boss01_UG_RunTask, 0x1F4};

void func_ov016_02126ff0(Boss01* boss) {
    Ov016Sprite subroutine;
    s32         temp_r5;

    if (boss->unk_1C0 == 0) {
        func_ov016_021256c0(&subroutine, 0, boss->unk_CA & 1);
        func_ov003_020c9064(&subroutine);
    }
    temp_r5 = func_ov003_020c9338(boss);
    if ((func_ov003_020c9008() != 0) && (data_ov016_021291d8 != 0)) {
        func_ov003_02082cc4((u8*)boss + 0x84);
        data_ov016_021291d8 = 0;
    }
    if (temp_r5 == 0) {
        func_ov003_020c50bc();
    }
    if ((temp_r5 == 0) && (EasyFade_IsFading() == 0)) {
        boss->unk_1CC = NULL;
        return;
    }
    boss->unk_1C0++;
}

s32 Boss01_UG_Init(TaskPool* pool, Task* task, void* args) {
    Ov016Sprite subroutine;
    Boss01_UG*  boss = task->data;

    MI_CpuSet(boss, 0, 0x1F4);

    void* src     = Data_GetPackEntryData(data_ov003_020e71b8->unk3D350, 66);
    boss->unk_1D8 = PaletteMgr_AcquireContiguous(g_PaletteManagers[DISPLAY_EXTENDED], src, 5, 1);

    func_ov003_020c3efc(boss, args);
    func_ov016_021256c0(&subroutine, 2, 0);
    func_ov003_02082998(&boss->unk_84, &subroutine);
    func_ov003_020c49c8(boss);
    func_ov003_020c4c5c(boss);
    boss->unk_54 |= 0x10000010;
    boss->unk_3C         = 0;
    boss->unk_28         = -0x80000;
    boss->unk_2C         = 0;
    boss->unk_30         = -0x40000;
    boss->unk_1DC        = 0x1000;
    boss->unk_1CC        = 1;
    boss->unk_1E4        = BtlObs_Bus_CreateTask(boss);
    *data_ov016_021292c0 = 0;
    Boss01_SetAiState((Boss01*)boss, func_ov016_021266b4);
    return 1;
}

s32 Boss01_UG_Update(TaskPool* pool, Task* task, void* args) {
    Boss01_UG* bossUG = task->data;

    if (func_ov003_020c3bf0(bossUG) != 0) {
        return 1;
    }

    s32 temp_r0 = func_ov003_02082f2c(bossUG);
    if ((temp_r0 != 1) && (temp_r0 == 3)) {
        Boss01_SetAiState((Boss01*)bossUG, func_ov016_02126ff0);
    }

    if (bossUG->unk_1C8 != NULL) {
        bossUG->unk_1C8(bossUG);
    }

    if (((s32)*data_ov016_021292c0 > 1) && (func_ov003_020cb3c4(0, 7) < 4) && (RNG_Next(180) == 0) && !(bossUG->unk_54 & 4)) {
        func_ov016_021264d8(bossUG);
    }
    func_ov003_02083000(0, bossUG);
    func_ov003_02082b0c(&bossUG->unk_84);
    return bossUG->unk_1CC;
}

s32 Boss01_UG_Render(TaskPool* pool, Task* task, void* args) {
    s16        spA, sp8;
    Boss01_UG* bossUG = task->data;

    if (bossUG->unk_54 & 1) {
        bossUG->unk_54 &= ~1;
    }

    if (!(bossUG->unk_18C & 1)) {
        func_ov003_02084348(0, &spA, &sp8, bossUG->unk_28, bossUG->unk_2C, bossUG->unk_30);
        func_ov003_02082724(&bossUG->unk_84, spA, sp8);
        func_ov003_02082b64(&bossUG->unk_84);
        bossUG->unk_34 = spA;
        bossUG->unk_36 = sp8 + 80;
        func_ov003_02084348(0, &spA, &sp8, bossUG->unk_28, bossUG->unk_2C, 0);
        func_ov003_02082724(&bossUG->unk_84, spA, sp8);
        func_ov016_021262d8((Ov016Obj*)bossUG);
    }
    return 1;
}

s32 Boss01_UG_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01_UG* bossUG = task->data;

    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_EXTENDED], bossUG->unk_1D8);
    func_ov003_02082cc4(&bossUG->unk_84);
    func_ov003_020c4a74(bossUG);
    return 1;
}

s32 Boss01_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02128fe4 = {
        .initialize = Boss01_UG_Init,
        .update     = Boss01_UG_Update,
        .render     = Boss01_UG_Render,
        .cleanup    = Boss01_UG_Destroy,
    };
    return data_ov016_02128fe4.iter[stage](pool, task, args);
}

/// MARK: Boss01s_FlyRG_Curve

typedef struct {
    /* 0x00 */ char unk_00[0x60];
    /* 0x60 */ Vec  unk_60;
    /* 0x6C */ s32  unk_6C;
    /* 0x70 */ s32  unk_70;
    /* 0x74 */ s32  unk_74;
    /* 0x78 */ s32  unk_78;
    /* 0x7C */ s32  unk_7C;
    /* 0x80 */ s32  unk_80;
    /* 0x84 */ s16  unk_84;
    /* 0x86 */ s16  unk_86;
    /* 0x88 */ s16  unk_88;
} Boss01s_FlyRG_Curve; // Size: 0x8C

static const TaskHandle Tsk_Boss01s_FlyRG_Curve = {"Tsk_Boss01s_FlyRG_Curve", Boss01s_FlyRG_Curve_RunTask, 0x8C};

void Boss02s_FlyRG_Curve_Randomize(Boss01s_FlyRG_Curve* arg0) {
    arg0->unk_6C = RNG_Next(3278) + 3277;
    arg0->unk_74 = RNG_Next(2458) + 2458;
    arg0->unk_80 = -(RNG_Next(25) + 25);

    f32 var_r0;
    if (RNG_Next(F2I(data_ov003_020e71b8->unk3D824)) != 0) {
        var_r0 = I2F(RNG_Next(F2I(data_ov003_020e71b8->unk3D824))) + 0.5f;
    } else {
        var_r0 = I2F(RNG_Next(F2I(data_ov003_020e71b8->unk3D824))) - 0.5f;
    }
    arg0->unk_60.x = var_r0;
    arg0->unk_60.y = 0;

    s64 temp_r3    = ((s64)arg0->unk_74 * (s64)arg0->unk_74) + 0x800;
    s32 temp_r5_2  = RNG_Next(29) + 100;
    arg0->unk_60.z = -I2F(temp_r5_2) + (FX_Divide(F2I(temp_r3), arg0->unk_80) / 2);

    if (RNG_Next(4) == 0) {
        arg0->unk_6C *= -1;
        arg0->unk_60.x += 0x80000;
        arg0->unk_60.z -= 0x14000;
    } else {
        arg0->unk_60.x -= 0x80000;
    }
}

s32 Boss01s_FlyRG_Curve_Init(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyRG_Curve* curve = task->data;

    MI_CpuSet(curve, 0, sizeof(Boss01s_FlyRG_Curve));
    func_ov003_02082a04(1, curve, &data_ov016_02128f98, &data_ov016_02129010, RNG_Next(2), 4, 4);
    Boss02s_FlyRG_Curve_Randomize(curve);

    f32 var_r0;
    if (RNG_Next(F2I(data_ov003_020e71b8->unk3D824)) != 0) {
        var_r0 = I2F(RNG_Next(F2I(data_ov003_020e71b8->unk3D824))) + 0.5f;
    } else {
        var_r0 = I2F(RNG_Next(F2I(data_ov003_020e71b8->unk3D824))) - 0.5f;
    }

    curve->unk_60.x = var_r0;
    return 1;
}

s32 Boss01s_FlyRG_Curve_Update(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyRG_Curve* curve = task->data;

    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    switch (curve->unk_88) {
        case 0:
            curve->unk_60.x += curve->unk_6C;
            curve->unk_74 += curve->unk_80;
            curve->unk_60.z += curve->unk_74;
            if (((curve->unk_6C < 0) && (curve->unk_60.x < -0x10000)) ||
                ((curve->unk_6C > 0) && (curve->unk_60.x > (data_ov003_020e71b8->unk3D824 + 0x10000))))
            {
                curve->unk_88++;
            }
            break;
        case 1:
            if (curve->unk_84 == 0) {
                curve->unk_86 = RNG_Next(180);
            }
            if (curve->unk_84 <= curve->unk_86) {
                curve->unk_84++;
            } else {
                Boss02s_FlyRG_Curve_Randomize(curve);
                curve->unk_84 = 0;
                curve->unk_88 = 0;
            }
            break;
    }
    func_ov003_02082b0c(curve);
    return 1;
}

s32 Boss01s_FlyRG_Curve_Render(TaskPool* pool, Task* task, void* args) {
    s16                  spA, sp8;
    Boss01s_FlyRG_Curve* curve = task->data;

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(curve), &spA, &sp8, curve->unk_60.x, curve->unk_60.y, curve->unk_60.z);
    func_ov003_02082724(curve, spA, sp8);
    func_ov003_02082730(curve, 0x7FFFFFFF);
    func_ov003_02082b64(curve);
    return 1;
}

s32 Boss01s_FlyRG_Curve_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyRG_Curve* curve = task->data;
    func_ov003_02082cc4(curve);
    return 1;
}

s32 Boss01s_FlyRG_Curve_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02129000 = {
        .initialize = Boss01s_FlyRG_Curve_Init,
        .update     = Boss01s_FlyRG_Curve_Update,
        .render     = Boss01s_FlyRG_Curve_Render,
        .cleanup    = Boss01s_FlyRG_Curve_Destroy,
    };
    return data_ov016_02129000.iter[stage](pool, task, args);
}

void Boss01s_FlyRG_Curve_CreateTask(void) {
    for (s32 i = 0; i < 5; i++) {
        EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &Tsk_Boss01s_FlyRG_Curve, NULL, 0, NULL, NULL);
    }
}

/// MARK: Boss01s_FlyUG

typedef struct {
    /* 0x00 */ char unk_00[0x60];
    /* 0x60 */ Vec  unk_60;
    /* 0x6C */ Vec  unk_6C;
    /* 0x78 */ s16  unk_78;
    /* 0x7A */ s16  unk_7A;
    /* 0x7C */ s16  unk_7C;
} Boss01s_FlyUG; // Size: 0x80

static const TaskHandle Tsk_Boss01s_FlyUG = {"Tsk_Boss01s_FlyUG", Boss01s_FlyUG_RunTask, sizeof(Boss01s_FlyUG)};

void Boss01s_FlyUG_GetSpawnPos(Boss01s_FlyUG* arg0, Vec* arg1) {
    s32 width = F2I(data_ov003_020e71b8->unk3D7CC);

    if (RNG_Next(2) != 0) {
        arg1->x = I2F((width / 2 - 0x30) + RNG_Next(65));
        arg1->y = I2F(RNG_Next(17) - 38);
        arg1->z = 0;
    } else {
        arg1->x = I2F(width / 2 + 48 + RNG_Next(width - (width / 2 + 47)));
        arg1->y = I2F(RNG_Next(17) - 38);
        arg1->z = 0;
    }
}

s32 Boss01s_FlyUG_Init(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyUG* flyUG = task->data;

    MI_CpuSet(flyUG, 0, sizeof(Boss01s_FlyUG));
    func_ov003_02082a04(0, flyUG, &data_ov016_02128f98, &data_ov016_0212903c, 0, 4, 4);
    Boss01s_FlyUG_GetSpawnPos(flyUG, &flyUG->unk_60);
    return 1;
}

s32 Boss01s_FlyUG_Update(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyUG* flyUG = task->data;

    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    switch (flyUG->unk_7C) {
        case 0:
            if (flyUG->unk_78 == 0) {
                func_ov003_020824a0(flyUG, (RNG_Next(2) + 1), 0);
                flyUG->unk_7A = (RNG_Next(301) + 300);
            }
            if (flyUG->unk_78 <= flyUG->unk_7A) {
                flyUG->unk_78++;
            } else {
                flyUG->unk_78 = 0;
                flyUG->unk_7C++;
            }
            break;
        case 1:
            if (flyUG->unk_78 == 0) {
                func_ov003_020824a0(flyUG, 0, 0);
                flyUG->unk_6C.x = (RNG_Next(2) == 0) ? -0x1000 : 0x1000;
                flyUG->unk_6C.z = -0x2000;
            }

            flyUG->unk_6C.z -= 0x800;
            if (flyUG->unk_6C.z < -0x4000) {
                flyUG->unk_6C.z = -0x4000;
            }

            flyUG->unk_60.x += flyUG->unk_6C.x;
            flyUG->unk_60.z += flyUG->unk_6C.z;

            if (flyUG->unk_60.z < -0x60000) {
                flyUG->unk_78 = 0;
                flyUG->unk_7C++;
            } else {
                flyUG->unk_78++;
            }
            break;
        case 2:
            if (flyUG->unk_78 == 0) {
                flyUG->unk_7A = (s16)(RNG_Next(0x12D) + 0x12C);
            }
            if (flyUG->unk_78 <= flyUG->unk_7A) {
                flyUG->unk_78++;

            } else {
                flyUG->unk_78 = 0;
                flyUG->unk_7C++;
            }
            break;
        case 3:
            if (flyUG->unk_78 == 0) {
                Boss01s_FlyUG_GetSpawnPos(flyUG, &flyUG->unk_6C);
                if (RNG_Next(2) != 0) {
                    flyUG->unk_60.x = (flyUG->unk_6C.x + 0x20000);
                } else {
                    flyUG->unk_60.x = (flyUG->unk_6C.x - 0x20000);
                }
                flyUG->unk_78 = 0x1E;
                goto block_35;
            }
        block_35:
            if (flyUG->unk_78 > 0) {
                func_02026590(&flyUG->unk_60.x, flyUG->unk_6C.x, flyUG->unk_78);
                func_020265d4(&flyUG->unk_60.y, flyUG->unk_6C.y, flyUG->unk_78);
                func_020265d4(&flyUG->unk_60.z, flyUG->unk_6C.z, flyUG->unk_78);
                flyUG->unk_78--;
            }
            if (flyUG->unk_78 <= 0) {
                flyUG->unk_78 = 0;
                flyUG->unk_7C = 0;
            }
            break;
    }
    func_ov003_02082b0c(flyUG);
    return 1;
}

s32 Boss01s_FlyUG_Render(TaskPool* pool, Task* task, void* args) {
    s16            spA, sp8;
    Boss01s_FlyUG* flyUG = task->data;

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(flyUG), &spA, &sp8, flyUG->unk_60.x, flyUG->unk_60.y, flyUG->unk_60.z);
    func_ov003_02082724(flyUG, spA, sp8);
    func_ov003_02082730(flyUG, 0x7FFFFFFF);
    func_ov003_02082b64(flyUG);
    return 1;
}

s32 Boss01s_FlyUG_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyUG* flyUG = task->data;

    func_ov003_02082cc4(flyUG);
    return 1;
}

s32 Boss01s_FlyUG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_0212902c = {
        .initialize = Boss01s_FlyUG_Init,
        .update     = Boss01s_FlyUG_Update,
        .render     = Boss01s_FlyUG_Render,
        .cleanup    = Boss01s_FlyUG_Destroy,
    };
    return data_ov016_0212902c.iter[stage](pool, task, args);
}

void Boss01s_FlyUG_CreateTask(void) {
    for (s32 i = 0; i < 5; i++) {
        EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Tsk_Boss01s_FlyUG, 0, 0, 0, NULL);
    }
}

/// MARK: Boss01s_LandRG

static const TaskHandle Tsk_Boss01s_LandRG = {"Tsk_Boss01s_LandRG", Boss01s_LandRG_RunTask, 0x70};

/* Task stage 0: initialize landing RG effect. */
s32 Boss01s_LandRG_Init(TaskPool* pool, Task* task, void* args) {
    u32       temp_r1;
    Ov016Obj* temp_r4 = task->data;

    MI_CpuSet(temp_r4, 0, 0x70);
    func_ov003_02082a04(1, temp_r4, &data_ov016_02128f98, &data_ov016_02129054, 0, 4, 0x3E);
    temp_r1        = data_ov003_020e71b8->unk3D824;
    temp_r4->unk60 = (s32)((s32)(temp_r1 + (temp_r1 >> 0x1F)) >> 1);
    temp_r4->unk64 = 0;
    temp_r4->unk68 = -0x4B000;
    return 1;
}

/* Task stage 1: update landing RG effect. */
s32 Boss01s_LandRG_Update(TaskPool* pool, Task* task, void* args) {
    Ov016Obj* temp_r4 = task->data;

    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02082b0c(temp_r4);
    return 1;
}

s32 Boss01s_LandRG_Render(TaskPool* pool, Task* task, void* args) {
    s16       spA, sp8;
    Ov016Obj* temp_r4 = task->data;

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk60, temp_r4->unk64, temp_r4->unk68);
    func_ov003_02082724(temp_r4, spA, sp8);
    func_ov003_02082730(temp_r4, 0x7FFFFFFF);
    func_ov003_02082b64(temp_r4);
    return 1;
}

s32 Boss01s_LandRG_Destroy(TaskPool* pool, Task* task, void* args) {
    func_ov003_02082cc4(task->data);
    return 1;
}

s32 Boss01s_LandRG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02129068 = {
        .initialize = Boss01s_LandRG_Init,
        .update     = Boss01s_LandRG_Update,
        .render     = Boss01s_LandRG_Render,
        .cleanup    = Boss01s_LandRG_Destroy,
    };
    return data_ov016_02129068.iter[stage](pool, task, args);
}

void Boss01s_LandRG_CreateTask(void) {
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Tsk_Boss01s_LandRG, 0, 0, 0, NULL);
}

/// MARK: Boss01s_LandUG

typedef struct {
    /* 0x00 */ char unk_00[0x60];
    /* 0x60 */ s32  unk_60;
    /* 0x64 */ s32  unk_64;
    /* 0x68 */ s32  unk_68;
    /* 0x6C */ char unk_6C[0x78 - 0x6C];
} Boss01s_LandUG; // Size: 0x78

static const TaskHandle Tsk_Boss01s_LandUG = {"Tsk_Boss01s_LandUG", Boss01s_LandUG_RunTask, 0x78};

s32 Boss01s_LandUG_Init(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandUG* landUG = task->data;

    MI_CpuSet(landUG, 0, sizeof(Boss01s_LandUG));
    func_ov003_02082a04(0, landUG, &data_ov016_02128f98, &data_ov016_02129078, 0, 4, 0xC);

    u32 temp_r0    = data_ov003_020e71b8->unk3D7CC;
    landUG->unk_60 = (s32)((s32)(temp_r0 + (temp_r0 >> 0x1F)) >> 1);
    landUG->unk_64 = -0x1E000;
    landUG->unk_68 = 0;
    return 1;
}

s32 Boss01s_LandUG_Update(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandUG* landUG = task->data;

    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02082b0c(landUG);
    return 1;
}

s32 Boss01s_LandUG_Render(TaskPool* pool, Task* task, void* args) {
    s16             spA, sp8;
    Boss01s_LandUG* landUG = task->data;

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(landUG), &spA, &sp8, landUG->unk_60, landUG->unk_64, landUG->unk_68);
    func_ov003_02082724(landUG, spA, sp8);
    func_ov003_02082730(landUG, 0x7FFFFFFF);
    func_ov003_02082b64(landUG);
    return 1;
}

s32 Boss01s_LandUG_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandUG* landUG = task->data;
    func_ov003_02082cc4(landUG);
    return 1;
}

s32 Boss01s_LandUG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_0212908c = {
        .initialize = Boss01s_LandUG_Init,
        .update     = Boss01s_LandUG_Update,
        .render     = Boss01s_LandUG_Render,
        .cleanup    = Boss01s_LandUG_Destroy,
    };
    return data_ov016_0212908c.iter[stage](pool, task, args);
}

void Boss01s_LandUG_CreateTask(void) {
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Tsk_Boss01s_LandUG, 0, 0, 0, NULL);
}

/// MARK: BtlEff_Bomb

typedef struct {
    /* 0x00 */ Vec   unk_00;
    /* 0x0C */ Quad* unk_0C;
    /* 0x10 */ s32   unk_10;
    /* 0x14 */ s16   unk_14;
} BtlEff_Bomb_Args;

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ char   unk_40[0x20];
    /* 0x60 */ Vec    unk_60;
    /* 0x6C */ Quad   unk_6C;
    /* 0x74 */ s32    unk_74;
    /* 0x78 */ s32    unk_78;
    /* 0x7C */ s16    unk_7C;
    /* 0x7E */ s16    unk_7E;
    /* 0x80 */ s16    unk_80;
} BtlEff_Bomb; // Size: 0x84

static const TaskHandle Task_BtlEff_Bomb = {"Task_BtlEff_Bomb", BtlEff_Bomb_RunTask, 0x84};

s32 BtlEff_Bomb_Init(TaskPool* pool, Task* task, void* args) {
    BtlEff_Bomb_Args* bombArgs = (BtlEff_Bomb_Args*)args;
    BtlEff_Bomb*      eff      = task->data;

    MI_CpuSet(eff, 0, sizeof(BtlEff_Bomb));

    eff->unk_60 = bombArgs->unk_00;
    eff->unk_74 = bombArgs->unk_10;
    eff->unk_7C = bombArgs->unk_14;
    eff->unk_6C = *bombArgs->unk_0C;

    func_ov003_02082a04(func_ov003_020c3aac(eff->unk_74), eff, &data_ov016_0212909c, &data_ov016_021290a4, 0, 7, 0);
    func_ov003_020c38e8(eff);
    return 1;
}

s32 BtlEff_Bomb_Update(TaskPool* pool, Task* task, void* args) {
    BtlEff_Bomb* eff    = task->data;
    s32          var_r5 = 1;

    switch (eff->unk_80) {
        case 0:
            if (eff->unk_7E < eff->unk_7C) {
                eff->unk_7E++;
            } else {
                eff->unk_7E = 0;
                eff->unk_80++;
            }
            break;

        case 1:
            if (eff->unk_7E == 0) {
                func_ov003_020824a0(eff, 0, 1);
                func_ov003_02087f28(501, eff->unk_60.x);
            }
            if (eff->unk_78 == 0) {
                eff->unk_78 = func_ov003_020c5bc4(eff->unk_74, &eff->unk_6C, eff->unk_60.x, eff->unk_60.y, eff->unk_60.z);
            }
            if (SpriteMgr_IsAnimationFinished(&eff->sprite) != 0) {
                var_r5 = 0;
            } else {
                eff->unk_7E++;
            }
    }

    func_ov003_02082b0c(eff);
    return var_r5;
}

/* Task stage 2: render bomb/obstacle. */
s32 BtlEff_Bomb_Render(TaskPool* pool, Task* task, void* args) {
    s16       spA;
    s16       sp8;
    Ov016Obj* temp_r4;

    temp_r4 = task->data;
    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk60, temp_r4->unk64, temp_r4->unk68);
    func_ov003_02082724(temp_r4, spA, sp8);
    if (func_ov003_020c37f8(temp_r4) != 0) {
        func_ov003_02082730(temp_r4, 0x7FDFFFFF);
    } else {
        func_ov003_02082730(temp_r4, 0x7FFFDFFF - temp_r4->unk64);
    }
    func_ov003_02082b64(temp_r4);
    return 1;
}

s32 BtlEff_Bomb_Destroy(TaskPool* pool, Task* task, void* args) {
    func_ov003_02082cc4(task->data);
    return 1;
}

s32 BtlEff_Bomb_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_021290b8 = {
        .initialize = BtlEff_Bomb_Init,
        .update     = BtlEff_Bomb_Update,
        .render     = BtlEff_Bomb_Render,
        .cleanup    = BtlEff_Bomb_Destroy,
    };
    return data_ov016_021290b8.iter[stage](pool, task, args);
}

void BtlEff_Bomb_CreateTaskDefault(s32 arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4) {
    BtlEff_Bomb_CreateTask(arg0, arg1, arg2, arg3, arg4, 0);
}

void BtlEff_Bomb_CreateTask(s32 arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    BtlEff_Bomb_Args args;
    args.unk_00.x = arg2;
    args.unk_00.y = arg3;
    args.unk_00.z = arg4;
    args.unk_0C   = arg1;
    args.unk_10   = arg0;
    args.unk_14   = arg5;

    s32 val = func_ov003_020c3aac(arg0);
    EasyTask_CreateTask(func_ov003_020c3a10(val), &Task_BtlEff_Bomb, NULL, 0, NULL, &args);
}

/// MARK: BtlEff_Fall

typedef struct BtlEff_Fall {
    /* 0x00 */ char                unk_00[0x20];
    /* 0x20 */ s32                 unk_20;
    /* 0x24 */ char                unk_24[0x28 - 0x24];
    /* 0x28 */ s32                 unk_28;
    /* 0x2C */ s32                 unk_2C;
    /* 0x30 */ s32                 unk_30;
    /* 0x34 */ char                unk_34[0x60 - 0x34];
    /* 0x60 */ void*               unk_60;
    /* 0x64 */ struct BtlEff_Fall* unk_64;
    /* 0x68 */ s32                 unk_68;
    /* 0x6C */ Quad                unk_6C;
    /* 0x74 */ u32                 unk_74;
} BtlEff_Fall; // Size: 0x78;

/* Spawn metadata for effects/obstacles (dimensions and timing). */
typedef struct {
    /* 0x00 */ u16  unk0;
    /* 0x02 */ u16  unk2;
    /* 0x04 */ Quad unk_4;
    /* 0x06 */ u16  unk6;
    /* 0x08 */ u16  unk8;
    /* 0x0A */ u16  unkA;
} Ov016SpawnMeta;

typedef struct {
    u32   unk_0;
    u32   unk_4;
    void* unk_8;
    u32   unk_C;
} BtlEff_Fall_Args;

static const TaskHandle Task_BtlEff_Fall = {"Task_BtlEff_Fall", BtlEff_Fall_RunTask, 0x78};

s32 BtlEff_Fall_Init(TaskPool* pool, Task* task, void* args) {
    BtlEff_Fall_Args* spawn_args = args;
    BtlEff_Fall*      temp_r4    = task->data;

    MI_CpuSet(temp_r4, 0, sizeof(BtlEff_Fall));
    temp_r4->unk_60 = spawn_args->unk_0;
    temp_r4->unk_64 = spawn_args->unk_4;
    temp_r4->unk_68 = spawn_args->unk_8;

    Ov016SpawnMeta* temp_r7 = (Ov016SpawnMeta*)spawn_args->unk_8;
    temp_r4->unk_6C         = temp_r7->unk_4;

    temp_r4->unk_74 = spawn_args->unk_C;
    func_ov003_02082a04(0, temp_r4, &data_ov016_0212909c, &data_ov016_021290e4, 0, 7, 0);

    s32 temp_r1 = ((Ov016SpriteParams*)temp_r4->unk_60)->unk2;
    if (temp_r1 <= 14) {
        switch ((s32)temp_r1 - 10) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                break;
            default:
                goto escape;
        }
    }
    if (temp_r1 != 25) {
    escape:
        func_ov003_020c38e8(temp_r4);
    }

    func_ov003_02087f28(500, ((Ov016Obj*)temp_r4->unk_64)->unk28);
    return 1;
}

s32 BtlEff_Fall_Update(TaskPool* pool, Task* task, void* args) {
    BtlEff_Fall* eff    = task->data;
    s32          var_r5 = 1;

    if (eff->unk_64->unk_30 >= 0) {
        var_r5 = 0;
        if (eff->unk_74 != 0x191) {
            if (eff->unk_74 == 0xE6) {
                BtlEff_Bomb_CreateTaskDefault(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28, eff->unk_64->unk_2C,
                                              eff->unk_64->unk_30);
            } else {
                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30, 0);

                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28 + 0x20000, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30 - 0x10000, 4);

                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28 - 0x28000, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30 - 0x30000, 7);

                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28 + 0x18000, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30 - 0x40000, 14);

                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28 - 0x10000, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30 - 0x68000, 14);
            }

            if (eff->unk_60 == NULL) {
                func_ov003_02082f1c(eff->unk_64, 3);
            } else if (((Ov016SpriteParams*)eff->unk_60)->unk2 == 0x19) {
                func_ov003_02082f1c((u8*)eff->unk_60 + 4, 1);
            } else {
                func_ov003_02086654(eff->unk_60);
            }
        }
    }
    func_ov003_02082b0c(eff);
    return var_r5;
}

s32 BtlEff_Fall_Render(TaskPool* pool, Task* task, void* args) {
    BtlEff_Fall* temp_r4 = task->data;
    s16          spA, sp8;

    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk_64->unk_28, temp_r4->unk_64->unk_2C,
                        temp_r4->unk_64->unk_30);
    func_ov003_02082724(temp_r4, spA, sp8);
    func_ov003_02082730(temp_r4, 0x7FFFEFFF - temp_r4->unk_64->unk_2C);
    func_ov003_02082b64(temp_r4);
    return 1;
}

s32 BtlEff_Fall_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlEff_Fall* temp_r4 = task->data;

    if (temp_r4->unk_20 != 0) {
        func_ov003_02082cc4(temp_r4);
    }
    return 1;
}

s32 BtlEff_Fall_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_021290d4 = {
        .initialize = BtlEff_Fall_Init,
        .update     = BtlEff_Fall_Update,
        .render     = BtlEff_Fall_Render,
        .cleanup    = BtlEff_Fall_Destroy,
    };
    return data_ov016_021290d4.iter[stage](pool, task, args);
}

void BtlEff_Fall_CreateTask(UnkStruct_Boss01* arg0, Boss01* arg1, s32 arg2) {
    BtlEff_Fall_Args args;
    args.unk_8 = arg1;
    args.unk_C = arg2;
    args.unk_0 = arg0;
    args.unk_4 = (s32)((u8*)arg0 + 4);
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Task_BtlEff_Fall, 0, 0, 0, &args);
}

/// MARK: BtlObs_Bus

typedef struct BtlObs_Bus {
    /* 0x000 */ char unk_000[0x2];
    /* 0x002 */ s16  unk_002;
    /* 0x004 */ u32  unk_004;
    /* 0x008 */ char unk_008[0x2C - 0x8];
    /* 0x02C */ s32  unk_02C;
    /* 0x030 */ s32  unk_030;
    /* 0x034 */ s32  unk_034;
    /* 0x038 */ char unk_038[0x40 - 0x38];
    /* 0x040 */ s32  unk_040;
    /* 0x044 */ char unk_044[0x4C - 0x44];
    /* 0x04C */ s32  unk_04C;
    /* 0x050 */ char unk_050[0x58 - 0x50];
    /* 0x058 */ u32  unk_058;
    /* 0x05C */ char unk_05C[0x74 - 0x5C];
    /* 0x074 */ s16  unk_074;
    /* 0x076 */ s16  unk_076;
    /* 0x078 */ char unk_078[0xC4 - 0x78];
    /* 0x0C4 */ u32  unk_0C4;
    /* 0x0C8 */ char unk_0C8[0xEC - 0xC8];
    /* 0x0EC */ s32  unk_0EC;
    /* 0x0F0 */ char unk_0F0[0x108 - 0xF0];
    /* 0x108 */ s16  unk_108;
    /* 0x10C */ char unk_10C[0x124 - 0x10A];
    /* 0x124 */ u32  unk_124;
    /* 0x128 */ char unk_128[0x14C - 0x128];
    /* 0x14C */ s32  unk_14C;
    /* 0x150 */ char unk_150[0x184 - 0x150];
    /* 0x184 */ s32  unk_184;
    /* 0x188 */ void (*unk_188)(struct BtlObs_Bus*);
    /* 0x18C */ s16 unk_18C;
    /* 0x18E */ s16 unk_18E;
    /* 0x190 */ u32 unk_190;
    /* 0x194 */ u32 unk_194;
} BtlObs_Bus; // Size: 0x198

typedef struct {
    /* 0x0 */ u32 unk_0;
} BtlObs_Bus_Args;

static const BinIdentifier data_ov016_021290fc = {3, "Apl_Mor/Grp_Boss01bus.bin"};

void BtlObs_Bus_SetState(BtlObs_Bus* arg0, void (*arg1)(BtlObs_Bus*)) {
    arg0->unk_188 = arg1;
    arg0->unk_18C = 0;
    arg0->unk_18E = 0;
}

void BtlObs_Bus_OnPhaseStart(BtlObs_Bus* obs) {
    if ((obs->unk_058 & 0x200) == 0) {
        return;
    }
    if ((*data_ov016_021292c0 == 2) && (obs->unk_14C == obs->unk_0EC)) {
        obs->unk_074 = 60;
        obs->unk_076 = 100;
        func_ov003_020824a0(&obs->unk_0C4, 2, 0);
        func_ov003_020826c8(&obs->unk_0C4, 11);
        func_ov003_02082cc4(&obs->unk_124);
        func_ov003_020880e4(0, &obs->unk_124, &obs->unk_004);
    }
}

void BtlObs_Bus_StateIdle(BtlObs_Bus* obs) {
    return;
}

void BtlObs_Bus_StateStop(BtlObs_Bus* obs) {
    obs->unk_194 = 0;
}

void BtlObs_Bus_StateEnd(BtlObs_Bus* obs) {
    return;
}

void BtlObs_Bus_StateHitFlash(BtlObs_Bus* obs) {
    if (obs->unk_18E == 0) {
        obs->unk_058 |= 0x10;
    }

    if (obs->unk_18E < 20) {
        obs->unk_190 = !obs->unk_190;
    } else if (obs->unk_18E % 3 == 0) {
        obs->unk_190 = 1;
    } else {
        obs->unk_190 = 0;
    }

    if (obs->unk_18E > 20) {
        obs->unk_190 = 0;
        obs->unk_194 = 0;
    } else {
        obs->unk_18E++;
    }
}

s32 BtlObs_Bus_Init(TaskPool* pool, Task* task, void* args) {
    BtlObs_Bus*      obs     = task->data;
    BtlObs_Bus_Args* busArgs = (BtlObs_Bus_Args*)args;

    MI_CpuSet(obs, 0, sizeof(BtlObs_Bus));
    obs->unk_184 = busArgs->unk_0;
    obs->unk_190 = 1;
    obs->unk_194 = 1;
    func_ov003_02083118((u8*)obs + 4, 2);
    obs->unk_002 = 0x19;
    obs->unk_02C = 0x180000;
    obs->unk_030 = 0x9000;
    obs->unk_034 = 0;
    obs->unk_074 = 0x64;
    obs->unk_076 = 0x64;
    obs->unk_040 = 0xCD;
    obs->unk_04C = 0xE66;
    func_ov003_02082a04(0, &obs->unk_0C4, &data_ov016_021290fc, &data_ov016_02129120, 0, 10, 0x180);
    func_ov003_02082a04(0, &obs->unk_124, &data_ov016_021290fc, &data_ov016_02129120, 1, 10, 0);
    func_ov003_020824a0(&obs->unk_0C4, 0, 0);
    func_ov003_020824a0(&obs->unk_124, 1, 0);
    BtlObs_Bus_SetState(obs, BtlObs_Bus_StateIdle);
    return 1;
}

s32 BtlObs_Bus_Update(TaskPool* pool, Task* task, void* args) {
    BtlObs_Bus* obs = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 1;
    }
    switch (func_ov003_02082f2c(&obs->unk_004)) {
        case 1:
            BtlObs_Bus_SetState(obs, BtlObs_Bus_StateHitFlash);
            break;
        case 3:
            BtlObs_Bus_SetState(obs, BtlObs_Bus_StateStop);
            break;
        case 5:
            BtlObs_Bus_SetState(obs, BtlObs_Bus_StateEnd);
            break;
    }
    if (obs->unk_188 != NULL) {
        obs->unk_188(obs);
    }
    func_ov003_02083074(&obs->unk_004);
    BtlObs_Bus_OnPhaseStart(obs);
    func_ov003_02082b0c(&obs->unk_0C4);
    func_ov003_02082b0c(&obs->unk_124);
    return obs->unk_194;
}

// Nonmatching: Invalid type casting
s32 BtlObs_Bus_Render(TaskPool* pool, Task* task, void* args) {
    BtlObs_Bus* obs = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 1;
    }
    if (obs->unk_190 == 0) {
        return 1;
    }

    s16 temp_r4 = func_ov003_020843b0(0, obs->unk_02C);
    u32 temp_r6 = func_ov003_020843ec(0, obs->unk_030, obs->unk_034);
    if (obs->unk_108 == 0) {
        func_ov003_02082724(&obs->unk_124, temp_r4, temp_r6 + 8);
        func_ov003_02082730(&obs->unk_124, 0x7FFFEFFF);
        func_ov003_02082b64(&obs->unk_124);
        func_ov003_02082724(&obs->unk_0C4, temp_r4, temp_r6);
        func_ov003_02082730(&obs->unk_0C4, 0x7FFFEFFF);
        func_ov003_02082b64(&obs->unk_0C4);
    } else {
        func_ov003_02082724(&obs->unk_0C4, temp_r4, temp_r6);
        func_ov003_02082730(&obs->unk_0C4, 0x7FFFFFFF - obs->unk_030);
        func_ov003_02082b64(&obs->unk_0C4);
        func_ov003_0208810c(&obs->unk_124, &obs->unk_004);
    }
    return 1;
}

s32 BtlObs_Bus_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlObs_Bus* obs = task->data;

    func_ov003_02082cc4(&obs->unk_0C4);
    func_ov003_02082cc4(&obs->unk_124);
    return 1;
}

s32 BtlObs_Bus_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02129110 = {
        .initialize = BtlObs_Bus_Init,
        .update     = BtlObs_Bus_Update,
        .render     = BtlObs_Bus_Render,
        .cleanup    = BtlObs_Bus_Destroy,
    };
    return data_ov016_02129110.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_BtlObs_Bus = {"Tsk_BtlObs_Bus", BtlObs_Bus_RunTask, 0x198};

void* BtlObs_Bus_CreateTask(void* bossUG) {
    void* sp8 = bossUG;

    s32 taskId = EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Tsk_BtlObs_Bus, NULL, 0, NULL, &sp8);
    return EasyTask_GetTaskData(&data_ov003_020e71b8->unk_00000, taskId);
}

/// MARK: BtlObs_RG

typedef struct BtlObs_RG {
    /* 0x000 */ char unk_000[0x28];
    /* 0x028 */ s32  unk_028;
    /* 0x02C */ u32  unk_02C;
    /* 0x030 */ u32  unk_030;
    /* 0x034 */ char unk_34[0x38 - 0x34];
    /* 0x038 */ s32  unk_038;
    /* 0x03C */ char unk_03C[0x40 - 0x3C];
    /* 0x040 */ s32  unk_040;
    /* 0x044 */ s32  unk_044;
    /* 0x048 */ char unk_048[0x54 - 0x48];
    /* 0x054 */ s32  unk_054;
    /* 0x058 */ char unk_058[0x84 - 0x58];
    /* 0x084 */ s32  unk_084;
    /* 0x088 */ char unk_088[0x18C - 0x88];
    /* 0x18C */ u16  unk_18C;
    /* 0x18E */ char unk_18E[0x1C0 - 0x18E];
    /* 0x1C0 */ u16  unk_1C0;
    /* 0x1C2 */ u16  unk_1C2;
    /* 0x1C4 */ char unk_1C4[0x284 - 0x1C4];
    /* 0x284 */ Quad unk_284;
    /* 0x28C */ void (*unk_28C)(struct BtlObs_RG*); // function pointer for boss sub-state
    /* 0x290 */ u16 unk_290;
    /* 0x292 */ s16 unk_292;
    /* 0x294 */ u32 unk_294;
} BtlObs_RG; // Size: 0x298

typedef struct {
    u16   unused;
    u16   unk_2;
    u32   unk_4;
    u32   unk_8;
    Quad* unk_C;
} BtlObs_RG_Args;

static s32  BtlObs_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static void BtlObs_RG_StateFall(BtlObs_RG* obs);

static const TaskHandle Tsk_BtlObs_RG = {"Tsk_BtlObs_RG", BtlObs_RG_RunTask, sizeof(BtlObs_RG)};

void BtlObs_RG_SetState(BtlObs_RG* obs, void (*arg1)(BtlObs_RG*)) {
    obs->unk_28C = arg1;
    obs->unk_290 = 0;
    obs->unk_292 = 0;
}

static void BtlObs_RG_StateIdle(BtlObs_RG* arg0) {
    return;
}

static void BtlObs_RG_StateHit(BtlObs_RG* obs) {
    obs->unk_054 |= 6;
    func_ov003_020c4a34(obs);

    if (data_ov003_020e71b8->unk3D8A0 == obs) {
        data_ov003_020e71b8->unk3D8A0 = NULL;
    }

    if (obs->unk_18C & 0x1000) {
        obs->unk_294 = 0;
        return;
    }

    if (func_ov009_020fa71c(&obs->unk_1C0) != 0) {
        BtlEff_Bomb_CreateTaskDefault(233, &obs->unk_284, obs->unk_028, obs->unk_02C, obs->unk_030);
        obs->unk_294 = 0;
    } else {
        BtlObs_RG_SetState(obs, BtlObs_RG_StateFall);
    }
}

static void BtlObs_RG_StateFall(BtlObs_RG* obs) {
    BtlObs_RG* temp_r5 = data_ov003_020e71b8->unk3D89C;

    if (obs->unk_292 == 0) {
        if (obs->unk_1C2 == 0) {
            func_ov003_020824a0(&obs->unk_084, 1, 0);
        }

        obs->unk_040 = (obs->unk_028 >= temp_r5->unk_028) ? 0x5000 : -0x5000;
        obs->unk_044 = 0;
        obs->unk_038 = 0;
    }
    if ((obs->unk_030 == 0) && (obs->unk_038 == 0)) {
        obs->unk_294 = 0;
        return;
    }
    obs->unk_292++;
}

s32 BtlObs_RG_Init(TaskPool* pool, Task* task, void* args) {
    BtlObs_RG_Args* spawnArgs = (BtlObs_RG_Args*)args;
    BtlObs_RG*      obs       = task->data;

    MI_CpuSet(obs, 0, sizeof(BtlObs_RG));

    obs->unk_284 = *spawnArgs->unk_C;

    func_ov003_02086404(1, &obs->unk_084, spawnArgs->unk_2);
    func_ov003_020864dc(obs, spawnArgs->unk_2);
    obs->unk_1C2 = spawnArgs->unk_2;

    if (RNG_Next(2) != 0) {
        func_ov003_02082750(&obs->unk_084, 1);
    }

    func_ov003_020c4988(obs);
    obs->unk_294 = 1;
    BtlObs_RG_SetState(obs, BtlObs_RG_StateIdle);
    return 1;
}

s32 BtlObs_RG_Update(TaskPool* pool, Task* task, void* args) {
    BtlObs_RG* obs = task->data;

    if (data_ov003_020e71b8->unk3D878 & 2) {
        return 1;
    }

    s32 temp = func_ov003_02082f2c(obs);
    if ((temp == 1) || (temp == 3)) {
        BtlObs_RG_SetState(obs, BtlObs_RG_StateHit);
    }

    if (obs->unk_28C != NULL) {
        obs->unk_28C(obs);
    }

    func_ov003_02083074(obs);
    func_ov003_02082b0c(&obs->unk_084);
    return obs->unk_294;
}

s32 BtlObs_RG_Render(TaskPool* pool, Task* task, void* args) {
    BtlObs_RG* obs = task->data;

    if (func_ov003_0208656c() == 0) {
        return 1;
    }
    func_ov003_020831e4(obs, &obs->unk_084);
    return 1;
}

s32 BtlObs_RG_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlObs_RG* obs = task->data;

    func_ov003_02082cc4(&obs->unk_084);
    func_ov003_020c4a34(obs);
    if (data_ov003_020e71b8->unk3D8A0 == obs) {
        data_ov003_020e71b8->unk3D8A0 = NULL;
    }
    return 1;
}

s32 BtlObs_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02129148 = {
        .initialize = BtlObs_RG_Init,
        .update     = BtlObs_RG_Update,
        .render     = BtlObs_RG_Render,
        .cleanup    = BtlObs_RG_Destroy,
    };
    return data_ov016_02129148.iter[stage](pool, task, args);
}

s32 BtlObs_RG_CreateTask(u16 arg0, void* arg1) {
    BtlObs_RG_Args args;
    args.unk_C = arg1;
    args.unk_2 = arg0;
    return EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &Tsk_BtlObs_RG, NULL, 0, NULL, &args);
}
