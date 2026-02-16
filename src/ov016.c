#include "DatMgr.h"
#include "EasyTask.h"
#include "common_data.h"
#include <types.h>

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

/* Resource reference entry used by the animation system. */
typedef struct {
    /* 0x00 */ u32         unk0;
    /* 0x04 */ const char* path;
} Ov016ResRef;

/* Task data wrapper used by the EasyTask pool for this overlay. */
typedef struct {
    /* 0x00 */ u8        pad_18[0x18];
    /* 0x18 */ Ov016Obj* unk18;
} Ov016Task;

/* Actor root header used to find actor-specific data. */
typedef struct {
    /* 0x00 */ u8  pad_0[0x8];
    /* 0x08 */ u8* unk8;
} Ov003ActorRoot;

/* Global overlay state block for ov003 (shared world state). */
typedef struct {
    /* 0x000000 */ u8               pad_3D350[0x3D350];
    /* 0x003D350 */ Ov003ActorRoot* unk3D350;
    /* 0x003D354 */ u8              pad_3D7CC[0x478];
    /* 0x003D7CC */ s32             unk3D7CC;
    /* 0x003D7D0 */ u8              pad_3D824[0x54];
    /* 0x003D824 */ s32             unk3D824;
    /* 0x003D828 */ u8              pad_3D878[0x50];
    /* 0x003D878 */ u32             unk3D878;
    /* 0x003D87C */ u8              pad_3D898[0x1C];
    /* 0x003D898 */ Ov016Obj*       unk3D898;
    /* 0x003D89C */ Ov016Obj*       unk3D89C;
    /* 0x003D8A0 */ Ov016Obj*       unk3D8A0;
    /* 0x003D8A4 */ u8              pad_3D8AC[0x8];
    /* 0x003D8AC */ void*           unk3D8AC;
    /* 0x003D8B0 */ u8              pad_3D8B4[0x4];
    /* 0x003D8B4 */ void*           unk3D8B4;
} Ov003Global;

/* Simple quad of u16 values, often used for dimensions or indices. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
} Ov016U16Quad;

/* Spawn metadata for effects/obstacles (dimensions and timing). */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
} Ov016SpawnMeta;

/* Spawn arguments for tasks that create effects or obstacles. */
typedef struct {
    /* 0x00 */ s32             unk0;
    /* 0x04 */ s32             unk4;
    /* 0x08 */ Ov016SpawnMeta* unk8;
    /* 0x0C */ s32             unkC;
    /* 0x10 */ s32             unk10;
    /* 0x14 */ s32             unk14;
} Ov016SpawnArgs;

/* Basic initializer used by several task stages. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
} Ov016InitArgs4;

/* Boss spawn arguments including animation and geometry data. */
typedef struct {
    /* 0x00 */ u16                 unk0;
    /* 0x02 */ u16                 unk2;
    /* 0x04 */ u32                 pad_04;
    /* 0x08 */ const Ov016U16Quad* unkC;
} Ov016BossSpawnArgs;

/* Simple 3D vector-like container used for positions. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} Ov016Vec3;

#define OV003_TASK_POOL  ((void*)((u8*)data_ov003_020e71b8 + 0x808C))
#define OV003_LIST_3D8AC ((void*)((u8*)data_ov003_020e71b8 + 0x3D8AC))
#define OV003_LIST_3D8B4 ((void*)((u8*)data_ov003_020e71b8 + 0x3D8B4))

typedef s32 (*Ov016StageFunc)(void* arg0, Ov016Task* arg1, void* arg2);

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
    /* 0x1C8 */ void (*unk1C8)(Ov016Obj*);
    /* 0x1CC */ void (*unk1CC)(Ov016Obj*);
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

void                EasyFade_FadeBothDisplays(s32, s32, s32);                                          /* extern */
s32                 EasyFade_IsFading();                                                               /* extern */
s32                 FX_Divide(s32, s32);                                                               /* extern */
s32                 RNG_Next(s32);                                                                     /* extern */
u16                 func_02002bc4(const void*, s32, s32, s32, s32);                                    /* extern */
s32                 func_0200a968(s32, void*, s32, s32);                                               /* extern */
void                func_0200afec(s32, s32);                                                           /* extern */
s32                 func_0200ea4c(void*);                                                              /* extern */
void                func_02026590(void*, s32, u16);                                                    /* extern */
void                func_020265d4(void*, s32, u16);                                                    /* extern */
void                MI_CpuSet(void*, s32, s32);                                                        /* extern */
void                func_ov000_020824a0(void*, u16, s32);                                              /* extern */
void                func_ov003_020826c8(void*, s32);                                                   /* extern */
void                func_ov003_02082724(void*, s16, s16);                                              /* extern */
void                func_ov003_02082730(void*, s32);                                                   /* extern */
void                func_ov003_02082750(void*, s32);                                                   /* extern */
void                func_ov003_02082848(void*, s32);                                                   /* extern */
void                func_ov003_02082940(void*, s32, const void*);                                      /* extern */
void                func_ov003_02082998(void*, void*);                                                 /* extern */
void                func_ov003_02082a04(s32, void*, const void*, const void*, s32, s32, s32);          /* extern */
void                func_ov003_02082a90(s16, void*, const void*, s32, s32, s32, s32, s32);             /* extern */
void                func_ov003_02082b0c(void*);                                                        /* extern */
void                func_ov003_02082b64(void*);                                                        /* extern */
void                func_ov003_02082cc4(void*);                                                        /* extern */
void                func_ov003_02082f1c(void*, s32);                                                   /* extern */
s32                 func_ov003_02082f2c(void*);                                                        /* extern */
void                func_ov003_02083000(s32, void*);                                                   /* extern */
void                func_ov003_02083074(void*);                                                        /* extern */
void                func_ov003_02083118(void*, s32);                                                   /* extern */
void                func_ov003_020831e4(s32, s32);                                                     /* extern */
void                func_ov003_02083bfc(void*, s32, s32);                                              /* extern */
void                func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);                               /* extern */
s16                 func_ov003_020843b0(s32, s32);                                                     /* extern */
s16                 func_ov003_020843ec(s32, s32, s32);                                                /* extern */
void*               func_ov003_0208495c(void*);                                                        /* extern */
void*               func_ov003_02084984(void*);                                                        /* extern */
void                func_ov003_02086404(s32, void*, u16);                                              /* extern */
void                func_ov003_020864dc(void*, u16);                                                   /* extern */
s32                 func_ov003_0208656c();                                                             /* extern */
void                func_ov003_02086654();                                                             /* extern */
void                func_ov003_02087f28(s32, s32);                                                     /* extern */
void                func_ov003_020880e4(s32, void*, void*);                                            /* extern */
void                func_ov003_0208810c(void*, void*);                                                 /* extern */
s32                 func_ov003_02088130();                                                             /* extern */
s32                 func_ov003_020c37f8(void*);                                                        /* extern */
void                func_ov003_020c38e8(void*);                                                        /* extern */
s32                 func_ov003_020c3908(void*);                                                        /* extern */
s32                 func_ov003_020c3960(s32*, s32);                                                    /* extern */
void*               func_ov003_020c3a10();                                                             /* extern */
s32                 func_ov003_020c3aac(s32);                                                          /* extern */
s32                 func_ov003_020c3bf0(void*);                                                        /* extern */
s32                 func_ov003_020c3c28();                                                             /* extern */
void                func_ov003_020c3c44(void*, s32, s32);                                              /* extern */
void*               func_ov003_020c3ca4(void*);                                                        /* extern */
void                func_ov003_020c3cec(void*, void*, void*, void*);                                   /* extern */
void                func_ov003_020c3ec0(void*);                                                        /* extern */
void                func_ov003_020c3efc(void*, void*);                                                 /* extern */
void                func_ov003_020c427c();                                                             /* extern */
void                func_ov003_020c4988(void*);                                                        /* extern */
void                func_ov003_020c49c8(void*);                                                        /* extern */
void                func_ov003_020c4a34(void*);                                                        /* extern */
void                func_ov003_020c4a74(void*);                                                        /* extern */
void                func_ov003_020c4c1c(void*);                                                        /* extern */
void                func_ov003_020c4c5c(void*);                                                        /* extern */
void                func_ov003_020c4cc4(void*, s32);                                                   /* extern */
void                func_ov003_020c4ff4();                                                             /* extern */
void                func_ov003_020c50bc();                                                             /* extern */
void                func_ov003_020c5a00(void*);                                                        /* extern */
s32                 func_ov003_020c5a28(void*, void*);                                                 /* extern */
void                func_ov003_020c5ac0(void*, s32, void*, s32);                                       /* extern */
void                func_ov003_020c5b0c(s32, void*);                                                   /* extern */
s32                 func_ov003_020c5b2c(s32, void*, s32, s32, s32);                                    /* extern */
s32                 func_ov003_020c5bc4(u16, void*, s32, s32, s32);                                    /* extern */
s32                 func_ov003_020c6f40();                                                             /* extern */
s32                 func_ov003_020c9008();                                                             /* extern */
void                func_ov003_020c9064(void*);                                                        /* extern */
s32                 func_ov003_020c9338(void*);                                                        /* extern */
s32                 func_ov003_020cb3c4(s32, s32);                                                     /* extern */
s32                 func_ov003_020cc5fc(s32, s32);                                                     /* extern */
s32                 func_ov009_020fa71c(void*);                                                        /* extern */
s32                 func_ov016_021257a0(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_0212586c(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_021259a0(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_021259f8(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02125a0c(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
void                func_ov016_02125bf8(Ov016Obj* arg0);                                               /* static */
void                func_ov016_02125d28(Ov016Obj* arg0);                                               /* static */
void                func_ov016_021260b4(Ov016Obj* arg0);                                               /* static */
s32                 func_ov016_021260d0(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02126174(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_021261e8(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02126270(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02126290(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
void                func_ov016_021266b4(Ov016Obj* arg0);                                               /* static */
void                func_ov016_021266f4(Ov016Obj* arg0);                                               /* static */
void                func_ov016_0212682c(Ov016Obj* arg0);                                               /* static */
void                func_ov016_02126854(Ov016Obj* arg0);                                               /* static */
void                func_ov016_02126c24(Ov016Obj* arg0);                                               /* static */
void                func_ov016_02126d38(Ov016Obj* arg0);                                               /* static */
void                func_ov016_02126ff0(Ov016Obj* arg0);                                               /* static */
s32                 func_ov016_021270a8(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_021271c0(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127280(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127340(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127374(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
s32                 func_ov016_0212751c(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127604(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127720(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127798(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_021277ac(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
s32                 func_ov016_02127900(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127960(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127bc4(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127c3c(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127c50(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
s32                 func_ov016_02127ce8(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127d74(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127dac(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127e24(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127e38(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
s32                 func_ov016_02127eb8(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127f44(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127f7c(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02127ff4(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128008(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
s32                 func_ov016_02128088(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_0212812c(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_0212820c(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_021282a0(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_021282b4(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
void                func_ov016_021282fc(s32 arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4);           /* static */
void                func_ov016_02128320(s32 arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5); /* static */
s32                 func_ov016_02128374(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_0212845c(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_021285e0(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128658(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128678(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
void                func_ov016_021286c0(Ov016Obj* arg0, Ov016Obj* arg1, s32 arg2);                     /* static */
void                func_ov016_021287a4(Ov016Obj* arg0);                                               /* static */
void                func_ov016_021287a8(Ov016Obj* arg0);                                               /* static */
void                func_ov016_021287b4(Ov016Obj* arg0);                                               /* static */
void                func_ov016_021287b8(Ov016Obj* arg0);                                               /* static */
s32                 func_ov016_02128850(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128960(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128a20(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128b28(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128b48(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
void*               func_ov016_02128b90(Ov016Obj* arg0);                                               /* static */
void                func_ov016_02128bf8(Ov016Obj* arg0);                                               /* static */
void                func_ov016_02128bfc(Ov016Obj* arg0);                                               /* static */
void                func_ov016_02128ca8(Ov016Obj* arg0);                                               /* static */
s32                 func_ov016_02128d48(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128df8(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128e70(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128e9c(void* arg0, Ov016Task* arg1, void* arg2);                      /* static */
s32                 func_ov016_02128edc(TaskPool* pool, Task* task, void* args, s32 stage);            /* static */
s32                 func_ov016_02128f24(u16 arg0, void* arg1);                                         /* static */
void                func_ov016_021277f4(void);                                                         /* static */
void                func_ov016_02127c98(void);                                                         /* static */
void                func_ov016_02127e80(void);                                                         /* static */
void                func_ov016_02128050(void);                                                         /* static */
extern Ov003Global* data_ov003_020e71b8;
extern void*        data_ov009_021234cc;
extern void*        data_ov009_02123540;

/* Resource paths and task names for boss and battle effects. */
// char data_ov016_021291b8[] = "Tsk_Boss01_Eff";
// char data_ov016_021291c8[] = "Tsk_Boss01_RG";
// char data_ov016_021291dc[] = "Tsk_Boss01_UG";
/*char data_ov016_021291ec[] = "Tsk_Boss01s_FlyRG_Curve";
char data_ov016_02129204[] = "Tsk_Boss01s_FlyUG";
char data_ov016_02129218[] = "Tsk_Boss01s_LandRG";
char data_ov016_0212922c[] = "Tsk_Boss01s_LandUG";
char data_ov016_0212925c[] = "Task_BtlEff_Bomb";
char data_ov016_02129270[] = "Task_BtlEff_Fall";
char data_ov016_02129284[] = "Tsk_BtlObs_Bus";
char data_ov016_021292b0[] = "Tsk_BtlObs_RG";*/

/* Resource table entries used by animations/sprites. */
static const Ov016ResRef data_ov016_02128f88 = {3, "Apl_Mor/Grp_Boss01b.bin"};
static const Ov016ResRef data_ov016_02128f90 = {3, "Apl_Mor/Grp_Boss01c.bin"};
static const Ov016ResRef data_ov016_02128f98 = {3, "Apl_Mor/Grp_Boss01s.bin"};
static const Ov016ResRef data_ov016_0212909c = {3, "Apl_Mor/Grp_BtlEff_Obs.bin"};
static const Ov016ResRef data_ov016_021290fc = {3, "Apl_Mor/Grp_Boss01bus.bin"};

/* Default animation config for a boss sprite state. */
static const Ov016AnimConfig data_ov016_02128f70 = {
    {1, 3, 2, 0},
    &data_ov016_02128f88, (const Ov016SpriteParams*)&data_ov016_02128f70, 0, 4, 0xD0, 0
};

/* Task descriptors for boss, effects, and obstacle spawns. */
static const TaskHandle data_ov016_02128fa0 = {"Tsk_Boss01_Eff", func_ov016_02125a0c, 0x80};
static const TaskHandle data_ov016_02128fbc = {"Tsk_Boss01_RG", func_ov016_02126290, 0x1E0};
static const TaskHandle data_ov016_02128fd8 = {"Tsk_Boss01_UG", func_ov016_02127374, 0x1F4};
static const TaskHandle data_ov016_02128ff4 = {"Tsk_Boss01s_FlyRG_Curve", func_ov016_021277ac, 0x8C};
static const TaskHandle data_ov016_02129020 = {"Tsk_Boss01s_FlyUG", func_ov016_02127c50, 0x80};
static const TaskHandle data_ov016_0212905c = {"Tsk_Boss01s_LandRG", func_ov016_02127e38, 0x70};
static const TaskHandle data_ov016_02129080 = {"Tsk_Boss01s_LandUG", func_ov016_02128008, 0x78};
static const TaskHandle data_ov016_021290ac = {"Task_BtlEff_Bomb", func_ov016_021282b4, 0x84};
static const TaskHandle data_ov016_021290c8 = {"Task_BtlEff_Fall", func_ov016_02128678, 0x78};
static const TaskHandle data_ov016_02129104 = {"Tsk_BtlObs_Bus", func_ov016_02128b48, 0x198};
static const TaskHandle data_ov016_0212913c = {"Tsk_BtlObs_RG", func_ov016_02128edc, 0x298};

/* Stage function tables for task lifecycles. */
static const Ov016StageFunc data_ov016_02128fac[4] = {func_ov016_021257a0, func_ov016_0212586c, func_ov016_021259a0,
                                                      func_ov016_021259f8};

static const Ov016StageFunc data_ov016_02128fc8[4] = {func_ov016_021260d0, func_ov016_02126174, func_ov016_021261e8,
                                                      func_ov016_02126270};

static const Ov016StageFunc data_ov016_02128fe4[4] = {func_ov016_021270a8, func_ov016_021271c0, func_ov016_02127280,
                                                      func_ov016_02127340};

static const Ov016StageFunc data_ov016_02129000[4] = {func_ov016_0212751c, func_ov016_02127604, func_ov016_02127720,
                                                      func_ov016_02127798};

static const Ov016StageFunc data_ov016_0212902c[4] = {func_ov016_02127900, func_ov016_02127960, func_ov016_02127bc4,
                                                      func_ov016_02127c3c};

static const Ov016StageFunc data_ov016_02129068[4] = {func_ov016_02127ce8, func_ov016_02127d74, func_ov016_02127dac,
                                                      func_ov016_02127e24};

static const Ov016StageFunc data_ov016_0212908c[4] = {func_ov016_02127eb8, func_ov016_02127f44, func_ov016_02127f7c,
                                                      func_ov016_02127ff4};

static const Ov016StageFunc data_ov016_021290b8[4] = {func_ov016_02128088, func_ov016_0212812c, func_ov016_0212820c,
                                                      func_ov016_021282a0};

static const Ov016StageFunc data_ov016_021290d4[4] = {func_ov016_02128374, func_ov016_0212845c, func_ov016_021285e0,
                                                      func_ov016_02128658};

static const Ov016StageFunc data_ov016_02129110[4] = {func_ov016_02128850, func_ov016_02128960, func_ov016_02128a20,
                                                      func_ov016_02128b28};

static const Ov016StageFunc data_ov016_02129148[4] = {func_ov016_02128d48, func_ov016_02128df8, func_ov016_02128e70,
                                                      func_ov016_02128e9c};

/* Frame/animation parameter tables. */
static const u16 data_ov016_02129010[] = {1, 3, 2, 0, 1, 3, 2, 3};

static const u16 data_ov016_0212903c[] = {1, 3, 2, 4, 1, 3, 2, 5, 1, 3, 2, 6};

static const u16 data_ov016_02129054[] = {1, 3, 2, 1};
static const u16 data_ov016_02129078[] = {1, 3, 2, 2};
static const u16 data_ov016_021290a4[] = {4, 6, 5, 0};

static const u16 data_ov016_021290e4[] = {1, 3, 2, 0, 1, 3, 2, 1, 1, 3, 2, 2};

static const u16 data_ov016_02129120[] = {1, 3, 2, 0, 4, 6, 5, 0, 7, 9, 8, 0, 0x5000, 0};

/* Boss resource selection by index. */
static const Ov016ResRef* data_ov016_021291ac[3] = {&data_ov016_02128f88, &data_ov016_02128f88, &data_ov016_02128f90};

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

/* Task stage 0: initialize a small boss effect object. */
s32 func_ov016_021257a0(void* arg0, Ov016Task* arg1, void* arg2) {
    s16             temp_r5_2;
    Ov016Obj*       temp_r4;
    Ov016InitArgs4* args;
    s32*            temp_r5;

    (void)arg0;
    temp_r4 = arg1->unk18;
    args    = (Ov016InitArgs4*)arg2;
    MI_CpuSet(temp_r4, 0, 0x80);
    temp_r5        = (s32*)((u8*)temp_r4 + 0x60);
    temp_r4->unk60 = (s32)args->unk0;
    temp_r5[1]     = (s32)args->unk4;
    temp_r5[2]     = (s32)args->unk8;
    temp_r5[3]     = (s32)args->unkC;
    temp_r5_2      = (s16)temp_r4->unk6C;
    func_ov003_02082a90(temp_r5_2, temp_r4, data_ov016_021291ac[temp_r5_2], 1, 3, 2, 4, (s32)(u16)(RNG_Next(5) + 1));
    func_ov003_02082750(temp_r4, RNG_Next(2));
    temp_r4->unk7C = (s32)(RNG_Next(1) + 0x1666);
    temp_r4->unk78 = (s32)(0 - (RNG_Next(0x219B) + 0x666));
    return 1;
}

/* Task stage 1: update movement and state transitions for the effect. */
s32 func_ov016_0212586c(void* arg0, Ov016Task* arg1, void* arg2) {
    s32       temp_r0;
    s32       temp_r0_2;
    s32       temp_r0_3;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    temp_r0 = temp_r4->unk74;
    switch (temp_r0) { /* irregular */
        case 0:
            temp_r4->unk60 = (s32)(temp_r4->unk60 + temp_r4->unk7C);
            temp_r4->unk68 = (s32)(temp_r4->unk68 + temp_r4->unk78);
            temp_r0_2      = temp_r4->unk78 + 0x11E;
            temp_r4->unk78 = temp_r0_2;
            if (temp_r0_2 > 0x2E1) {
                temp_r4->unk70 = 0;
                temp_r4->unk74 = (s32)(temp_r4->unk74 + 1);
            } else {
                temp_r4->unk70 = (s32)(temp_r4->unk70 + 1);
            }
        default:
            func_ov003_02082b0c(temp_r4);
            return 1;
        case 1:
            temp_r4->unk60 = (s32)(temp_r4->unk60 + temp_r4->unk7C);
            temp_r4->unk68 = (s32)(temp_r4->unk68 + temp_r4->unk78);
            temp_r0_3      = temp_r4->unk78 - 0xCD;
            temp_r4->unk78 = temp_r0_3;
            if (temp_r0_3 < 0) {
                temp_r4->unk78 = (s32)(RNG_Next(0xB35) + 0xCCC);
                if ((s32)temp_r4->unk7C > 0) {
                    temp_r4->unk7C = (s32)(0 - (RNG_Next(0x1001) + 0x800));
                } else {
                    temp_r4->unk7C = (s32)(RNG_Next(0x1001) + 0x800);
                }
            }
            if ((s32)temp_r4->unk68 >= 0) {
                return 0;
            }
            /* Duplicate return node #14. Try simplifying control flow for better match */
            func_ov003_02082b0c(temp_r4);
            return 1;
    }
}

/* Task stage 2: apply camera transform and render update. */
s32 func_ov016_021259a0(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       spA;
    s16       sp8;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk60, temp_r4->unk64, temp_r4->unk68);
    func_ov003_02082724(temp_r4, spA, sp8);
    func_ov003_02082b64(temp_r4);
    return 1;
}

/* Task stage 3: finalize and free resources. */
s32 func_ov016_021259f8(void* arg0, Ov016Task* arg1, void* arg2) {
    (void)arg0;
    (void)arg2;
    func_ov003_02082cc4(arg1->unk18);
    return 1;
}

/* Task dispatcher for the boss effect handle. */
s32 func_ov016_02125a0c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_02128fac[stage](pool, task_data, args);
}

/* Periodically spawn sub-effects around the boss. */
void func_ov016_02125a54(Ov016Obj* arg0) {
    s16   sp16;
    s16   sp14;
    s32   sp10;
    s32   spC;
    void* sp8;
    u32   temp_r4;

    if ((s32)arg0->unk1A4 > (s32)arg0->unk1A6) {
        temp_r4 = (u32)(arg0->unk84 << 0x1E) >> 0x1E;
        if (arg0->unk24 == 0) {
            sp8 = arg0->unk28 + ((RNG_Next(0x19) + 0x18) << 0xC);
        } else {
            sp8 = arg0->unk28 - ((RNG_Next(0x19) + 0x18) << 0xC);
        }
        spC  = arg0->unk2C;
        sp10 = arg0->unk30 + ((RNG_Next(0x81) - 0x40) << 0xC);
        sp14 = (s16)temp_r4;
        sp16 = arg0->unkCA & 1;
        EasyTask_CreateTask(func_ov003_020c3ca4(arg0), &data_ov016_02128fa0, 0, 0, 0, &sp8);
        arg0->unk1A4 = 0;
        arg0->unk1A6 = (s16)(RNG_Next(0xB) + 0xA);
    }
    arg0->unk1A4 = (s16)(arg0->unk1A4 + 1);
}

/* Check if object has moved outside horizontal bounds. */
s32 func_ov016_02125b48(Ov016Obj* arg0) {
    s32 temp_lr;
    s32 var_ip;
    s32 var_ip_2;
    s32 var_r3;

    temp_lr = arg0->unk24;
    var_r3  = 1;
    var_ip  = 0;
    if ((temp_lr == 0) && ((s32)arg0->unk28 < -0x80000)) {
        var_ip = 1;
    }
    if (var_ip == 0) {
        var_ip_2 = 0;
        if ((temp_lr != 0) && ((s32)arg0->unk28 > (s32)(data_ov003_020e71b8->unk3D824 + 0x80000))) {
            var_ip_2 = 1;
        }
        if (var_ip_2 == 0) {
            var_r3 = 0;
        }
    }
    return var_r3;
}

/* Flip horizontal velocity based on facing side. */
void func_ov016_02125bb8(Ov016Obj* arg0) {
    s32 var_r1;

    var_r1 = 0x4000;
    if (arg0->unk24 == 0) {
        var_r1 = -0x4000;
    }
    arg0->unk1D4 = var_r1;
}

/* Switch to a new boss behavior callback. */
void func_ov016_02125bd0(Ov016Obj* arg0, void (*arg1)(Ov016Obj*)) {
    func_ov003_020c427c();
    arg0->unk1CC = arg1;
    arg0->unk1C8 = 0;
    arg0->unk1C0 = 0;
}

/* Wait for global state before advancing behavior. */
void func_ov016_02125bf8(Ov016Obj* arg0) {
    if (*data_ov016_021292c0 != 5) {
        return;
    }
    func_ov016_02125bd0(arg0, func_ov016_02125d28);
}

/* Toggle collision/interaction flags across active objects. */
void func_ov016_02125c20(Ov016Obj* arg0, s32 arg1) {
    s32       temp_r0;
    s32       temp_r0_2;
    s32       temp_r0_3;
    s32       temp_r0_4;
    Ov016Obj* var_r4;

    temp_r0 = arg0->unk1D8;
    if ((temp_r0 != 0) && (arg1 != 0)) {
        return;
    }
    if ((temp_r0 == 0) && (arg1 == 0)) {
        return;
    }
    arg0->unk1D8 = arg1;
    var_r4       = func_ov003_0208495c(OV003_LIST_3D8AC);
    if (var_r4 == NULL) {
        return;
    }
    do {
        temp_r0_2 = var_r4->unk7C;
        switch (temp_r0_2) { /* irregular */
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
                    data_ov003_020e71b8->unk3D8A0 = EasyTask_GetTaskData(OV003_TASK_POOL, arg0->unk1DC);
                    var_r4->unk54                 = (s32)(var_r4->unk54 & ~0x10 & ~0x20);
                }
                break;
        }
        var_r4 = func_ov003_02084984((u8*)var_r4 + 0x178);
    } while (var_r4 != NULL);
}

/* Boss movement routine that tracks a target and spawns effects. */
void func_ov016_02125d28(Ov016Obj* arg0) {
    f32       var_r0_2;
    f32       var_r0_3;
    f32       var_r0_5;
    s16       temp_r0_3;
    s16       temp_r8;
    s32       temp_r0;
    s32       temp_r0_4;
    s32       temp_r1;
    s32       temp_r1_2;
    s32       temp_r2;
    s32       temp_r9;
    s32       var_r0;
    s32       var_r0_4;
    s32       var_r0_6;
    s32       var_r0_7;
    s32       var_r1;
    s32       var_r1_2;
    Ov016Obj* temp_r0_2;
    Ov016Obj* temp_r0_5;
    Ov016Obj* temp_r4;
    Ov016Obj* temp_r5;
    Ov016Obj* temp_r6;

    temp_r6 = arg0->unk184;
    temp_r4 = data_ov003_020e71b8->unk3D89C;
    temp_r5 = (Ov016Obj*)temp_r6->unk188;
    if (arg0->unk1C0 == 0) {
        if ((s32)temp_r6->unk28 < 0) {
            arg0->unk28 = -0x80000;
            var_r0      = 0x4000;
        } else {
            arg0->unk28 = (s32)(data_ov003_020e71b8->unk3D824 + 0x80000);
            var_r0      = -0x4000;
        }
        arg0->unk1D4 = var_r0;
        arg0->unk2C  = (s32)temp_r4->unk2C;
        arg0->unk30  = -0x40000;
        func_ov003_020c4c1c(arg0);
        func_ov003_020c4988(arg0);
        if (temp_r5 != NULL) {
            arg0->unk1DC = func_ov016_02128f24(temp_r5->unk2, (u8*)data_ov003_020e71b8->unk3D89C + 4);
            temp_r8      = temp_r5->unk76;
            temp_r9      = temp_r8 << 0xC;
            if ((s32)temp_r8 > 0) {
                var_r0_2 = 0.5f + (f32)temp_r9;
            } else {
                var_r0_2 = (f32)temp_r9 - 0.5f;
            }
            if ((s32)var_r0_2 > 0x80000) {
                if ((s32)temp_r8 > 0) {
                    var_r0_3 = 0.5f + (f32)temp_r9;
                } else {
                    var_r0_3 = (f32)temp_r9 - 0.5f;
                }
                arg0->unk30 = (s32)((0 - (s32)var_r0_3) - 0x40000);
            }
            temp_r2 = temp_r5->unk76 * (temp_r5->unk74 << 0xD);
            temp_r1 = (s32)(temp_r2 + ((u32)(temp_r2 >> 5) >> 0x1A)) >> 6;
            var_r1  = (s32)(temp_r1 + ((u32)(temp_r1 >> 4) >> 0x1B)) >> 5;
            if (var_r1 > 0x2000) {
                var_r1 = 0x2000;
            }
            temp_r0 = arg0->unk1D4;
            if (arg0->unk24 == 0) {
                arg0->unk1D4 = (s32)(temp_r0 + var_r1);
            } else {
                arg0->unk1D4 = (s32)(temp_r0 - var_r1);
            }
            goto block_18;
        }
        goto block_18;
    }
block_18:
    temp_r1_2   = arg0->unk28 + arg0->unk1D4;
    arg0->unk28 = temp_r1_2;
    var_r0_4    = temp_r4->unk28 - temp_r1_2;
    if (var_r0_4 < 0) {
        var_r0_4 = 0 - var_r0_4;
    }
    if (var_r0_4 < 0x80000) {
        func_ov016_02125c20(arg0, 1);
    } else {
        func_ov016_02125c20(arg0, 0);
    }
    if (EasyTask_ValidateTaskId(OV003_TASK_POOL, &arg0->unk1DC) != 0) {
        temp_r0_2 = EasyTask_GetTaskData(OV003_TASK_POOL, arg0->unk1DC);
        if (!(temp_r0_2->unk54 & 2)) {
            temp_r0_2->unk28 = (s32)arg0->unk28;
            temp_r0_2->unk2C = (s32)arg0->unk2C;
            temp_r0_3        = temp_r0_2->unk72;
            temp_r0_4        = temp_r0_3 << 0xC;
            if ((s32)temp_r0_3 > 0) {
                var_r0_5 = 0.5f + (f32)temp_r0_4;
            } else {
                var_r0_5 = (f32)temp_r0_4 - 0.5f;
            }
            temp_r0_2->unk30 = (s32)(arg0->unk30 + (s32)var_r0_5);
        } else {
            func_ov016_02125bb8(arg0);
        }
    } else {
        func_ov016_02125bb8(arg0);
    }
    var_r1_2 = arg0->unk1D4;
    var_r0_6 = temp_r4->unk28 - arg0->unk28;
    if (var_r0_6 < 0) {
        var_r0_6 = 0 - var_r0_6;
    }
    if (var_r1_2 < 0) {
        var_r1_2 = 0 - var_r1_2;
    }
    if (var_r0_6 < var_r1_2) {
        func_ov003_020c5b0c(0xE8, arg0);
    }
    func_ov016_02125a54(arg0);
    if (func_ov016_02125b48(arg0) != 0) {
        func_ov003_020c4a34(arg0);
        if (EasyTask_ValidateTaskId(OV003_TASK_POOL, &arg0->unk1DC) != 0) {
            temp_r0_5 = EasyTask_GetTaskData(OV003_TASK_POOL, arg0->unk1DC);
            if (temp_r0_5->unk54 & 2) {
                temp_r6->unk188 = NULL;
            } else {
                temp_r0_5->unk18C = (u16)(temp_r0_5->unk18C | 0x1000);
                func_ov003_020c4ff4();
            }
        } else {
            temp_r6->unk188 = NULL;
        }
        if ((s32)arg0->unk28 < 0) {
            var_r0_7 = -0x80000;
        } else {
            var_r0_7 = data_ov003_020e71b8->unk3D7CC + 0x80000;
        }
        temp_r6->unk28       = var_r0_7;
        *data_ov016_021292c0 = 2;
        func_ov016_02125bd0(arg0, func_ov016_02125bf8);
        return;
    }
    arg0->unk1C0 = (s16)(arg0->unk1C0 + 1);
}

/* Clear an internal action flag when a condition resolves. */
void func_ov016_021260b4(Ov016Obj* arg0) {
    if (func_ov003_020c6f40() == 0) {
        arg0->unk1D0 = 0;
    }
}

/* Task stage 0: initialize boss main object. */
s32 func_ov016_021260d0(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj*   temp_r4;
    Ov016Sprite subroutine;

    (void)arg0;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x1E0);
    func_ov003_020c3efc(temp_r4, arg2);
    func_ov016_021256c0(&subroutine, 1, 0);
    func_ov003_02082998((u8*)temp_r4 + 0x84, &subroutine);
    func_ov003_020c4c1c(temp_r4);
    temp_r4->unk54  = (s32)(temp_r4->unk54 | 0x10000000);
    temp_r4->unk3C  = 0;
    temp_r4->unk28  = -0x80000;
    temp_r4->unk2C  = 0;
    temp_r4->unk30  = -0x40000;
    temp_r4->unk1DC = -1;
    temp_r4->unk1D0 = 1;
    func_ov016_02125bd0(temp_r4, func_ov016_02125bf8);
    return 1;
}

/* Task stage 1: run boss logic and behavior callback. */
s32 func_ov016_02126174(void* arg0, Ov016Task* arg1, void* arg2) {
    void (*temp_r1)(Ov016Obj*);
    s32       temp_r0;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c3bf0(temp_r4) != 0) {
        return 1;
    }
    temp_r0 = func_ov003_02082f2c(temp_r4);
    if ((temp_r0 != 1) && (temp_r0 == 3)) {
        func_ov016_02125bd0(temp_r4, func_ov016_021260b4);
    }
    temp_r1 = temp_r4->unk1CC;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov003_02083000(1, temp_r4);
    func_ov003_02082b0c((u8*)temp_r4 + 0x84);
    return temp_r4->unk1D0;
}

/* Task stage 2: update sprite draw state with offsets. */
s32 func_ov016_021261e8(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       spA;
    s16       sp8;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (!(temp_r4->unk18C & 1)) {
        func_ov003_02084348(1, &spA, &sp8, temp_r4->unk28, temp_r4->unk2C, temp_r4->unk30);
        func_ov003_02082724((u8*)temp_r4 + 0x84, spA, sp8);
        func_ov003_02082730((u8*)temp_r4 + 0x84, 0x7FF3FFFF);
        func_ov003_02082b64((u8*)temp_r4 + 0x84);
        temp_r4->unk34 = spA;
        temp_r4->unk36 = (s16)(sp8 + 0x50);
    }
    return 1;
}

/* Task stage 3: cleanup boss sprite and state. */
s32 func_ov016_02126270(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    func_ov003_02082cc4((u8*)temp_r4 + 0x84);
    func_ov003_020c4a34(temp_r4);
    return 1;
}

/* Task dispatcher for the boss main object. */
s32 func_ov016_02126290(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_02128fc8[stage](pool, task_data, args);
}

/* Render a shadow/secondary sprite using actor header data. */
void func_ov016_021262d8(Ov016Obj* arg0) {
    s16             temp_r7;
    s32             temp_r2;
    s32             temp_r4;
    s32             temp_r6;
    u16             temp_r1_2;
    u16             var_r0;
    u32             temp_r0_2;
    u32             temp_r0_3;
    Ov003ActorRoot* temp_r0;
    void*           temp_r5;
    void*           var_r8;

    if (func_ov003_02088130() == 0) {
        return;
    }
    temp_r0 = data_ov003_020e71b8->unk3D350;

    var_r8 = Data_GetPackEntryData((Data*)temp_r0, 66);

    temp_r4 = arg0->unk1D8;
    temp_r2 = arg0->unk1DC;
    if (arg0->unkCA & 1) {
        var_r0 = func_02002bc4(&data_02069804, 0, temp_r2, temp_r2, 1);
    } else {
        var_r0 = func_02002bc4(&data_02069804, 0, temp_r2, temp_r2, 0);
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

/* Check if boss has reached screen edge for this state. */
s32 func_ov016_02126468(Ov016Obj* arg0) {
    s32 temp_lr;
    s32 var_ip;
    s32 var_ip_2;
    s32 var_r3;

    temp_lr = arg0->unk24;
    var_r3  = 1;
    var_ip  = 0;
    if ((temp_lr == 0) && ((s32)arg0->unk28 < -0x80000)) {
        var_ip = 1;
    }
    if (var_ip == 0) {
        var_ip_2 = 0;
        if ((temp_lr != 0) && ((s32)arg0->unk28 > (s32)(data_ov003_020e71b8->unk3D7CC + 0x80000))) {
            var_ip_2 = 1;
        }
        if (var_ip_2 == 0) {
            var_r3 = 0;
        }
    }
    return var_r3;
}

/* Spawn an obstacle effect sequence. */
void func_ov016_021264d8(void) {
    u8 subroutine[0x14];

    func_ov003_020c3c44(subroutine, 7, 3);
    func_ov003_020c3cec(&data_ov009_02123540, &data_ov009_021234cc, subroutine, subroutine);
}

/* Validate potential target based on collision and status flags. */
s32 func_ov016_02126514(Ov016Obj* arg0, Ov016Obj* arg1) {
    if ((func_ov003_020c5a28(arg0, (u8*)arg1 + 4) != 0) && (arg1->unk44 == 0) && (arg1->unk48 == 0) && (arg1->unk34 == 0)) {
        return 1;
    }
    return 0;
}

/* Validate target based on grounded/airborne state. */
s32 func_ov016_02126554(Ov016Obj* arg0, Ov016Obj* arg1, s32 arg2) {
    if ((arg2 != 0) && (func_ov009_020fa71c(arg1) == 0)) {
        return 0;
    }
    if ((arg2 == 0) && (func_ov009_020fa71c(arg1) != 0)) {
        return 0;
    }
    return func_ov016_02126514(arg0, arg1);
}

/* Pick a random eligible target from the active list. */
Ov016Obj* func_ov016_021265a8(Ov016Obj* arg0, s32 arg1) {
    s32       temp_r5;
    s32       var_r9;
    Ov016Obj* var_r4;
    Ov016Obj* var_r8;

    var_r4  = NULL;
    temp_r5 = RNG_Next(func_ov003_020cc5fc(0, 0x19));
    var_r8  = func_ov003_0208495c(OV003_LIST_3D8B4);
    var_r9  = 0;
    if (var_r8 != NULL) {
    loop_1:
        if (func_ov016_02126554(arg0, var_r8, arg1) != 0) {
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
Ov016Obj* func_ov016_02126638(Ov016Obj* arg0) {
    s16       temp_r1_2;
    Ov016Obj* temp_r1;
    Ov016Obj* temp_r4;
    Ov016Obj* var_r0;

    temp_r4   = func_ov016_021265a8(arg0, 0);
    var_r0    = func_ov016_021265a8(arg0, 1);
    temp_r1_2 = arg0->unk6E;
    if ((s32)arg0->unk6C <= (s32)((s32)(temp_r1_2 + ((u32)temp_r1_2 >> 0x1F)) >> 1)) {
        temp_r1 = arg0->unk1E4;
        if (temp_r1 != NULL) {
            return temp_r1;
        }
    }
    if (var_r0 == NULL) {
        var_r0 = temp_r4;
    }
    return var_r0;
}

/* Switch boss AI state callback and reset timers. */
void func_ov016_02126688(Ov016Obj* arg0, void (*arg1)(Ov016Obj*)) {
    func_ov003_020c427c();
    arg0->unk1C8 = arg1;
    arg0->unk1C4 = 0;
    arg0->unk1C0 = 0;
    arg0->unk1EC = 0;
}

/* Gate transition into battle intro sequence. */
void func_ov016_021266b4(Ov016Obj* arg0) {
    if (func_ov003_020cb3c4(0, 7) != 0) {
        return;
    }
    *data_ov016_021292c0 = 1;
    func_ov016_02126688(arg0, func_ov016_021266f4);
}

/* Boss intro state machine including fades and spawns. */
void func_ov016_021266f4(Ov016Obj* arg0) {
    s16 temp_r0;
    s16 temp_r2;
    s32 var_r5;

    temp_r2 = arg0->unk1C4;
    switch (temp_r2) {
        default:
            return;
        case 0:
            temp_r0 = arg0->unk1C0;
            if ((s32)temp_r0 <= 0xB4) {
                arg0->unk1C0 = (s16)(temp_r0 + 1);
                return;
            }
            arg0->unk1C0 = 0;
            arg0->unk1C4 = (s16)(arg0->unk1C4 + 1);
            return;
        case 1:
            arg0->unk1C4 = (s16)(temp_r2 + 1);
            return;
        case 2:
            func_ov016_02126688(arg0, func_ov016_02126c24);
            return;
        case 3:
            EasyFade_FadeBothDisplays(0, -0x10, 0x400);
            if (EasyFade_IsFading() != 0) {
                return;
            }
            arg0->unk1C4 = (s16)(arg0->unk1C4 + 1);
            return;
        case 4:
            if (arg0->unk1C0 == 0) {
                EasyFade_FadeBothDisplays(0, 0, 0x400);
                func_ov003_020c3ec0(arg0);
                func_ov016_02127e80();
                func_ov016_02128050();
                func_ov016_021277f4();
                func_ov016_02127c98();
                var_r5 = 0;
                do {
                    func_ov016_021264d8();
                    var_r5 += 1;
                } while (var_r5 < 4);
            }
            if (EasyFade_IsFading() == 0) {
                *data_ov016_021292c0 = 2;
                func_ov016_02126688(arg0, func_ov016_02126854);
                return;
            }
            arg0->unk1C0 = (s16)(arg0->unk1C0 + 1);
            return;
    }
}

/* Wait until global state allows next phase. */
void func_ov016_0212682c(Ov016Obj* arg0) {
    if (*data_ov016_021292c0 != 2) {
        return;
    }
    func_ov016_02126688(arg0, func_ov016_02126854);
}

/* Boss roaming/attack decision logic. */
void func_ov016_02126854(Ov016Obj* arg0) {
    s32       sp8;
    s32       var_r3;
    s16       temp_r1_2;
    s32       temp_r3;
    s32       temp_r3_2;
    s32       var_r0;
    u32       temp_r1;
    Ov016Obj* temp_r0;
    Ov016Obj* temp_r0_2;
    Ov016Obj* temp_r0_3;
    Ov016Obj* temp_r4;

    temp_r4 = data_ov003_020e71b8->unk3D898;
    if (arg0->unk1C0 == 0) {
        func_ov003_020c4c5c(arg0);
        arg0->unk54  = (s32)(arg0->unk54 | 0x10);
        arg0->unk1DC = 0x666;
        arg0->unk2C  = (s32)temp_r4->unk2C;
        arg0->unk30  = -0x200000;
        var_r0       = 0x3000;
        if (arg0->unk24 == 0) {
            var_r0 = -0x3000;
        }
        arg0->unk1D0 = var_r0;
        if ((arg0->unk188 == 0) && (arg0->unk1E0 != NULL)) {
            func_ov003_02086654();
            arg0->unk1E8 = 0;
            arg0->unk1E0 = NULL;
        }
        if (arg0->unk1E8 != 0) {
            arg0->unk1EC = (RNG_Next(2) == 0) ? 1 : 0;
            if (arg0->unk1EC != 0) {
                temp_r3      = FX_Divide(0x2800, arg0->unk1E0->unk4C) >> 0xC;
                temp_r1      = temp_r3 * (arg0->unk1E0->unk4C * temp_r3);
                arg0->unk1F0 = (s32)((temp_r3 * 0x2800) - ((s32)(temp_r1 + (temp_r1 >> 0x1F)) >> 1));
            }
            goto block_12;
        }
        goto block_12;
    }
block_12:
    arg0->unk28 = (s32)(arg0->unk28 + arg0->unk1D0);
    if (arg0->unk1E8 != 0) {
        sp8       = arg0->unk28;
        temp_r1_2 = arg0->unk1E0->unk74;
        func_ov003_02083bfc((u8*)arg0 + 0x2C, ((s32)(temp_r1_2 + ((u32)temp_r1_2 >> 0x1F)) >> 1) << 0xC, arg0->unk28);
        arg0->unk1E0->unk2C = (s32)arg0->unk28;
        arg0->unk1E0->unk30 = (s32)arg0->unk2C;
        arg0->unk1E0->unk34 = (s32)arg0->unk30;
        if ((arg0->unk1EC != 0) &&
            (((temp_r3_2 = arg0->unk1D0, (temp_r3_2 > 0)) && ((s32)arg0->unk28 > (s32)(temp_r4->unk28 - arg0->unk1F0))) ||
             ((temp_r3_2 < 0) && ((s32)arg0->unk28 < (s32)(temp_r4->unk28 + arg0->unk1F0)))) &&
            (func_ov003_020c3960(&sp8, arg0->unk1E0->unk74 << 0xC) == 0))
        {
            var_r3 = 0x2800;
            if (arg0->unk24 == 0) {
                var_r3 = -0x2800;
            }
            func_ov003_020c5ac0(arg0, 0xE5, (u8*)arg0->unk1E0 + 4, var_r3);
            temp_r0 = arg0->unk1E0;
            if (temp_r0 == arg0->unk1E4) {
                func_ov016_021286c0(temp_r0, arg0, 0xE7);
                arg0->unk1E0->unk34 = -0x120000;
                arg0->unk1E4        = 0;
            } else {
                temp_r0_2 = arg0->unk1E0;
                if (func_ov009_020fa71c(temp_r0_2) != 0) {
                    func_ov016_021286c0(temp_r0_2, arg0, 0xE6);
                } else {
                    func_ov016_021286c0(temp_r0_2, arg0, 0x191);
                }
            }
            arg0->unk1E0 = NULL;
            arg0->unk188 = 0;
            arg0->unk1E8 = 0;
        }
    }
    if (func_ov016_02126468(arg0) != 0) {
        temp_r0_3 = func_ov016_02126638(arg0);
        if (arg0->unk1E8 != 0) {
            *data_ov016_021292c0 = 2;
            func_ov016_02126688(arg0, func_ov016_02126854);
            return;
        }
        if ((temp_r0_3 == arg0->unk1E4) && (temp_r0_3 != NULL)) {
            if ((s32)arg0->unk28 < 0) {
                *data_ov016_021292c0 = 2;
                func_ov016_02126688(arg0, func_ov016_02126854);
                return;
            }
            *data_ov016_021292c0 = 3;
            func_ov016_02126688(arg0, func_ov016_02126c24);
            return;
        }
        if (func_ov003_0208495c(OV003_LIST_3D8B4) != NULL) {
            *data_ov016_021292c0 = 3;
            func_ov016_02126688(arg0, func_ov016_02126c24);
            return;
        }
        if (RNG_Next(2) != 0) {
            *data_ov016_021292c0 = 2;
            func_ov016_02126688(arg0, func_ov016_02126854);
            return;
        }
        *data_ov016_021292c0 = 3;
        func_ov016_02126688(arg0, func_ov016_02126c24);
        return;
    }
    arg0->unk1C0 = (s16)(arg0->unk1C0 + 1);
}

/* Boss retreat/transition logic. */
void func_ov016_02126c24(Ov016Obj* arg0) {
    Ov016Obj* temp_r5;

    temp_r5 = data_ov003_020e71b8->unk3D898;
    if (arg0->unk1C0 == 0) {
        func_ov003_020c4c5c(arg0);
        arg0->unk54  = (s32)(arg0->unk54 | 0x10);
        arg0->unk1DC = 0xCCD;
        arg0->unk2C  = (s32)temp_r5->unk2C;
        arg0->unk30  = -0x12C000;
        if (arg0->unk24 == 0) {
            arg0->unk1D0 = arg0->unk30 + 0x128000;
        } else {
            arg0->unk1D0 = 0x4000;
        }
        if (*data_ov016_021292c0 == 1) {
            func_ov003_020c4cc4(arg0, 0x1F2);
        }
        goto block_6;
    }
block_6:
    arg0->unk28 = (s32)(arg0->unk28 + arg0->unk1D0);
    if (func_ov016_02126468(arg0) != 0) {
        if (*data_ov016_021292c0 == 1) {
            func_ov016_02126688(arg0, func_ov016_021266f4);
            arg0->unk1C4 = 3;
            return;
        }
        *data_ov016_021292c0 = 4;
        func_ov016_02126688(arg0, func_ov016_02126d38);
        return;
    }
    arg0->unk1C0 = (s16)(arg0->unk1C0 + 1);
}

/* Boss chase/grab logic and target interaction. */
void func_ov016_02126d38(Ov016Obj* arg0) {
    f32       var_r0;
    s16       temp_r0_2;
    s32       temp_r0_3;
    s32       temp_r0_4;
    s32       temp_r1_2;
    Ov016Obj* temp_r1_3;
    s32       temp_r2_2;
    s32       temp_r2_3;
    s32       temp_r3;
    s32       var_r0_2;
    s32       var_r1;
    Ov016Obj* temp_r0;
    Ov016Obj* temp_r1;
    Ov016Obj* temp_r2;
    Ov016Obj* temp_r4;

    temp_r4 = data_ov003_020e71b8->unk3D898;
    if (arg0->unk1C0 == 0) {
        arg0->unk1E0 = func_ov016_02126638(arg0);
        func_ov003_020c4c5c(arg0);
        arg0->unk54  = (s32)(arg0->unk54 & ~0x10);
        arg0->unk1DC = 0x1000;
        arg0->unk1D4 = 0;
        temp_r0      = arg0->unk1E0;
        if (temp_r0 != NULL) {
            arg0->unk2C = (s32)temp_r0->unk30;
            temp_r0_2   = arg0->unk1E0->unk76;
            temp_r0_3   = temp_r0_2 << 0xC;
            if ((s32)temp_r0_2 > 0) {
                var_r0 = 0.5f + (f32)temp_r0_3;
            } else {
                var_r0 = (f32)temp_r0_3 - 0.5f;
            }
            arg0->unk30 = (s32)((0 - (s32)var_r0) - 0x8000);
        } else {
            arg0->unk2C = (s32)temp_r4->unk2C;
            arg0->unk30 = -0x40000;
        }
        var_r0_2 = 0x4000;
        if (arg0->unk24 == 0) {
            var_r0_2 = -0x4000;
        }
        arg0->unk1D0 = var_r0_2;
        func_ov003_020c4cc4(arg0, 0x1F2);
    }
    arg0->unk28 = (s32)(arg0->unk28 + arg0->unk1D0);
    temp_r2     = arg0->unk1E0;
    if (temp_r2 != NULL) {
        if (arg0->unk1E8 != 0) {
            temp_r2->unk2C      = (s32)arg0->unk28;
            arg0->unk1E0->unk30 = (s32)arg0->unk2C;
            arg0->unk1E0->unk34 = -0x8000;
        }
        temp_r2_2 = arg0->unk28;
        temp_r3   = arg0->unk1E0->unk2C;
        if ((temp_r2_2 >= (s32)(temp_r3 - 0x4000)) && (temp_r2_2 < (s32)(temp_r3 + 0x4000)) &&
            (func_ov016_02126514(arg0, arg0->unk1E0) != 0))
        {
            func_ov003_020c5a00((u8*)arg0->unk1E0 + 4);
            arg0->unk1E0->unk5C = 6;
            arg0->unk1E8        = 1;
            arg0->unk188        = (void*)arg0->unk1E0;
            func_ov003_020c4cc4(arg0, 0x1F3);
            temp_r1   = arg0->unk1E0;
            temp_r2_3 = temp_r1->unk76 * (temp_r1->unk74 << 0xD);
            temp_r1_2 = (s32)(temp_r2_3 + ((u32)(temp_r2_3 >> 5) >> 0x1A)) >> 6;
            var_r1    = (s32)(temp_r1_2 + ((u32)(temp_r1_2 >> 4) >> 0x1B)) >> 5;
            if (var_r1 > 0x2000) {
                var_r1 = 0x2000;
            }
            temp_r0_4 = arg0->unk1D0;
            if (arg0->unk24 == 0) {
                arg0->unk1D0 = (s32)(temp_r0_4 + var_r1);
            } else {
                arg0->unk1D0 = (s32)(temp_r0_4 - var_r1);
            }
            goto block_21;
        }
        goto block_21;
    }
block_21:
    if (arg0->unk1D4 == 0) {
        arg0->unk1D4 = func_ov003_020c5b2c(0xE4, arg0, arg0->unk28, arg0->unk2C, arg0->unk30);
    }
    func_ov016_02125a54(arg0);
    if (func_ov016_02126468(arg0) != 0) {
        arg0->unk54 = (s32)(arg0->unk54 | 0x10);
        if (arg0->unk1E8 == 0) {
            arg0->unk1E0 = NULL;
        }
        temp_r1_3 = arg0->unk1E4;
        if ((temp_r1_3 != 0) && (arg0->unk1E0 == temp_r1_3)) {
            *data_ov016_021292c0 = 2;
            func_ov016_02126688(arg0, func_ov016_0212682c);
            return;
        }
        *data_ov016_021292c0 = 5;
        func_ov016_02126688(arg0, func_ov016_0212682c);
        return;
    }
    arg0->unk1C0 = (s16)(arg0->unk1C0 + 1);
}

/* Boss death or phase-change visual sequence. */
void func_ov016_02126ff0(Ov016Obj* arg0) {
    Ov016Sprite subroutine;
    s32         temp_r5;

    if (arg0->unk1C0 == 0) {
        func_ov016_021256c0(&subroutine, 0, arg0->unkCA & 1);
        func_ov003_020c9064(&subroutine);
    }
    temp_r5 = func_ov003_020c9338(arg0);
    if ((func_ov003_020c9008() != 0) && (data_ov016_021291d8 != 0)) {
        func_ov003_02082cc4((u8*)arg0 + 0x84);
        data_ov016_021291d8 = 0;
    }
    if (temp_r5 == 0) {
        func_ov003_020c50bc();
    }
    if ((temp_r5 == 0) && (EasyFade_IsFading() == 0)) {
        arg0->unk1CC = NULL;
        return;
    }
    arg0->unk1C0 = (s16)(arg0->unk1C0 + 1);
}

/* Task stage 0: initialize boss controller and intro state. */
s32 func_ov016_021270a8(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Sprite     subroutine;
    Ov003ActorRoot* temp_r0;
    Ov016Obj*       temp_r4;
    void*           var_r1;

    (void)arg0;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x1F4);
    temp_r0 = data_ov003_020e71b8->unk3D350;
    var_r1  = Data_GetPackEntryData((Data*)(temp_r0), 66);

    temp_r4->unk1D8 = func_0200a968(data_0206b3cc[2], var_r1, 5, 1);
    func_ov003_020c3efc(temp_r4, arg2);
    func_ov016_021256c0(&subroutine, 2, 0);
    func_ov003_02082998((u8*)temp_r4 + 0x84, &subroutine);
    func_ov003_020c49c8(temp_r4);
    func_ov003_020c4c5c(temp_r4);
    temp_r4->unk54       = (s32)(temp_r4->unk54 | 0x10000010);
    temp_r4->unk3C       = 0;
    temp_r4->unk28       = -0x80000;
    temp_r4->unk2C       = 0;
    temp_r4->unk30       = -0x40000;
    temp_r4->unk1DC      = 0x1000;
    temp_r4->unk1CC      = (void (*)(Ov016Obj*))1;
    temp_r4->unk1E4      = func_ov016_02128b90(temp_r4);
    *data_ov016_021292c0 = 0;
    func_ov016_02126688(temp_r4, func_ov016_021266b4);
    return 1;
}

/* Task stage 1: update boss controller and spawn hazards. */
s32 func_ov016_021271c0(void* arg0, Ov016Task* arg1, void* arg2) {
    void (*temp_r1)(Ov016Obj*);
    s32       temp_r0;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c3bf0(temp_r4) != 0) {
        return 1;
    }
    temp_r0 = func_ov003_02082f2c(temp_r4);
    if ((temp_r0 != 1) && (temp_r0 == 3)) {
        func_ov016_02126688(temp_r4, func_ov016_02126ff0);
    }
    temp_r1 = temp_r4->unk1C8;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    if (((s32)*data_ov016_021292c0 > 1) && (func_ov003_020cb3c4(0, 7) < 4) && (RNG_Next(0xB4) == 0) && !(temp_r4->unk54 & 4)) {
        func_ov016_021264d8();
    }
    func_ov003_02083000(0, temp_r4);
    func_ov003_02082b0c((u8*)temp_r4 + 0x84);
    return temp_r4->unk1CC;
}

/* Task stage 2: update boss controller visuals. */
s32 func_ov016_02127280(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       spA;
    s16       sp8;
    s32       temp_r0;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    temp_r0 = temp_r4->unk54;
    if (temp_r0 & 1) {
        temp_r4->unk54 = (s32)(temp_r0 & ~1);
    }
    if (!(temp_r4->unk18C & 1)) {
        func_ov003_02084348(0, &spA, &sp8, temp_r4->unk28, temp_r4->unk2C, temp_r4->unk30);
        func_ov003_02082724((u8*)temp_r4 + 0x84, spA, sp8);
        func_ov003_02082b64((u8*)temp_r4 + 0x84);
        temp_r4->unk34 = spA;
        temp_r4->unk36 = (s16)(sp8 + 0x50);
        func_ov003_02084348(0, &spA, &sp8, temp_r4->unk28, temp_r4->unk2C, 0);
        func_ov003_02082724((u8*)temp_r4 + 0x84, spA, sp8);
        func_ov016_021262d8(temp_r4);
    }
    return 1;
}

/* Task stage 3: cleanup boss controller. */
s32 func_ov016_02127340(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    func_0200afec(data_0206b3cc[2], temp_r4->unk1D8);
    func_ov003_02082cc4((u8*)temp_r4 + 0x84);
    func_ov003_020c4a74(temp_r4);
    return 1;
}

/* Task dispatcher for boss controller. */
s32 func_ov016_02127374(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_02128fe4[stage](pool, task_data, args);
}

/* Initialize parameters for a flying debris arc. */
void func_ov016_021273bc(Ov016Obj* arg0) {
    f32 var_r0;
    s32 temp_r5;
    s32 temp_r5_2;
    u32 temp_r0;
    u64 temp_r3;

    arg0->unk6C = (s32)(RNG_Next(0xCCE) + 0xCCD);
    arg0->unk74 = (s32)(RNG_Next(0x99A) + 0x99A);
    arg0->unk80 = (s32)(0 - (RNG_Next(0x19) + 0x19));
    if (RNG_Next((s32)data_ov003_020e71b8->unk3D824 >> 0xC) != 0) {
        var_r0 = 0.5f + (f32)(RNG_Next((s32)data_ov003_020e71b8->unk3D824 >> 0xC) << 0xC);
    } else {
        var_r0 = (f32)(RNG_Next((s32)data_ov003_020e71b8->unk3D824 >> 0xC) << 0xC) - 0.5f;
    }
    arg0->unk60 = (s32)var_r0;
    arg0->unk64 = 0;
    temp_r5     = arg0->unk74;
    temp_r3     = ((u64)(u32)temp_r5 * (u64)(u32)temp_r5) + 0x800;
    temp_r5_2   = RNG_Next(0x1D) + 0x64;
    temp_r0     = (u32)FX_Divide((s32)(temp_r3 >> 0xC), arg0->unk80);
    arg0->unk68 = (s32)((0 - (temp_r5_2 << 0xC)) + ((s32)(temp_r0 + (temp_r0 >> 0x1F)) >> 1));
    if (RNG_Next(4) != 0) {
        arg0->unk60 = (s32)(arg0->unk60 - 0x80000);
        return;
    }
    arg0->unk6C = (s32)(arg0->unk6C * -1);
    arg0->unk60 = (s32)(arg0->unk60 + 0x80000);
    arg0->unk68 = (s32)(arg0->unk68 - 0x14000);
}

/* Task stage 0: spawn airborne debris effect. */
s32 func_ov016_0212751c(void* arg0, Ov016Task* arg1, void* arg2) {
    f32       var_r0;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x8C);
    func_ov003_02082a04(1, temp_r4, &data_ov016_02128f98, &data_ov016_02129010, RNG_Next(2), 4, 4);
    func_ov016_021273bc(temp_r4);
    if (RNG_Next((s32)data_ov003_020e71b8->unk3D824 >> 0xC) != 0) {
        var_r0 = 0.5f + (f32)(RNG_Next((s32)data_ov003_020e71b8->unk3D824 >> 0xC) << 0xC);
    } else {
        var_r0 = (f32)(RNG_Next((s32)data_ov003_020e71b8->unk3D824 >> 0xC) << 0xC) - 0.5f;
    }
    temp_r4->unk60 = (s32)var_r0;
    return 1;
}

/* Task stage 1: animate airborne debris. */
s32 func_ov016_02127604(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       temp_r0;
    s16       temp_r1_2;
    s32       temp_r1;
    s32       temp_r2;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    temp_r0 = temp_r4->unk88;
    switch (temp_r0) { /* irregular */
        case 0:
            temp_r4->unk60 = (s32)(temp_r4->unk60 + temp_r4->unk6C);
            temp_r1        = temp_r4->unk74 + temp_r4->unk80;
            temp_r4->unk74 = temp_r1;
            temp_r4->unk68 = (s32)(temp_r4->unk68 + temp_r1);
            temp_r2        = temp_r4->unk6C;
            if (((temp_r2 < 0) && ((s32)temp_r4->unk60 < -0x10000)) ||
                ((temp_r2 > 0) && ((s32)temp_r4->unk60 > (s32)(data_ov003_020e71b8->unk3D824 + 0x10000))))
            {
                temp_r4->unk88 = (s16)(temp_r4->unk88 + 1);
            }
            break;
        case 1:
            if (temp_r4->unk84 == 0) {
                temp_r4->unk86 = (s16)RNG_Next(0xB4);
            }
            temp_r1_2 = temp_r4->unk84;
            if ((s32)temp_r1_2 <= (s32)temp_r4->unk86) {
                temp_r4->unk84 = (s16)(temp_r1_2 + 1);
            } else {
                func_ov016_021273bc(temp_r4);
                temp_r4->unk84 = 0;
                temp_r4->unk88 = 0;
            }
            break;
    }
    func_ov003_02082b0c(temp_r4);
    return 1;
}

/* Task stage 2: render airborne debris. */
s32 func_ov016_02127720(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       spA;
    s16       sp8;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk60, temp_r4->unk64, temp_r4->unk68);
    func_ov003_02082724(temp_r4, spA, sp8);
    func_ov003_02082730(temp_r4, 0x7FFFFFFF);
    func_ov003_02082b64(temp_r4);
    return 1;
}

/* Task stage 3: cleanup debris. */
s32 func_ov016_02127798(void* arg0, Ov016Task* arg1, void* arg2) {
    (void)arg0;
    (void)arg2;
    func_ov003_02082cc4(arg1->unk18);
    return 1;
}

/* Task dispatcher for airborne debris. */
s32 func_ov016_021277ac(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_02129000[stage](pool, task_data, args);
}

/* Spawn a burst of flying objects (RG curve). */
void func_ov016_021277f4(void) {
    s32 var_r7;

    var_r7 = 0;
    do {
        EasyTask_CreateTask(OV003_TASK_POOL, &data_ov016_02128ff4, 0, 0, 0, NULL);
        var_r7 += 1;
    } while (var_r7 < 5);
}

/* Choose a random spawn position within bounds. */
void func_ov016_0212784c(Ov016Vec3* arg1) {
    s32 temp_r5;
    s32 temp_r6;

    temp_r5 = (s32)data_ov003_020e71b8->unk3D7CC >> 0xC;
    if (RNG_Next(2) != 0) {
        arg1->unk0 = (s32)(((((s32)(temp_r5 + ((u32)temp_r5 >> 0x1F)) >> 1) - 0x30) + RNG_Next(0x41)) << 0xC);
        arg1->unk4 = (s32)((RNG_Next(0x11) - 0x26) << 0xC);
        arg1->unk8 = 0;
        return;
    }
    temp_r6    = (s32)(temp_r5 + ((u32)temp_r5 >> 0x1F)) >> 1;
    arg1->unk0 = (s32)((temp_r6 + 0x30 + RNG_Next(temp_r5 - (temp_r6 + 0x2F))) << 0xC);
    arg1->unk4 = (s32)((RNG_Next(0x11) - 0x26) << 0xC);
    arg1->unk8 = 0;
}

/* Task stage 0: initialize falling object pattern. */
s32 func_ov016_02127900(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x80);
    func_ov003_02082a04(0, temp_r4, &data_ov016_02128f98, &data_ov016_0212903c, 0, 4, 4);
    func_ov016_0212784c((Ov016Vec3*)((u8*)temp_r4 + 0x60));
    return 1;
}

/* Task stage 1: update falling object pattern and phases. */
s32 func_ov016_02127960(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       temp_r0_2;
    s16       temp_r0_3;
    s16       temp_r1;
    s16       temp_r1_4;
    s32       temp_r0;
    s32       temp_r1_2;
    s32       temp_r1_3;
    s32       var_r1;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    temp_r0_2 = temp_r4->unk7C;
    switch (temp_r0_2) {
        default:
            func_ov003_02082b0c(temp_r4);
            return 1;
        case 0:
            if (temp_r4->unk78 == 0) {
                func_ov000_020824a0(temp_r4, (u16)(RNG_Next(2) + 1), 0);
                temp_r4->unk7A = (s16)(RNG_Next(0x12D) + 0x12C);
            }
            temp_r1 = temp_r4->unk78;
            if ((s32)temp_r1 <= (s32)temp_r4->unk7A) {
                temp_r4->unk78 = (s16)(temp_r1 + 1);
            } else {
                temp_r4->unk78 = 0;
                temp_r4->unk7C = (s16)(temp_r4->unk7C + 1);
            }
            /* Duplicate return node #39. Try simplifying control flow for better match */
            func_ov003_02082b0c(temp_r4);
            return 1;
        case 1:
            if (temp_r4->unk78 == 0) {
                func_ov000_020824a0(temp_r4, 0U, 0);
                var_r1 = 0x1000;
                if (RNG_Next(2) == 0) {
                    var_r1 = -0x1000;
                }
                temp_r4->unk6C = var_r1;
                temp_r4->unk74 = -0x2000;
            }
            temp_r1_2      = temp_r4->unk74 - 0x800;
            temp_r4->unk74 = temp_r1_2;
            if (temp_r1_2 < -0x4000) {
                temp_r4->unk74 = -0x4000;
            }
            temp_r4->unk60 = (s32)(temp_r4->unk60 + temp_r4->unk6C);
            temp_r1_3      = temp_r4->unk68 + temp_r4->unk74;
            temp_r4->unk68 = temp_r1_3;
            if (temp_r1_3 < -0x60000) {
                temp_r4->unk78 = 0;
                temp_r4->unk7C = (s16)(temp_r4->unk7C + 1);
            } else {
                temp_r4->unk78 = (s16)(temp_r4->unk78 + 1);
            }
            /* Duplicate return node #39. Try simplifying control flow for better match */
            func_ov003_02082b0c(temp_r4);
            return 1;
        case 2:
            if (temp_r4->unk78 == 0) {
                temp_r4->unk7A = (s16)(RNG_Next(0x12D) + 0x12C);
            }
            temp_r1_4 = temp_r4->unk78;
            if ((s32)temp_r1_4 <= (s32)temp_r4->unk7A) {
                temp_r4->unk78 = (s16)(temp_r1_4 + 1);
            } else {
                temp_r4->unk78 = 0;
                temp_r4->unk7C = (s16)(temp_r4->unk7C + 1);
            }
            /* Duplicate return node #39. Try simplifying control flow for better match */
            func_ov003_02082b0c(temp_r4);
            return 1;
        case 3:
            if (temp_r4->unk78 == 0) {
                func_ov016_0212784c((Ov016Vec3*)((u8*)temp_r4 + 0x6C));
                temp_r0 = temp_r4->unk6C;
                if (RNG_Next(2) != 0) {
                    temp_r4->unk60 = (s32)(temp_r0 + 0x20000);
                } else {
                    temp_r4->unk60 = (s32)(temp_r0 - 0x20000);
                }
                temp_r4->unk78 = 0x1E;
                goto block_35;
            }
        block_35:
            temp_r0_3 = temp_r4->unk78;
            if ((s32)temp_r0_3 > 0) {
                func_02026590((u8*)temp_r4 + 0x60, temp_r4->unk6C, (u16)temp_r0_3);
                func_020265d4((u8*)temp_r4 + 0x64, temp_r4->unk70, (u16)temp_r4->unk78);
                func_020265d4((u8*)temp_r4 + 0x68, temp_r4->unk74, (u16)temp_r4->unk78);
                temp_r4->unk78 = (s16)(temp_r4->unk78 - 1);
            }
            if ((s32)temp_r4->unk78 <= 0) {
                temp_r4->unk78 = 0;
                temp_r4->unk7C = 0;
            }
            /* Duplicate return node #39. Try simplifying control flow for better match */
            func_ov003_02082b0c(temp_r4);
            return 1;
    }
}

/* Task stage 2: render falling object. */
s32 func_ov016_02127bc4(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       spA;
    s16       sp8;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk60, temp_r4->unk64, temp_r4->unk68);
    func_ov003_02082724(temp_r4, spA, sp8);
    func_ov003_02082730(temp_r4, 0x7FFFFFFF);
    func_ov003_02082b64(temp_r4);
    return 1;
}

/* Task stage 3: cleanup falling object. */
s32 func_ov016_02127c3c(void* arg0, Ov016Task* arg1, void* arg2) {
    (void)arg0;
    (void)arg2;
    func_ov003_02082cc4(arg1->unk18);
    return 1;
}

/* Task dispatcher for falling object pattern. */
s32 func_ov016_02127c50(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_0212902c[stage](pool, task_data, args);
}

/* Spawn a burst of flying objects (UG). */
void func_ov016_02127c98(void) {
    s32 var_r7;

    var_r7 = 0;
    do {
        EasyTask_CreateTask(OV003_TASK_POOL, &data_ov016_02129020, 0, 0, 0, NULL);
        var_r7 += 1;
    } while (var_r7 < 5);
}

/* Task stage 0: initialize landing RG effect. */
s32 func_ov016_02127ce8(void* arg0, Ov016Task* arg1, void* arg2) {
    u32       temp_r1;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x70);
    func_ov003_02082a04(1, temp_r4, &data_ov016_02128f98, &data_ov016_02129054, 0, 4, 0x3E);
    temp_r1        = data_ov003_020e71b8->unk3D824;
    temp_r4->unk60 = (s32)((s32)(temp_r1 + (temp_r1 >> 0x1F)) >> 1);
    temp_r4->unk64 = 0;
    temp_r4->unk68 = -0x4B000;
    return 1;
}

/* Task stage 1: update landing RG effect. */
s32 func_ov016_02127d74(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02082b0c(temp_r4);
    return 1;
}

/* Task stage 2: render landing RG effect. */
s32 func_ov016_02127dac(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       spA;
    s16       sp8;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk60, temp_r4->unk64, temp_r4->unk68);
    func_ov003_02082724(temp_r4, spA, sp8);
    func_ov003_02082730(temp_r4, 0x7FFFFFFF);
    func_ov003_02082b64(temp_r4);
    return 1;
}

/* Task stage 3: cleanup landing RG effect. */
s32 func_ov016_02127e24(void* arg0, Ov016Task* arg1, void* arg2) {
    (void)arg0;
    (void)arg2;
    func_ov003_02082cc4(arg1->unk18);
    return 1;
}

/* Task dispatcher for landing RG effect. */
s32 func_ov016_02127e38(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_02129068[stage](pool, task_data, args);
}

/* Spawn single landing RG effect. */
void func_ov016_02127e80(void) {
    EasyTask_CreateTask(OV003_TASK_POOL, &data_ov016_0212905c, 0, 0, 0, NULL);
}

/* Task stage 0: initialize landing UG effect. */
s32 func_ov016_02127eb8(void* arg0, Ov016Task* arg1, void* arg2) {
    u32       temp_r0;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x78);
    func_ov003_02082a04(0, temp_r4, &data_ov016_02128f98, &data_ov016_02129078, 0, 4, 0xC);
    temp_r0        = data_ov003_020e71b8->unk3D7CC;
    temp_r4->unk60 = (s32)((s32)(temp_r0 + (temp_r0 >> 0x1F)) >> 1);
    temp_r4->unk64 = -0x1E000;
    temp_r4->unk68 = 0;
    return 1;
}

/* Task stage 1: update landing UG effect. */
s32 func_ov016_02127f44(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02082b0c(temp_r4);
    return 1;
}

/* Task stage 2: render landing UG effect. */
s32 func_ov016_02127f7c(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       spA;
    s16       sp8;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk60, temp_r4->unk64, temp_r4->unk68);
    func_ov003_02082724(temp_r4, spA, sp8);
    func_ov003_02082730(temp_r4, 0x7FFFFFFF);
    func_ov003_02082b64(temp_r4);
    return 1;
}

/* Task stage 3: cleanup landing UG effect. */
s32 func_ov016_02127ff4(void* arg0, Ov016Task* arg1, void* arg2) {
    (void)arg0;
    (void)arg2;
    func_ov003_02082cc4(arg1->unk18);
    return 1;
}

/* Task dispatcher for landing UG effect. */
s32 func_ov016_02128008(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_0212908c[stage](pool, task_data, args);
}

/* Spawn single landing UG effect. */
void func_ov016_02128050(void) {
    EasyTask_CreateTask(OV003_TASK_POOL, &data_ov016_02129080, 0, 0, 0, NULL);
}

/* Task stage 0: initialize bomb/obstacle spawner. */
s32 func_ov016_02128088(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016SpawnMeta* temp_r2;
    s32*            temp_r3;
    Ov016Obj*       temp_r4;
    Ov016SpawnArgs* args;

    (void)arg0;
    args    = (Ov016SpawnArgs*)arg2;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x84);
    temp_r3        = (s32*)((u8*)temp_r4 + 0x60);
    temp_r4->unk60 = (s32)args->unk0;
    temp_r3[1]     = (s32)args->unk4;
    temp_r3[2]     = (s32)args->unk8;
    temp_r4->unk74 = (s32)args->unk10;
    temp_r4->unk7C = (s16)args->unk14;
    temp_r2        = (Ov016SpawnMeta*)args->unkC;
    temp_r4->unk6C = (u16)temp_r2->unk0;
    temp_r4->unk6E = (u16)temp_r2->unk2;
    temp_r4->unk70 = (u16)temp_r2->unk4;
    temp_r4->unk72 = (u16)temp_r2->unk6;
    func_ov003_02082a04(func_ov003_020c3aac(temp_r4->unk74), temp_r4, &data_ov016_0212909c, &data_ov016_021290a4, 0, 7, 0);
    func_ov003_020c38e8(temp_r4);
    return 1;
}

/* Task stage 1: update bomb/obstacle lifecycle. */
s32 func_ov016_0212812c(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       temp_r0;
    s16       temp_r1;
    s32       var_r5;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    var_r5  = 1;
    temp_r0 = temp_r4->unk80;
    switch (temp_r0) { /* irregular */
        case 0:
            temp_r1 = temp_r4->unk7E;
            if ((s32)temp_r1 < (s32)temp_r4->unk7C) {
                temp_r4->unk7E = (s16)(temp_r1 + 1);
            } else {
                temp_r4->unk7E = 0;
                temp_r4->unk80 = (s16)(temp_r4->unk80 + 1);
            }
        default:
            func_ov003_02082b0c(temp_r4);
            return var_r5;
        case 1:
            if (temp_r4->unk7E == 0) {
                func_ov000_020824a0(temp_r4, 0U, 1);
                func_ov003_02087f28(0x1F5, temp_r4->unk60);
            }
            if (temp_r4->unk78 == 0) {
                temp_r4->unk78 = func_ov003_020c5bc4((u16)temp_r4->unk74, (u8*)temp_r4 + 0x6C, temp_r4->unk60, temp_r4->unk64,
                                                     temp_r4->unk68);
            }
            if (func_0200ea4c(temp_r4) == 0) {
                temp_r4->unk7E = (s16)(temp_r4->unk7E + 1);
            } else {
                var_r5 = 0;
            }
            /* Duplicate return node #13. Try simplifying control flow for better match */
            func_ov003_02082b0c(temp_r4);
            return var_r5;
    }
}

/* Task stage 2: render bomb/obstacle. */
s32 func_ov016_0212820c(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       spA;
    s16       sp8;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
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

/* Task stage 3: cleanup bomb/obstacle. */
s32 func_ov016_021282a0(void* arg0, Ov016Task* arg1, void* arg2) {
    (void)arg0;
    (void)arg2;
    func_ov003_02082cc4(arg1->unk18);
    return 1;
}

/* Task dispatcher for bomb/obstacle spawner. */
s32 func_ov016_021282b4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_021290b8[stage](pool, task_data, args);
}

/* Convenience wrapper to spawn a single falling effect. */
void func_ov016_021282fc(s32 arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_ov016_02128320(arg0, arg1, arg2, arg3, arg4, 0);
}

/* Spawn a falling effect with optional variant index. */
void func_ov016_02128320(s32 arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s16   sp1C;
    s32   sp18;
    void* sp14;
    s32   sp10;
    s32   spC;
    s32   sp8;

    sp8  = arg2;
    spC  = arg3;
    sp10 = arg4;
    sp14 = arg1;
    sp18 = arg0;
    sp1C = (s16)arg5;
    func_ov003_020c3aac(arg0);
    EasyTask_CreateTask(func_ov003_020c3a10(), &data_ov016_021290ac, 0, 0, 0, &sp8);
}

/* Task stage 0: initialize falling debris from a source object. */
s32 func_ov016_02128374(void* arg0, Ov016Task* arg1, void* arg2) {
    s32             temp_r0;
    u16             temp_r1;
    Ov016Obj*       temp_r4;
    Ov016SpawnMeta* temp_r7;
    Ov016SpawnArgs* args;

    (void)arg0;
    args    = (Ov016SpawnArgs*)arg2;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x78);
    temp_r4->unk60 = (s32)args->unk0;
    temp_r4->unk64 = (s32)args->unk4;
    temp_r4->unk68 = (s32)args->unk8;
    temp_r7        = (Ov016SpawnMeta*)args->unk8;
    temp_r4->unk6C = (u16)temp_r7->unk4;
    temp_r4->unk6E = (u16)temp_r7->unk6;
    temp_r4->unk70 = (u16)temp_r7->unk8;
    temp_r4->unk72 = (u16)temp_r7->unkA;
    temp_r4->unk74 = (s32)args->unkC;
    func_ov003_02082a04(0, temp_r4, &data_ov016_0212909c, &data_ov016_021290e4, 0, 7, 0);
    temp_r1 = ((Ov016SpriteParams*)temp_r4->unk60)->unk2;
    if ((s32)temp_r1 <= 0xE) {
        temp_r0 = temp_r1 - 0xA;
        if (temp_r0 >= 0) {
            switch (temp_r0) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                    break;
            }
        } else {
            goto block_10;
        }
    } else if (temp_r1 != 0x19) {
    block_10:
        func_ov003_020c38e8(temp_r4);
    }
    func_ov003_02087f28(0x1F4, ((Ov016Obj*)temp_r4->unk64)->unk28);
    return 1;
}

/* Task stage 1: update falling debris, spawn sub-effects on impact. */
s32 func_ov016_0212845c(void* arg0, Ov016Task* arg1, void* arg2) {
    s32       temp_r1;
    s32       temp_r2;
    s32       var_r5;
    Ov016Obj* temp_r0_obj;
    Ov016Obj* temp_r3;
    Ov016Obj* temp_r3_2;
    Ov016Obj* temp_r3_3;
    Ov016Obj* temp_r3_4;
    Ov016Obj* temp_r3_5;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    var_r5  = 1;
    temp_r3 = (Ov016Obj*)temp_r4->unk64;
    temp_r2 = temp_r3->unk30;
    if (temp_r2 >= 0) {
        temp_r1 = temp_r4->unk74;
        var_r5  = 0;
        if (temp_r1 != 0x191) {
            if (temp_r1 == 0xE6) {
                func_ov016_021282fc(temp_r4->unk74, (u8*)temp_r4 + 0x6C, temp_r3->unk28, temp_r3->unk2C, temp_r2);
            } else {
                func_ov016_02128320(temp_r4->unk74, (u8*)temp_r4 + 0x6C, temp_r3->unk28, temp_r3->unk2C, temp_r2, 0);
                temp_r3_2 = (Ov016Obj*)temp_r4->unk64;
                func_ov016_02128320(temp_r4->unk74, (u8*)temp_r4 + 0x6C, temp_r3_2->unk28 + 0x20000, temp_r3_2->unk2C,
                                    temp_r3_2->unk30 - 0x10000, 4);
                temp_r3_3 = (Ov016Obj*)temp_r4->unk64;
                func_ov016_02128320(temp_r4->unk74, (u8*)temp_r4 + 0x6C, temp_r3_3->unk28 - 0x28000, temp_r3_3->unk2C,
                                    temp_r3_3->unk30 - 0x30000, 7);
                temp_r3_4 = (Ov016Obj*)temp_r4->unk64;
                func_ov016_02128320(temp_r4->unk74, (u8*)temp_r4 + 0x6C, temp_r3_4->unk28 + 0x18000, temp_r3_4->unk2C,
                                    temp_r3_4->unk30 - 0x40000, 0xE);
                temp_r3_5 = (Ov016Obj*)temp_r4->unk64;
                func_ov016_02128320(temp_r4->unk74, (u8*)temp_r4 + 0x6C, temp_r3_5->unk28 - 0x10000, temp_r3_5->unk2C,
                                    temp_r3_5->unk30 - 0x68000, 0xE);
            }
            temp_r0_obj = (Ov016Obj*)temp_r4->unk60;
            if (temp_r0_obj == NULL) {
                func_ov003_02082f1c(temp_r3, 3);
            } else if (((Ov016SpriteParams*)temp_r0_obj)->unk2 == 0x19) {
                func_ov003_02082f1c((u8*)temp_r0_obj + 4, 1);
            } else {
                func_ov003_02086654();
            }
        }
    }
    func_ov003_02082b0c(temp_r4);
    return var_r5;
}

/* Task stage 2: render debris relative to source object. */
s32 func_ov016_021285e0(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       spA;
    s16       sp8;
    Ov016Obj* temp_r4;
    Ov016Obj* temp_r5;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    temp_r5 = (Ov016Obj*)temp_r4->unk64;
    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r5->unk28, temp_r5->unk2C, temp_r5->unk30);
    func_ov003_02082724(temp_r4, spA, sp8);
    func_ov003_02082730(temp_r4, 0x7FFFEFFF - temp_r5->unk2C);
    func_ov003_02082b64(temp_r4);
    return 1;
}

/* Task stage 3: cleanup debris. */
s32 func_ov016_02128658(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (temp_r4->unk20 != 0) {
        func_ov003_02082cc4(temp_r4);
    }
    return 1;
}

/* Task dispatcher for falling debris. */
s32 func_ov016_02128678(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_021290d4[stage](pool, task_data, args);
}

/* Spawn a fall effect tied to a target and source. */
void func_ov016_021286c0(Ov016Obj* arg0, Ov016Obj* arg1, s32 arg2) {
    s32   sp14;
    void* sp10;
    s32   spC;
    void* sp8;

    sp10 = arg1;
    sp14 = arg2;
    sp8  = arg0;
    spC  = (s32)((u8*)arg0 + 4);
    EasyTask_CreateTask(OV003_TASK_POOL, &data_ov016_021290c8, 0, 0, 0, &sp8);
}

/* Set boss sub-behavior callback for a multi-part actor. */
void func_ov016_02128710(Ov016Obj* arg0, void (*arg1)(Ov016Obj*)) {
    arg0->unk188 = (void*)arg1;
    arg0->unk18C = 0;
    arg0->unk18E = 0;
}

/* Trigger overlay effects when a phase starts. */
void func_ov016_02128728(Ov016Obj* arg0) {
    if (!(arg0->unk58 & 0x200)) {
        return;
    }
    if ((*data_ov016_021292c0 == 2) && (arg0->unk14C == arg0->unkEC)) {
        arg0->unk74 = 0x3C;
        arg0->unk76 = 0x64;
        func_ov000_020824a0((u8*)arg0 + 0xC4, 2U, 0);
        func_ov003_020826c8((u8*)arg0 + 0xC4, 0xB);
        func_ov003_02082cc4((u8*)arg0 + 0x124);
        func_ov003_020880e4(0, (u8*)arg0 + 0x124, (u8*)arg0 + 4);
    }
}

/* No-op sub-behavior. */
void func_ov016_021287a4(Ov016Obj* arg0) {
    (void)arg0;
}

/* Reset a phase flag. */
void func_ov016_021287a8(Ov016Obj* arg0) {
    arg0->unk194 = 0;
}

/* No-op sub-behavior. */
void func_ov016_021287b4(Ov016Obj* arg0) {
    (void)arg0;
}

/* Blink/flash sequence for a multi-part actor. */
void func_ov016_021287b8(Ov016Obj* arg0) {
    s16 temp_lr;

    if (arg0->unk18E == 0) {
        arg0->unk58 = (s32)(arg0->unk58 | 0x10);
    }
    temp_lr = arg0->unk18E;
    if ((s32)temp_lr < 0x14) {
        arg0->unk190 = (arg0->unk190 == 0) ? 1 : 0;
    } else {
        arg0->unk190 = ((temp_lr % 3) == 0) ? 1 : 0;
    }
    if ((s32)temp_lr <= 0x14) {
        arg0->unk18E = (s16)(temp_lr + 1);
        return;
    }
    arg0->unk190 = 0;
    arg0->unk194 = 0;
}

/* Task stage 0: initialize boss bus/obstacle actor. */
s32 func_ov016_02128850(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x198);
    temp_r4->unk184 = (s32) * (s32*)arg2;
    temp_r4->unk190 = 1;
    temp_r4->unk194 = 1;
    func_ov003_02083118((u8*)temp_r4 + 4, 2);
    temp_r4->unk2  = 0x19;
    temp_r4->unk2C = 0x180000;
    temp_r4->unk30 = 0x9000;
    temp_r4->unk34 = 0;
    temp_r4->unk74 = 0x64;
    temp_r4->unk76 = 0x64;
    temp_r4->unk40 = 0xCD;
    temp_r4->unk4C = 0xE66;
    func_ov003_02082a04(0, (u8*)temp_r4 + 0xC4, &data_ov016_021290fc, &data_ov016_02129120, 0, 0xA, 0x180);
    func_ov003_02082a04(0, (u8*)temp_r4 + 0x124, &data_ov016_021290fc, &data_ov016_02129120, 1, 0xA, 0);
    func_ov000_020824a0((u8*)temp_r4 + 0xC4, 0U, 0);
    func_ov000_020824a0((u8*)temp_r4 + 0x124, 1U, 0);
    func_ov016_02128710(temp_r4, func_ov016_021287a4);
    return 1;
}

/* Task stage 1: update bus/obstacle actor and sub-behavior. */
s32 func_ov016_02128960(void* arg0, Ov016Task* arg1, void* arg2) {
    void (*temp_r1)(Ov016Obj*);
    s32       temp_r0;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 1;
    }
    temp_r0 = func_ov003_02082f2c((u8*)temp_r4 + 4);
    switch (temp_r0) { /* irregular */
        case 1:
            func_ov016_02128710(temp_r4, func_ov016_021287b8);
            break;
        case 3:
            func_ov016_02128710(temp_r4, func_ov016_021287a8);
            break;
        case 5:
            func_ov016_02128710(temp_r4, func_ov016_021287b4);
            break;
    }
    temp_r1 = temp_r4->unk188;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov003_02083074((u8*)temp_r4 + 4);
    func_ov016_02128728(temp_r4);
    func_ov003_02082b0c((u8*)temp_r4 + 0xC4);
    func_ov003_02082b0c((u8*)temp_r4 + 0x124);
    return temp_r4->unk194;
}

/* Task stage 2: render bus/obstacle actor. */
s32 func_ov016_02128a20(void* arg0, Ov016Task* arg1, void* arg2) {
    s16       temp_r4;
    s16       temp_r6;
    Ov016Obj* temp_r5;

    (void)arg0;
    (void)arg2;
    temp_r5 = arg1->unk18;
    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 1;
    }
    if (temp_r5->unk190 == 0) {
        return 1;
    }
    temp_r4 = func_ov003_020843b0(0, temp_r5->unk2C);
    temp_r6 = func_ov003_020843ec(0, temp_r5->unk30, temp_r5->unk34);
    if (temp_r5->unk108 == 0) {
        func_ov003_02082724((u8*)temp_r5 + 0x124, temp_r4, (s16)(temp_r6 + 8));
        func_ov003_02082730((u8*)temp_r5 + 0x124, 0x7FFFEFFF);
        func_ov003_02082b64((u8*)temp_r5 + 0x124);
        func_ov003_02082724((u8*)temp_r5 + 0xC4, temp_r4, temp_r6);
        func_ov003_02082730((u8*)temp_r5 + 0xC4, 0x7FFFEFFF);
        func_ov003_02082b64((u8*)temp_r5 + 0xC4);
    } else {
        func_ov003_02082724((u8*)temp_r5 + 0xC4, temp_r4, temp_r6);
        func_ov003_02082730((u8*)temp_r5 + 0xC4, 0x7FFFFFFF - temp_r5->unk30);
        func_ov003_02082b64((u8*)temp_r5 + 0xC4);
        func_ov003_0208810c((u8*)temp_r5 + 0x124, (u8*)temp_r5 + 4);
    }
    return 1;
}

/* Task stage 3: cleanup bus/obstacle actor. */
s32 func_ov016_02128b28(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    func_ov003_02082cc4((u8*)temp_r4 + 0xC4);
    func_ov003_02082cc4((u8*)temp_r4 + 0x124);
    return 1;
}

/* Task dispatcher for bus/obstacle actor. */
s32 func_ov016_02128b48(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_02129110[stage](pool, task_data, args);
}

/* Spawn the bus/obstacle actor and return its data pointer. */
void* func_ov016_02128b90(Ov016Obj* arg0) {
    void* sp8;
    s32   task_id;

    sp8     = arg0;
    task_id = EasyTask_CreateTask(OV003_TASK_POOL, &data_ov016_02129104, 0, 0, 0, &sp8);
    return EasyTask_GetTaskData(OV003_TASK_POOL, task_id);
}

/* Switch boss sub-state for obstacle behavior. */
void func_ov016_02128be0(Ov016Obj* arg0, void (*arg1)(Ov016Obj*)) {
    arg0->unk28C = arg1;
    arg0->unk290 = 0;
    arg0->unk292 = 0;
}

/* No-op sub-state. */
void func_ov016_02128bf8(Ov016Obj* arg0) {
    (void)arg0;
}

/* Start obstacle death/escape sequence. */
void func_ov016_02128bfc(Ov016Obj* arg0) {
    arg0->unk54 = (s32)(arg0->unk54 | 6);
    func_ov003_020c4a34(arg0);
    if (data_ov003_020e71b8->unk3D8A0 == arg0) {
        data_ov003_020e71b8->unk3D8A0 = 0;
    }
    if (arg0->unk18C & 0x1000) {
        arg0->unk294 = 0;
        return;
    }
    if (func_ov009_020fa71c((u8*)arg0 + 0x1C0) != 0) {
        func_ov016_021282fc(0xE9, (u8*)arg0 + 0x284, arg0->unk28, arg0->unk2C, arg0->unk30);
        arg0->unk294 = 0;
        return;
    }
    func_ov016_02128be0(arg0, func_ov016_02128ca8);
}

/* Move obstacle toward boss and resolve ground stop. */
void func_ov016_02128ca8(Ov016Obj* arg0) {
    s32       var_r0;
    Ov016Obj* temp_r5;

    temp_r5 = data_ov003_020e71b8->unk3D89C;
    if (arg0->unk292 == 0) {
        if (arg0->unk1C2 == 0) {
            func_ov000_020824a0((u8*)arg0 + 0x84, 1U, 0);
        }
        var_r0 = 0x5000;
        if ((s32)arg0->unk28 < (s32)temp_r5->unk28) {
            var_r0 = -0x5000;
        }
        arg0->unk40 = var_r0;
        arg0->unk44 = 0;
        arg0->unk38 = 0;
    }
    if ((arg0->unk30 == 0) && (arg0->unk38 == 0)) {
        arg0->unk294 = 0;
        return;
    }
    arg0->unk292 = (s16)(arg0->unk292 + 1);
}

/* Task stage 0: initialize boss core actor. */
s32 func_ov016_02128d48(void* arg0, Ov016Task* arg1, void* arg2) {
    const Ov016U16Quad* temp_lr;
    Ov016Obj*           temp_r4;
    Ov016BossSpawnArgs* args;

    (void)arg0;
    args    = (Ov016BossSpawnArgs*)arg2;
    temp_r4 = arg1->unk18;
    MI_CpuSet(temp_r4, 0, 0x298);
    temp_lr         = args->unkC;
    temp_r4->unk284 = (u16)temp_lr->unk0;
    temp_r4->unk286 = (u16)temp_lr->unk2;
    temp_r4->unk288 = (u16)temp_lr->unk4;
    temp_r4->unk28A = (u16)temp_lr->unk6;
    func_ov003_02086404(1, (u8*)temp_r4 + 0x84, args->unk2);
    func_ov003_020864dc(temp_r4, args->unk2);
    temp_r4->unk1C2 = (u16)args->unk2;
    if (RNG_Next(2) != 0) {
        func_ov003_02082750((u8*)temp_r4 + 0x84, 1);
    }
    func_ov003_020c4988(temp_r4);
    temp_r4->unk294 = 1;
    func_ov016_02128be0(temp_r4, func_ov016_02128bf8);
    return 1;
}

/* Task stage 1: update boss core actor and sub-state. */
s32 func_ov016_02128df8(void* arg0, Ov016Task* arg1, void* arg2) {
    void (*temp_r1)(Ov016Obj*);
    s32       temp_r0;
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (data_ov003_020e71b8->unk3D878 & 2) {
        return 1;
    }
    temp_r0 = func_ov003_02082f2c(temp_r4);
    if ((temp_r0 != 1) && (temp_r0 != 3)) {

    } else {
        func_ov016_02128be0(temp_r4, func_ov016_02128bfc);
    }
    temp_r1 = temp_r4->unk28C;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov003_02083074(temp_r4);
    func_ov003_02082b0c((u8*)temp_r4 + 0x84);
    return temp_r4->unk294;
}

/* Task stage 2: sync boss core to global draw manager. */
s32 func_ov016_02128e70(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    if (func_ov003_0208656c() == 0) {
        return 1;
    }
    func_ov003_020831e4((s32)temp_r4, (s32)((u8*)temp_r4 + 0x84));
    return 1;
}

/* Task stage 3: cleanup boss core actor. */
s32 func_ov016_02128e9c(void* arg0, Ov016Task* arg1, void* arg2) {
    Ov016Obj* temp_r4;

    (void)arg0;
    (void)arg2;
    temp_r4 = arg1->unk18;
    func_ov003_02082cc4((u8*)temp_r4 + 0x84);
    func_ov003_020c4a34(temp_r4);
    if (data_ov003_020e71b8->unk3D8A0 == temp_r4) {
        data_ov003_020e71b8->unk3D8A0 = 0;
    }
    return 1;
}

/* Task dispatcher for boss core actor. */
s32 func_ov016_02128edc(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov016Task* task_data = (Ov016Task*)task->data;
    return data_ov016_02129148[stage](pool, task_data, args);
}

/* Spawn a boss-side task and return its task id. */
s32 func_ov016_02128f24(u16 arg0, void* arg1) {
    void* sp14;
    u16   spA;
    s16   sp8;

    sp14 = arg1;
    spA  = arg0;
    return EasyTask_CreateTask(OV003_TASK_POOL, &data_ov016_0212913c, 0, 0, 0, (void*)&sp8);
}
