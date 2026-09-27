#include "Combat/Noise/Private/BtlEnm006.h"
#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"

#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

// MARK: Data

// The variant records in .rodata reference these tables and the asset-name strings in
// .data, so both sides are forward-declared here.
// Task entry points referenced by the TaskHandle records below.
extern s32 func_ov010_02125730(TaskPool*, Task*, void*, s32);
extern s32 func_ov010_021269d0(TaskPool*, Task*, void*, s32);
extern s32 func_ov010_02126d04(TaskPool*, Task*, void*, s32);
extern s32 func_ov010_02127178(TaskPool*, Task*, void*, s32);
extern s32 func_ov010_02128dbc(TaskPool*, Task*, void*, s32);

// Per-stage workers dispatched by the task entry points above. Declared void* for the
// task data because the owning structs are not mapped yet.
extern s32  func_ov010_02125780(void*, void*);
extern void func_ov010_02126a20(BtlEnm006*, void*);
extern s32  func_ov010_02126a94(BtlEnm006*);
extern void func_ov010_02126d54(Enm006Swirl*, Enm006Spawn*);
extern s32  func_ov010_02126e58(Enm006Swirl*);
extern s32  func_ov010_02126fdc(Enm006Swirl*);
extern s32  func_ov010_021271c0(Enm006Swlo*, Enm006Spawn*);
extern s32  func_ov010_021272e0(Enm006Swlo*);
extern s32  func_ov010_02128e80(void*);

// ov003 helpers.
extern void                  func_ov003_020c48b0(void*);
extern void                  func_ov003_020c4878(void*);
extern void                  func_ov003_020c48fc(void*);
extern void                  func_ov003_020c492c(void*);
extern s32                   func_ov003_020c703c(void*);
extern void                  func_ov003_020c427c(void*);
extern void                  func_ov003_020cba54(s32, s32, s32, s32, s32, s32);
extern s32                   func_ov003_020cb3c4(s32, s32);
extern s32                   func_ov003_020c5bfc(void*);
extern s32                   func_ov003_020c62c4(void*, s32);
extern s32                   func_ov003_020c65cc(void*, s32);
extern s32                   func_ov003_020c72b4(void*, s32, s32);
extern s32                   func_ov003_020c37f8(void*);
extern s32                   func_ov003_020c7070(void*);
extern s32                   func_ov003_020c4e0c(void*);
extern s32                   func_ov003_020cc354(void*);
extern s32                   func_ov003_020cc38c(void*, s32, s32, s32, s32, s32, s32);
extern s32                   func_ov003_020c3c28(void*);
extern s32                   func_ov003_020c3efc(void*, void*);
extern void                  func_ov003_020c4520(void*);
extern void                  func_ov003_020c4b5c(void*);
extern void                  func_ov003_020c4668(void*);
extern void                  func_ov003_02084348(s32, void*, void*, s32, s32, s32);
extern void                  func_ov003_02082724(void*, s16, s16);
extern void                  func_ov003_02082b64(void*);
extern void                  func_ov003_02084694(void*, s32);
extern s32                   func_ov003_020cb910(void*, void*, s32, s32, s32, s32, s32, s32, s32, void*);
extern void                  func_ov003_020cb498(s32, s32, void*, void*);
extern s32                   func_ov003_020843b0(s32, s32);
extern void                  Mini108_VBlank(CombatSprite*, u16, s32);
extern s32                   func_ov010_02128820(BtlEnm006*);
extern void                  CombatSprite_SetPaletteSource(CombatSprite*, s32);
extern void                  func_ov003_020c4ab4(BtlEnm006*, s32);
extern const SpriteAnimEntry data_ov010_02129238[3];
extern const SpriteAnimEntry data_ov010_021292f4[3];

extern s32   func_ov010_02128bcc(void*, void*);
extern void  func_ov010_02127110(void*, void*);
extern void  func_ov010_02127650(void);
extern void  func_ov010_02127cc0(void);
extern void  func_ov010_0212847c(s32*, s32*, void*, s32);
extern s32   func_ov003_020cba2c(s32, s32, s32, s32);
extern s32   func_ov003_020cb764(s32);
extern void* func_ov003_020c3c88(void);
extern s32   func_ov003_020c42ec(BtlEnm006*);
extern s32   func_ov003_020c4348(BtlEnm006*);
extern s32   func_ov010_02128a08(BtlEnm006*, s32);
extern void  func_ov010_02127488(BtlEnm006*);

/// The animation tables are addressed as `table + variant*8 + phase*2`, i.e. an array of 8-byte
/// records each holding four halfwords -- not as `SpriteAnimEntry[]`, whose field access folds the
/// two index terms together and produces a different instruction order.
typedef struct Enm006AnimRow {
    u16 slot[4];
} Enm006AnimRow;

// Per-instance callbacks passed to the init helpers.
extern void func_ov010_021259e8(BtlEnm006*);
extern void func_ov010_02127764(void);
extern void func_ov010_02125c80(void);
extern void func_ov010_02125b28(BtlEnm006*);
extern void func_ov010_02125938(BtlEnm006*);
extern s32  func_ov010_021265ac(BtlEnm006*, s32);
extern s32  func_ov003_020cb7a4(s32);
extern void func_ov003_020c44ac(BtlEnm006*);
extern void func_ov003_020c4b1c(BtlEnm006*);
extern void func_ov003_020c4628(BtlEnm006*);
extern s32  func_ov003_020c3bf0(void);
extern s32  func_ov003_02082f2c(BtlEnm006*);
extern s32  func_ov010_02126420(void*);
extern void func_ov010_0212636c(BtlEnm006*);
extern void func_ov010_021263c4(BtlEnm006*);
extern void func_ov010_02125998(BtlEnm006*);
extern void func_ov010_02127550(BtlEnm006*);
extern void func_ov010_02125de4(void);

extern char data_ov010_0212932c[28];
extern char data_ov010_02129348[28];
extern char data_ov010_02129364[24];
extern char data_ov010_0212938c[24];
extern char data_ov010_021293a4[20];
extern char data_ov010_021293b8[20];
extern char data_ov010_021293cc[20];
extern char data_ov010_021293e0[32];

extern const BinIdentifier   data_ov010_02129050;
extern const BinIdentifier   data_ov010_02129058;
extern const BinIdentifier   data_ov010_02129060;
extern const SpriteAnimEntry data_ov010_02129068[17];
extern const SpriteAnimEntry data_ov010_021290f0[14];
extern const SpriteAnimEntry data_ov010_02129178[17];

extern const Enm006Variant   data_ov010_02129008;
extern const Enm006Variant   data_ov010_02129018;
extern const SpriteAnimEntry data_ov010_02129206[1];
extern const SpriteAnimEntry data_ov010_0212920e[1];
extern const SpriteAnimEntry data_ov010_02129216[1];

// .rodata, in the original's layout order.
//
// Two granularity caveats, both worth knowing before trusting the .rodata score:
//  - symbols.txt has no entry at 0x02129038, so the auto-tagger folded the 24-byte
//    anim triple there into data_ov010_02129028 and gave that symbol a 40-byte extent.
//    It is declared here as the 16-byte variant it actually is, plus a separate
//    24-byte array at 0x02129038, so objdiff sees a size disagreement on the former.
//  - 0x02129216 is followed by two bytes of padding that the tagger folded into its
//    extent, and the 0x0212925c region has no interior symbols at all.

// symbols.txt has no entry at 0x02129038, so the auto-tagger folded the 24-byte anim triple
// that follows this variant into it and gave the symbol a 40-byte extent. Reproducing that
// granularity here -- one 0x28 object rather than a 16-byte variant plus a separate array --
// is what objdiff compares against.
typedef struct Enm006VariantTail {
    /* 0x00 */ Enm006Variant   variant;
    /* 0x10 */ SpriteAnimEntry anim[3];
} Enm006VariantTail; // Size: 0x28

const Enm006Variant data_ov010_02129008 = {&data_ov010_02129060, data_ov010_021290f0, 0, 0x13, 0x160};
const Enm006Variant data_ov010_02129018 = {&data_ov010_02129058, data_ov010_02129068, 0, 0x13, 0x150};

const Enm006VariantTail data_ov010_02129028 = {
    {&data_ov010_02129050, data_ov010_02129178, 0, 0x13, 0x150},
    {
     {19, 20, 21, 22},
     {19, 20, 21, 22},
     {19, 20, 21, 22},
     },
};

const BinIdentifier data_ov010_02129050 = {3, data_ov010_0212932c};
const BinIdentifier data_ov010_02129058 = {3, data_ov010_02129348};
const BinIdentifier data_ov010_02129060 = {3, data_ov010_02129364};

const SpriteAnimEntry data_ov010_02129068[17] = {
    { 1,  3,  2, 0},
    { 1,  3,  2, 1},
    { 1,  3,  2, 2},
    { 4,  6,  5, 0},
    { 4,  6,  5, 1},
    { 4,  6,  5, 2},
    { 7,  9,  8, 0},
    { 7,  9,  8, 1},
    { 7,  9,  8, 2},
    { 7,  9,  8, 3},
    { 7,  9,  8, 3},
    { 7,  9,  8, 4},
    {10, 12, 11, 0},
    {16, 18, 17, 2},
    {13, 15, 14, 0},
    {16, 18, 17, 3},
    {13, 15, 14, 1},
};

const SpriteAnimEntry data_ov010_021290f0[14] = {
    { 1,  3,  2, 0},
    { 1,  3,  2, 1},
    { 1,  3,  2, 2},
    { 4,  6,  5, 0},
    { 4,  6,  5, 1},
    { 4,  6,  5, 2},
    { 7,  9,  8, 0},
    { 7,  9,  8, 1},
    { 7,  9,  8, 2},
    { 7,  9,  8, 3},
    { 7,  9,  8, 3},
    { 7,  9,  8, 4},
    {10, 12, 11, 0},
    {16, 18, 17, 2},
};

const SpriteAnimEntry data_ov010_02129160[3] = {
    {13, 15, 14, 0},
    {16, 18, 17, 3},
    {13, 15, 14, 1},
};

const SpriteAnimEntry data_ov010_02129178[17] = {
    { 1,  3,  2, 0},
    { 1,  3,  2, 1},
    { 1,  3,  2, 2},
    { 4,  6,  5, 0},
    { 4,  6,  5, 1},
    { 4,  6,  5, 2},
    { 7,  9,  8, 0},
    { 7,  9,  8, 1},
    { 7,  9,  8, 2},
    { 7,  9,  8, 3},
    { 7,  9,  8, 3},
    { 7,  9,  8, 4},
    {10, 12, 11, 0},
    {16, 18, 17, 2},
    {13, 15, 14, 0},
    { 1,  3,  2, 0},
    { 1,  3,  2, 0},
};

const s16 data_ov010_02129200[3] = {0x17, 0x17, 0x17};

const SpriteAnimEntry data_ov010_02129206[1] = {
    {16, 18, 17, 0},
};

const SpriteAnimEntry data_ov010_0212920e[1] = {
    {16, 18, 17, 0},
};

const SpriteAnimEntry data_ov010_02129216[1] = {
    {16, 18, 17, 0},
};

const TaskHandle Tsk_BtlEnm006_DeadEff = {data_ov010_0212938c, func_ov010_02125730, 0x6C};
const TaskHandle Tsk_BtlEnm006_RG      = {data_ov010_021293a4, func_ov010_021269d0, 0x1FC};

const SpriteAnimEntry data_ov010_02129238[3] = {
    {19, 20, 21, 22},
    {19, 20, 21, 22},
    {19, 20, 21, 22},
};

const s16 data_ov010_02129250[2] = {0, 6};
const s32 data_ov010_02129254    = 0;
const s32 data_ov010_02129258    = 2;

// 42 s16, one object. The true type is not identified yet; the bytes look like a table of
// small signed (x, y) fixed-point offset pairs. Declared s16[] because objdiff compares the
// byte image rather than the type.
const s16 data_ov010_0212925c[42] = {
    1, 0, 0, 4, -32768, -2, 2,  0,      0, 0, 0, -4, -32768, -2, 0, 0,      0, 0, 0, -6, 0,
    0, 1, 0, 0, 0,      0,  -4, -32768, 1, 1, 0, 1,  0,      0,  4, -32768, 1, 0, 0, 1,  0,
};

const s16 data_ov010_021292b0[4] = {0x17, 0x17, 0x17, 0};

const TaskHandle Tsk_BtlEnm006_Swirl = {data_ov010_021293b8, func_ov010_02126d04, 0xA0};

const SpriteAnimEntry data_ov010_021292c4[3] = {
    {16, 18, 17, 4},
    {16, 18, 17, 4},
    {16, 18, 17, 4},
};

const TaskHandle Tsk_BtlEnm006_Swlo = {data_ov010_021293cc, func_ov010_02127178, 0x34};
const TaskHandle Tsk_BtlEnm006_UG   = {data_ov010_021293e0, func_ov010_02128dbc, 0x200};

const SpriteAnimEntry data_ov010_021292f4[3] = {
    {19, 20, 21, 22},
    {19, 20, 21, 22},
    {19, 20, 21, 22},
};

// .data, in the original's layout order.
//
// "Apl_Suy/Grp_BtlEnm006b.bin" is split 24 + 4 at 0x02129364 / 0x0212937c in the
// original (the 4-byte half is flagged `ambiguous` in symbols.txt). The BinIdentifier
// still points at 0x02129364, so the runtime asset path is unchanged.
Enm006Variant* data_ov010_02129320[3] = {
    &data_ov010_02129028.variant,
    &data_ov010_02129018,
    &data_ov010_02129008,
};

char data_ov010_0212932c[28] = "Apl_Suy/Grp_BtlEnm006.bin";
char data_ov010_02129348[28] = "Apl_Suy/Grp_BtlEnm006a.bin";
// The original splits this literal mid-string at 0x02129364 / 0x0212937c: 24 bytes holding
// "Apl_Suy/Grp_BtlEnm006b.b" with no terminator, then "in\0\0". Needs a brace initializer
// because a 24-character literal does not fit a char[24]. The BinIdentifier still points at
// 0x02129364, so the runtime asset path is unchanged.
char data_ov010_02129364[24] = {'A', 'p', 'l', '_', 'S', 'u', 'y', '/', 'G', 'r', 'p', '_',
                                'B', 't', 'l', 'E', 'n', 'm', '0', '0', '6', 'b', '.', 'b'};
char data_ov010_0212937c[4]  = "in";

SpriteAnimEntry* data_ov010_02129380[3] = {
    &data_ov010_02129216[0],
    &data_ov010_0212920e[0],
    &data_ov010_02129206[0],
};

char data_ov010_0212938c[24] = "Tsk_BtlEnm006_DeadEff";
char data_ov010_021293a4[20] = "Tsk_BtlEnm006_RG";
char data_ov010_021293b8[20] = "Tsk_BtlEnm006_Swirl";
char data_ov010_021293cc[20] = "Tsk_BtlEnm006_Swlo";
char data_ov010_021293e0[32] = "Tsk_BtlEnm006_UG";

// MARK: Functions

BinIdentifier* func_ov010_021256c0(s32 index) {
    return (BinIdentifier*)((u8*)&data_ov010_02129050 + index * 8);
}

s32 func_ov010_02127458(void) {
    return 1;
}

// These are single-argument callbacks, not task handlers: none of them appears in a
// TaskHandle, and each passes its incoming r0 straight through with no register shuffle.
// Typed CombatSprite* where the callee is known to be one of the CombatSprite helpers.
s32 func_ov010_02125900(CombatSprite* sprite) {
    CombatSprite_Release(sprite);
    return 1;
}

s32 func_ov010_02126c18(CombatSprite* sprite) {
    func_ov003_020c4878(sprite);
    return 1;
}

s32 func_ov010_02126c28(CombatSprite* sprite) {
    func_ov003_020c48fc(sprite);
    return 1;
}

s32 func_ov010_02128fe8(CombatSprite* sprite) {
    func_ov003_020c48b0(sprite);
    return 1;
}

s32 func_ov010_02128ff8(CombatSprite* sprite) {
    func_ov003_020c492c(sprite);
    return 1;
}

// Releases the block at sprite + 4 rather than sprite itself.
s32 func_ov010_02127094(CombatSprite* sprite) {
    CombatSprite_Release((CombatSprite*)((u8*)sprite + 4));
    return 1;
}

s32 func_ov010_02125854(CombatSprite* sprite) {
    CombatSprite_Update(sprite);
    return SpriteMgr_IsAnimationFinished(&sprite->sprite) == 0;
}

s32 func_ov010_02126420(void* data) {
    s32 result = func_ov003_020c703c(data);
    if (result == 0) {
        // The BtlEnm006 layout is not mapped yet, so this field is reached by offset.
        *(s32*)((u8*)data + 0x1CC) = 0;
    }
    return result;
}

// MARK: Task entry points
//
// All five are the same shape: read task->data, dispatch on stage, and return whatever the
// per-stage worker returns (1 for an unhandled stage). The sub-workers are declared void*
// because their owning structs are not mapped yet -- their sizes come from the TaskHandle
// records: DeadEff 0x6C, RG 0x1FC, Swirl 0xA0, Swlo 0x34, UG 0x200.

// Tsk_BtlEnm006_DeadEff -- dense 4-case table.
s32 func_ov010_02125730(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            return func_ov010_02125780(data, args);
        case 1:
            return func_ov010_02125854(data);
        case 2:
            return func_ov010_02125878(data);
        case 3:
            return func_ov010_02125900(data);
    }
    return 1;
}

// Tsk_BtlEnm006_RG -- dense 4-case table.
s32 func_ov010_021269d0(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            func_ov010_02126a20(data, args);
            return 1;
        case 1:
            return func_ov010_02126a94(data);
        case 2:
            return func_ov010_02126c18(data);
        case 3:
            return func_ov010_02126c28(data);
    }
    return 1;
}

// Tsk_BtlEnm006_Swirl -- dense 4-case table.
s32 func_ov010_02126d04(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            func_ov010_02126d54((Enm006Swirl*)data, args);
            return 1;
        case 1:
            return func_ov010_02126e58(data);
        case 2:
            return func_ov010_02126fdc(data);
        case 3:
            return func_ov010_02127094(data);
    }
    return 1;
}

// Tsk_BtlEnm006_Swlo -- sparse chain; stages 0, 1 and 3 only, so no jump table.
s32 func_ov010_02127178(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            return func_ov010_021271c0(data, args);
        case 1:
            return func_ov010_021272e0((Enm006Swlo*)data);
        case 3:
            return func_ov010_02127458();
    }
    return 1;
}

// Tsk_BtlEnm006_UG -- dense 4-case table.
s32 func_ov010_02128dbc(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            return func_ov010_02128e0c(data, args);
        case 1:
            return func_ov010_02128e80(data);
        case 2:
            return func_ov010_02128fe8(data);
        case 3:
            return func_ov010_02128ff8(data);
    }
    return 1;
}

// func_ov010_02125910 and func_ov010_02127460 are byte-identical bodies. Both compute
// data + 0x100 once, *after* the call, and index the two halfword clears from that base --
// so the stores must be plain field accesses, not a pointer local, or the address gets
// hoisted above the call and an extra callee-saved register appears.
void func_ov010_02125910(BtlEnm006* data, void* arg1) {
    func_ov003_020c427c(data);
    data->unk_1C8       = arg1;
    data->sprite.unk_C4 = 0;
    data->sprite.unk_C0 = 0;
}

void func_ov010_02127460(BtlEnm006* data, void* arg1) {
    func_ov003_020c427c(data);
    data->unk_1C8       = arg1;
    data->sprite.unk_C4 = 0;
    data->sprite.unk_C0 = 0;
}

// The original branches all three tests forward to one shared `return 0`, then reaches
// `return 1` by a forward branch of its own. So both arms need explicit labels: early
// returns get predicated by MWCC (`movlt r0, #1; bxlt lr`) instead of branching.
s32 func_ov010_02128c3c(void* arg0, void* arg1) {
    s32 v = *(s32*)((u8*)arg1 + 0x7C);
    if (v < -1) {
        goto zero;
    }
    if (v == 5) {
        goto zero;
    }
    if (v < 0x3C) {
        goto one;
    }
zero:
    return 0;
one:
    return 1;
}

// func_ov010_0212688c and func_ov010_02128ac8 are byte-identical: a six-argument call where
// the two stack arguments are evaluated first.
void func_ov010_0212688c(BtlEnm006* data) {
    func_ov003_020cba54(data->unk_28, data->unk_2C, data->unk_30, data->unk_1D0, data->unk_1D4, data->unk_1D8);
}

void func_ov010_02128ac8(BtlEnm006* data) {
    func_ov003_020cba54(data->unk_28, data->unk_2C, data->unk_30, data->unk_1D0, data->unk_1D4, data->unk_1D8);
}

s32 func_ov010_02128b00(void) {
    s32 lo = func_ov003_020cb3c4(0, 5);
    s32 hi = func_ov003_020cb3c4(0, 0x3C);
    s32 t  = ((hi - lo) << 11) + 0x1000;
    return ((u32)FX_Divide(t * 0x64, t + 0x3000) << 4) >> 16;
}

// A family of five share this shape: bump sprite.unk_C0 if it is still zero, do a
// one-time setup, then bail out unless a gate returns zero -- in which case a per-instance
// callback is handed to one of the two init helpers. The gate and the callback differ.
void func_ov010_02125998(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        data->sprite.unk_C0++;
        data->sprite.unk_88 = data->sprite.twin->sprite.unk_88;
    }
    if (func_ov003_020c5bfc(data) != 0) {
        return;
    }
    func_ov010_02125910(data, (void*)func_ov010_021259e8);
}

void func_ov010_02127500(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        // The two-step data+0xF6 then +0x100 is load-bearing: folded to a single add, MWCC
        // reassociates it to (data+0x100)+0x100 and the stored pointer is 0xA too high.
        u8* p = (u8*)data + 0xF6;
        data->sprite.unk_C0++;
        data->sprite.unk_88 = p + 0x100;
    }
    if (func_ov003_020c5bfc(data) != 0) {
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127764);
}

void func_ov010_0212636c(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        data->sprite.unk_C0++;
        data->unk_1E4 = 0;
        data->unk_1E0 = 0;
        data->unk_1DC = 0;
    }
    if (func_ov003_020c62c4(data, 6) != 0) {
        return;
    }
    func_ov010_02125910(data, (void*)func_ov010_021259e8);
}

void func_ov010_021263c4(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        data->sprite.unk_C0++;
        data->unk_1E4 = 0;
        data->unk_1E0 = 0;
        data->unk_1DC = 0;
    }
    if (func_ov003_020c72b4(data, 0, 9) != 0) {
        return;
    }
    func_ov010_02125910(data, (void*)func_ov010_021259e8);
}

void func_ov010_021282b8(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        data->sprite.unk_C0++;
        data->unk_1E4 = 0;
        data->unk_1E0 = 0;
        data->unk_1DC = 0;
        data->unk_38  = 0;
    }
    if (func_ov003_020c65cc(data, 6) != 0) {
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127550);
}

void func_ov010_02128434(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        data->sprite.unk_C0++;
        data->unk_1E4 = 0;
        data->unk_1E0 = 0;
        data->unk_1DC = 0;
    }
    if (func_ov003_020c7070(data) == 0) {
        data->unk_1CC = 0;
    }
}

// func_ov010_02126830 and func_ov010_02128a6c are byte-identical. A ten-argument call: the six
// stack arguments are evaluated first, and the first two register arguments are the *addresses*
// of two of the fields that also appear as later arguments.
s16 func_ov010_02126830(BtlEnm006* data, void* arg1) {
    return func_ov003_020cb910(&data->unk_1DC, &data->unk_1E0, &data->unk_1E4, data->unk_28, data->unk_2C, data->unk_30,
                               data->unk_1D0, data->unk_1D4, data->unk_1D8, arg1);
}

s16 func_ov010_02128a6c(BtlEnm006* data, void* arg1) {
    return func_ov003_020cb910(&data->unk_1DC, &data->unk_1E0, &data->unk_1E4, data->unk_28, data->unk_2C, data->unk_30,
                               data->unk_1D0, data->unk_1D4, data->unk_1D8, arg1);
}

// Spawns the sub-task. The pool handle is either whatever the lookup at data+0x84 returns, or --
// when that returns zero -- a pair of globals offset by a fixed bias.
void func_ov010_02126c38(void* arg0) {
    // `&self` is what the original hands over as the task's param: it takes the address of the
    // incoming argument, which MWCC homes in the outgoing-argument area at sp+8. Taking the
    // address of the parameter directly makes it spill r0-r3 instead.
    void*     self = arg0;
    TaskPool* pool = (TaskPool*)func_ov003_020c37f8((u8*)self + 0x84);
    if (pool == NULL) {
        pool = (TaskPool*)data_ov003_020e71b8;
        if (pool != NULL) {
            pool = (TaskPool*)((u32)pool + 0x8C + 0x8000);
        }
    }
    EasyTask_CreateTask(pool, &Tsk_BtlEnm006_Swirl, 0, 0, 0, &self);
}

// All three tests reach one shared `return 0` block, but by different routes: the first two branch
// *forward* to it, while the third falls through into it and the body is reached by a forward
// branch. Written as plain early returns MWCC predicates them into the fall-through instead
// (`movlt r0, #0 / bxlt lr`), so the arms need explicit labels. The final decrement also re-reads
// the field rather than reusing the value tested a few lines earlier.
s32 func_ov010_02128c6c(s32 arg0, s32 arg1, void* arg2) {
    s32 v = *(s32*)((u8*)arg1 + 0x7C);
    if (v < -1) {
        goto zero;
    }
    if (v == 5) {
        goto zero;
    }
    if (v < 0x3C) {
        goto body;
    }
zero:
    return 0;
body:
    s32 c = *(s32*)arg2;
    if (c < 0) {
        return 0;
    }
    if (c == 0) {
        *(s32*)((u8*)arg2 + 4) = arg1;
    }
    *(s32*)arg2 = *(s32*)arg2 - 1;
    return 0;
}

// Picks the next behaviour by a coin flip. The "dead" case has to be the *outer* else so the
// f()==0 branch jumps forward past both random arms -- which is also what fixes the order of
// the three function-pointer literals in the pool.
void func_ov010_02125938(BtlEnm006* data) {
    if (func_ov003_020c4e0c(data) != 0) {
        if (RNG_Next(0x64) < 0x4B) {
            func_ov010_02125910(data, (void*)func_ov010_02125c80);
        } else {
            func_ov010_02125910(data, (void*)func_ov010_02125de4);
        }
    } else {
        func_ov010_02125910(data, (void*)func_ov010_021259e8);
    }
}

// The horizontal mirror is applied by biasing position.x by +/-0x8000 rather than by a flag, so
// the bias has to be materialised before the three stack arguments are pushed.
s32 func_ov010_021268c4(void* arg0, void* arg1) {
    BtlEnm006* data = arg0;
    if (func_ov003_020cc354(arg1) != 0) {
        return 0;
    }
    s32 flip = data->unk_24 == 0 ? 0x8000 : -0x8000;
    return func_ov003_020cc38c(arg1, data->unk_28 + flip, data->unk_2C + 0x8000, data->unk_30, 0x10000, 0x10000, 0x20000);
}

// Same shape as func_ov010_02126c38 -- spawn a sub-task with the current actor as its param --
// but the frame is 0x10 rather than 0xC and the handle is the DeadEff variant.
void func_ov010_021256d0(void* arg0, void* arg1) {
    void*     self = arg0;
    TaskPool* pool = (TaskPool*)func_ov003_020c37f8((u8*)arg1 + 0x84);
    if (pool == NULL) {
        pool = (TaskPool*)data_ov003_020e71b8;
        if (pool != NULL) {
            pool = (TaskPool*)((u32)pool + 0x8C + 0x8000);
        }
    }
    EasyTask_CreateTask(pool, &Tsk_BtlEnm006_DeadEff, 0, 0, 0, &self);
}

// Two more members of the init family. The `unk_1F5` bit-0 test has to be spelled as a shift
// pair, not `& 1`: `if (x & 1)` gives `tst r0, #1 / ldrne / bicne`, while the original wants
// the bit moved into a value and *then* predicated on it being zero
// (`lsl #30 / lsr #31 / ldreq / biceq / streq`). The `(u32)` cast matters -- the compiler is
// built with `-char signed`, so without it the final shift is `asr` rather than `lsr`.
void func_ov010_02128314(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        data->sprite.unk_C0++;
        data->unk_1E4 = 0;
        data->unk_1E0 = 0;
        data->unk_1DC = 0;
        data->unk_38  = 0;
        // 4.12 fixed point: (x * 10.0) rounded to nearest, via a 64-bit intermediate.
        data->unk_40 = (s32)((((s64)data->unk_40 * 0x2800) + 0x800) >> 12);
        if (((u32)data->unk_1F5 << 30) >> 31 == 0) {
            data->unk_54 &= ~0x40000000;
        }
        data->unk_54 &= ~0x4000;
    }
    if (func_ov003_020c65cc(data, 9) != 0) {
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127550);
}

void func_ov010_021283c0(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        data->sprite.unk_C0++;
        data->unk_1E4 = 0;
        data->unk_1E0 = 0;
        data->unk_1DC = 0;
        if (((u32)data->unk_1F5 << 30) >> 31 == 0) {
            data->unk_54 &= ~0x40000000;
        }
    }
    if (func_ov003_020c72b4(data, 0, 9) != 0) {
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127550);
}

// The sprite block is 0x200 bytes here, not 0xC8 -- the clear covers the whole thing, and the
// 0x1E8 halfword is reached through the overlapping `Enm006SpriteAlt` view so the address is
// built as base+0x100 then +0xE8 rather than as one outer-struct access.
s32 func_ov010_02128e0c(BtlEnm006* data, void* arg1) {
    MI_CpuSet(data, 0, 0x200);
    func_ov003_020c3efc(data, arg1);
    func_ov003_020c4520(data);
    func_ov003_020c4b5c(data);
    // 0x144 is inside the sprite block's span, so it cannot be an outer-struct field; the offset
    // cast is also what keeps the address as a single `add r0, r4, #0x144`.
    func_ov003_02084694((u8*)data + 0x144, 1);
    func_ov010_02127460(data, (void*)func_ov010_02127500);
    data->unk_1CC                             = 1;
    data->unk_1DC                             = 0;
    data->unk_1E0                             = 0;
    data->unk_1E4                             = 0;
    ((Enm006SpriteAlt*)&data->sprite)->unk_E8 = 0;
    return 1;
}

// Fixed-point velocity integration, as in the sibling per-frame workers: ask the trig helper for
// a direction from (unk_60, unk_64, unk_68), hand the sprite the result, then set a negated angle.
s32 func_ov010_02125878(BtlEnm006* data) {
    if (func_ov003_020c3c28(data) != 0) {
        return 0;
    }
    // Not initialised: the helper always writes both out-params, and zeroing them first would
    // add two `strh` the original does not have.
    s16 dy;
    s16 dx;
    s32 flag = func_ov003_020c37f8(data) != 0;
    func_ov003_02084348(flag, &dy, &dx, data->unk_60, data->unk_64, data->unk_68);
    CombatSprite_SetPosition((CombatSprite*)data, dx, dy);
    func_ov003_02082730((CombatSprite*)data, 0x80000000 - data->unk_64);
    CombatSprite_Render((CombatSprite*)data);
    return 1;
}

// Three-phase death animation, driven by sprite.unk_C4. Each phase re-enters its one-time setup
// only while unk_C0 is still zero, and unk_C0 is the frame counter within the phase.
void func_ov010_02125b28(BtlEnm006* data) {
    CombatSprite* cs = (CombatSprite*)((u8*)data + 0x84);
    switch (data->sprite.unk_C4) {
        case 0:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0++;
                Mini108_VBlank(cs, 3, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 1;
            return;
        case 1:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_8C |= 1;
                data->unk_54 |= 0x20;
                data->sprite.unk_8C |= 1;
                func_ov003_020cb578(0, 0);
                data->sprite.unk_C2 = 1;
            }
            if (data->sprite.unk_C0 < data->sprite.unk_C2) {
                data->sprite.unk_C0++;
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 2;
            return;
        case 2:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0++;
                data->sprite.unk_8C &= ~1;
                data->unk_54 &= ~0x20;
                Mini108_VBlank(cs, 4, 1);
                func_ov003_020cb578(data, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            func_ov010_02125910(data, (void*)func_ov010_021259e8);
            return;
    }
}

// Fires a one-off effect every fourth frame, and only while a flag sprite field says so.
s32 func_ov010_02128a08(BtlEnm006* data, s32 arg1) {
    // The address has to be computed *inside* the branch: hoisting it to a local turns the
    // original's `addeq r0, r4, #0x100` into an unconditional `add r1, r4, #0x100`.
    if (arg1 == 0) {
        ((Enm006SpriteAlt2*)&data->sprite)->unk_F0 = 0;
    }
    func_ov010_02128820(data);
    if ((data->unk_9A - 1) % 4 == 0) {
        if (data->unk_8C == 1) {
            // The second parameter of func_ov003_02087f00 is declared as a callback pointer, but
            // the original really calls 020843b0 here and passes the result.
            func_ov003_02087f00(0x1CE, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_28));
        }
    }
    return 1;
}

// Two near-twins: both advance a 4-entry animation phase and, on every phase but the last,
// restart the sprite animation and rescale the phase into 4.12 fixed point for the speed field.
// The divide is by three, spelled with the 0x55555556 reciprocal.
//
// The sprite view has to be re-derived at each use, not hoisted into a local: held in a variable
// MWCC gives it its own callee-saved register and the whole function spills (68.9% -> 100%).
// The animation table is read through a `u16*`, not through `SpriteAnimEntry`, because the
// original addresses it as `table + variant*8 + phase*2`.
void func_ov010_02126934(BtlEnm006* data, u16 arg1) {
    ((Enm006SpriteAlt*)&data->sprite)->unk_E8 += arg1;
    if (((Enm006SpriteAlt*)&data->sprite)->unk_E8 >= 4) {
        ((Enm006SpriteAlt*)&data->sprite)->unk_E8 = 3;
    } else {
        CombatSprite_SetPaletteSource(
            (CombatSprite*)((u8*)data + 0x84),
            ((const Enm006AnimRow*)data_ov010_02129238)[data->unk_80].slot[((Enm006SpriteAlt*)&data->sprite)->unk_E8]);
        data->unk_D4 = data->unk_C0;
        data->unk_D8 = data->unk_B4;
        data->unk_8  = (s16)((((s32)(((Enm006SpriteAlt*)&data->sprite)->unk_E8 << 12) / 3 + 0x1000) * 0x64) >> 12);
    }
    data->unk_54 |= 0x40000000;
}

void func_ov010_02128d20(BtlEnm006* data, u16 arg1) {
    ((Enm006SpriteAlt*)&data->sprite)->unk_E8 += arg1;
    if (((Enm006SpriteAlt*)&data->sprite)->unk_E8 >= 4) {
        ((Enm006SpriteAlt*)&data->sprite)->unk_E8 = 3;
    } else {
        CombatSprite_SetPaletteSource(
            (CombatSprite*)((u8*)data + 0x84),
            ((const Enm006AnimRow*)data_ov010_021292f4)[data->unk_80].slot[((Enm006SpriteAlt*)&data->sprite)->unk_E8]);
        data->unk_D4 = data->unk_C0;
        data->unk_D8 = data->unk_B4;
        data->unk_8  = (s16)((((s32)(((Enm006SpriteAlt*)&data->sprite)->unk_E8 << 12) / 3 + 0x1000) * 0x64) >> 12);
    }
    data->unk_54 |= 0x40000000;
}

// The third copy of the task-spawn shape, this time for the Swlo handle. Its frame is 0x18 and
// the param it hands over is a pointer to a local zero rather than to the incoming argument.
void func_ov010_021270a8(void* arg0, void* arg1) {
    s32       zero = 0;
    TaskPool* pool = (TaskPool*)func_ov003_020c37f8((u8*)arg0 + 0x84);
    if (pool == NULL) {
        pool = (TaskPool*)data_ov003_020e71b8;
        if (pool != NULL) {
            pool = (TaskPool*)((u32*)pool + 0x8C + 0x8000);
        }
    }
    EasyTask_CreateTask(pool, &Tsk_BtlEnm006_Swlo, 0, 0, 0, &zero);
}

// Twin of func_ov010_021268c4 -- same mirror-by-position-bias trick, different box size.
s32 func_ov010_02128bcc(void* arg0, void* arg1) {
    BtlEnm006* data = arg0;
    if (func_ov003_020cc354(arg1) != 0) {
        return 0;
    }
    s32 flip = data->unk_24 == 0 ? 0x8000 : -0x8000;
    return func_ov003_020cc38c(arg1, data->unk_28 + flip, data->unk_2C + 0x8000, data->unk_30, 0x18000, 0x20000, 0x20000);
}

// The gate before spawning a follower: the target must not be in its dying state, must not have
// bit 2 set in its engine flags, and must be inside the arena. The return value is the
// *negation* of the first argument, so a caller passing 1 gets 0.
s32 func_ov010_02128cbc(s32 arg0, void* arg1, void* arg2) {
    if (*(s32*)((u8*)arg1 + 0x7C) == 5) {
        return 0;
    }
    if (*(s32*)((u8*)arg1 + 0x54) & 4) {
        return 0;
    }
    if (func_ov010_02128bcc(arg2, arg1) == 0) {
        goto zero;
    }
    func_ov010_02127110(arg2, arg1);
    return arg0 == 0;
zero:
    // The first two early returns are predicated by MWCC and match; this one has to branch
    // forward to this shared block, which a plain `return 0` does not produce.
    return 0;
}

// The fourth copy of the task-spawn shape; the param handed over points at a local one.
void func_ov010_02127110(void* arg0, void* arg1) {
    s32       one  = 1;
    TaskPool* pool = (TaskPool*)func_ov003_020c37f8((u8*)arg0 + 0x84);
    if (pool == NULL) {
        pool = (TaskPool*)data_ov003_020e71b8;
        if (pool != NULL) {
            pool = (TaskPool*)((u32*)pool + 0x8C + 0x8000);
        }
    }
    EasyTask_CreateTask(pool, &Tsk_BtlEnm006_Swlo, 0, 0, 0, &one);
}

// Three-way behaviour pick. The `unk_80` test has to come first and *not* short-circuit the
// random draw: the original branches past the draw when unk_80 is zero. The rare arm also has to
// be the *last* block in the layout -- written inline, MWCC inverts the compare into `blo` and
// jumps over it, instead of the original's forward `bhs` to a block placed at the end.
void func_ov010_02127488(BtlEnm006* data) {
    if (data->unk_80 != 0) {
        if (RNG_Next(0x64) >= 0x5A) {
            goto big;
        }
    }
    if ((u32)RNG_Next(0x64) < (u32)func_ov010_02128b00()) {
        func_ov010_02127460(data, (void*)func_ov010_02127764);
    } else {
        func_ov010_02127460(data, (void*)func_ov010_02127650);
    }
    return;
big:
    func_ov010_02127460(data, (void*)func_ov010_02127cc0);
}

// Finds the index of the nearest of ten sample points. The x-axis cull is two-sided: the
// mirror flag decides which side of the owner counts as "behind".
s32 func_ov010_02128624(BtlEnm006* data) {
    s32 bestD = 0x7FFFFFFF;
    s32 bestI = -1;
    s32 y;
    s32 x;
    s32 i;
    for (i = 0; i < 10; i++) {
        func_ov010_0212847c(&y, &x, data, i);
        // Two sequential guards, not `else if`, and `flip` is read once: the original loads
        // unk_24 into a register after the call and tests that same register twice, so the
        // "unk_24 == 0 and in range" path falls straight through into the second test.
        s32 flip = data->unk_24;
        if (flip == 0) {
            if (y > data->unk_28) {
                continue;
            }
        }
        if (flip == 1) {
            if (y < data->unk_28) {
                continue;
            }
        }
        s32 d = func_ov003_020cba2c(data->unk_28, data->unk_2C, y, x);
        if (d < bestD) {
            bestD = d;
            bestI = i;
        }
    }
    if (bestI == -1) {
        if (data->unk_28 < func_ov003_020cb764(1)) {
            bestI = 0;
        } else {
            bestI = 5;
        }
    }
    return bestI;
}

// A near-twin of func_ov010_021259e8. On the first frame the sprite's two velocity components are
// scaled from the current 4-entry animation phase and a global manager's two s16 scale factors;
// after that it drives the frame counter and hands off to func_ov010_02127488.
void func_ov010_02127550(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        data->sprite.unk_C0++;
        u8* g               = (u8*)func_ov003_020c3c88();
        s16 kx              = *(s16*)(g + 0x1C);
        s16 ky              = *(s16*)(g + 0x20);
        s32 p               = ((s32)(((Enm006SpriteAlt*)&data->sprite)->unk_E8 << 11) / 3 + 0x1000) * kx;
        data->sprite.unk_98 = (s16)(p >> 12);
        p                   = ((s32)(((Enm006SpriteAlt*)&data->sprite)->unk_E8 << 11) / 3 + 0x1000) * ky;
        data->sprite.unk_9C = (s16)(p >> 12);
        data->sprite.unk_C2 = func_ov003_020c42ec(data);
    }
    func_ov010_02128a08(data, data->sprite.unk_C0);
    if (data->sprite.unk_C0 < data->sprite.unk_C2) {
        data->sprite.unk_C0++;
        return;
    }
    if (func_ov003_020c4348(data) != 0) {
        data->unk_1E4 = 0;
        data->unk_1E0 = 0;
        data->unk_1DC = 0;
        func_ov010_02127488(data);
        return;
    }
    data->sprite.unk_C0 = 1;
    data->sprite.unk_C2 = func_ov003_020c42ec(data);
}

// The twin of func_ov010_02127550, with a different manager struct (scale factors at 0x22/0x26
// rather than 0x1C/0x20) and different velocity slots. Both halves of the function are a chain
// of forward branches to one shared `unk_C0++` tail, so the guards have to be `goto`s.
void func_ov010_021259e8(BtlEnm006* data) {
    if (*(u16*)((u8*)data->sprite.unk_88 + 2) != 0) {
        data->unk_1E4 = 0;
        data->unk_1E0 = 0;
        data->unk_1DC = 0;
        func_ov010_02125910(data, (void*)func_ov010_02125b28);
        return;
    }
    if (data->sprite.unk_C0 == 0) {
        data->sprite.unk_C0++;
        u8* g               = (u8*)func_ov003_020c3c88();
        s16 kx              = *(s16*)(g + 0x22);
        s16 ky              = *(s16*)(g + 0x26);
        s32 p               = ((s32)(((Enm006SpriteAlt*)&data->sprite)->unk_E8 << 11) / 3 + 0x1000) * kx;
        data->sprite.unk_9E = (s16)(p >> 12);
        p                   = ((s32)(((Enm006SpriteAlt*)&data->sprite)->unk_E8 << 11) / 3 + 0x1000) * ky;
        data->sprite.unk_A2 = (s16)(p >> 12);
        data->sprite.unk_C2 = func_ov003_020c42ec(data);
    }
    if (func_ov010_021265ac(data, data->sprite.unk_C0) != 0) {
        if (data->sprite.unk_C0 >= data->sprite.unk_C2) {
            if (data->unk_2C == func_ov003_020cb7a4(1) / 2) {
                if (func_ov003_020c4348(data) != 0) {
                    func_ov010_02125938(data);
                    return;
                }
                data->sprite.unk_C0 = 1;
                data->sprite.unk_C2 = func_ov003_020c42ec(data);
                return;
            }
        }
    }
    data->sprite.unk_C0++;
}

// The DeadEff stage-0 constructor. The whole 0x1FC task data is cleared, not just the sprite
// block, and the engine bit is set *before* the init callback runs.
void func_ov010_02126a20(BtlEnm006* data, void* arg1) {
    MI_CpuSet(data, 0, 0x1FC);
    func_ov003_020c3efc(data, arg1);
    func_ov003_020c44ac(data);
    func_ov003_020c4b1c(data);
    data->unk_54 |= 0x80000000;
    func_ov010_02125910(data, (void*)func_ov010_02125998);
    data->unk_1CC                             = 1;
    data->unk_1DC                             = 0;
    data->unk_1E0                             = 0;
    data->unk_1E4                             = 0;
    ((Enm006SpriteAlt*)&data->sprite)->unk_E8 = 0;
}

// The DeadEff per-frame body: a 7-way stage switch, then the shared motion/tick tail. Cases 1 and
// 2 share a block, and 0/4/5 fall through to the tail untouched. Note the polarity of the two
// flag tests -- the *callback* runs when the bit is clear, because the original branches on the
// flags left by the `orrs`/`tst`, not on the value it just wrote.
s32 func_ov010_02126a94(BtlEnm006* data) {
    if (data->unk_1C8 != (void*)func_ov010_02125998) {
        if (func_ov003_020c3bf0() != 0) {
            return 1;
        }
    }
    s32 flag = data->unk_1C8 == (void*)func_ov010_02125de4;
    switch (func_ov003_02082f2c(data)) {
        case 1:
        case 2:
            if (flag == 0) {
                func_ov003_02084694((u8*)data + 0x144, 0);
                func_ov010_02125910(data, (void*)func_ov010_0212636c);
            }
            break;
        case 3:
            if (data->sprite.unk_8C & 0x10) {
                data->sprite.unk_8C |= 0x20;
            } else {
                func_ov010_02125910(data, (void*)func_ov010_02126420);
            }
            break;
        case 6:
            if (data->unk_54 & 1) {
                break;
            }
            data->unk_54 |= 1;
            func_ov010_02125910(data, (void*)func_ov010_021263c4);
            break;
    }
    data->unk_28 += data->unk_1DC;
    data->unk_2C += data->unk_1E0;
    data->unk_30 += data->unk_1E4;
    if (data->unk_1C8 != NULL) {
        ((void (*)(void))data->unk_1C8)();
    }
    u8* p = (u8*)data->sprite.unk_88;
    if (*(u16*)(p + 2) != 0) {
        if ((data->sprite.unk_8C & 1) || data->unk_C8 == 0xD) {
            func_ov010_02126934(data, 0);
            *(u16*)(p + 2) = 0;
        }
    }
    data->unk_54 |= 0x80000000;
    func_ov003_020c4628(data);
    return data->unk_1CC;
}

// The Swirl stage-0 constructor. The x coordinate carries a 0x20000 bias, and the mirror case
// *normalises* it: subtract the bias, and bounce straight back if that lands exactly on zero.
// Written as an `if (x == 0x20000) x += 0x20000` it compiles to the same thing but reads worse.
void func_ov010_02126d54(Enm006Swirl* data, Enm006Spawn* args) {
    MI_CpuSet(data, 0, 0xA0);
    BtlEnm006* d = args->unk_00;
    u16        v = d->unk_80;
    u32        m = *(u32*)((u8*)d + 0x84) & 3;
    func_ov003_02082a04(m, (u8*)data + 4, (void*)func_ov010_021256c0(v), 0, (u16)data_ov010_021292b0[v], 0x30);
    Mini108_VBlank((CombatSprite*)((u8*)data + 4), 0, 0);
    data->unk_00 = d;
    data->unk_68 = d->unk_28;
    data->unk_6C = d->unk_2C;
    data->unk_74 = 0;
    data->unk_7E = 0;
    if (d->unk_24 != 0) {
        data->unk_78 = 0;
        data->unk_68 -= 0x20000;
        if (data->unk_68 == 0) {
            data->unk_78 = 1;
            data->unk_68 += 0x20000;
        }
    } else {
        data->unk_78 = 1;
        data->unk_68 += 0x20000;
    }
    s32 i;
    for (i = 0; i < 8; i++) {
        data->unk_80[i] = 1;
    }
}

// Advances one of the eight swirl points along the spiral. The 4.12 fixed-point scale is
// converted up to 16.16 with a rounding term, which is what the `>> 29` in the original is.
void func_ov010_02126c94(Enm006Swirl* data, s32* outY, s32* outX, s32 index) {
    // The `>> 2` and the `>> 29` have to be two separate shifts; folded into one, MWCC emits
    // `asr #31` where the original has `asr #2 / lsr #29`.
    s32 t = index << 16;
    s32 h = t >> 2;
    s32 v = data->unk_7C + ((t + (h >> 29)) >> 3);
    func_ov003_020cbcb4(outY, outX, (s16)v, data->unk_70, 0x800);
    *outY += data->unk_68;
    *outX += data->unk_6C;
}

// The Swirl per-frame body. Two details are load-bearing: the y bounds test `>=` the *upper*
// limit while the x bounds test `>=` too but are reached by inverting a `blt`, and a point that
// fails the bounds test is *still emitted* -- only its `unk_80` slot is cleared, and the emit
// falls through into the same call.
s32 func_ov010_02126e58(Enm006Swirl* data) {
    s32 count = 0;
    if (func_ov003_020c3c28(data) != 0) {
        return 0;
    }
    if (data->unk_64 == 0) {
        data->unk_64++;
        func_ov003_02087f00(0x1D0, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_00->unk_28));
    }
    s32 hi = func_ov003_020cb744(0);
    s32 lo = func_ov003_020cb7a4(0);
    CombatSprite_Update((CombatSprite*)((u8*)data + 4));
    data->unk_70 += data->unk_74;
    // The two-step add is load-bearing: folded, it reassociates.
    s32 step = data->unk_74 + 0x9A;
    step += 0x100;
    data->unk_74 = step;
    if (data->unk_74 > 0x4000) {
        data->unk_74 = 0x4000;
    }
    if (data->unk_78 == 1) {
        data->unk_7C = data->unk_7C + data->unk_7E;
    } else {
        data->unk_7C = data->unk_7C - data->unk_7E;
    }
    data->unk_7E += 0x20;
    if (data->unk_7E > 0x200) {
        data->unk_7E = 0x200;
    }
    s32 i;
    for (i = 0; i < 8; i++) {
        if (data->unk_80[i] == 0) {
            continue;
        }
        s32 y;
        s32 x;
        func_ov010_02126c94(data, &y, &x, i);
        if (y - 0x10 < 0 || y + 0x10 >= hi || x - 0x10 < 0 || x + 0x10 >= lo) {
            data->unk_80[i] = 0;
        }
        func_ov003_020c5b2c(0x50, data->unk_00, y, x, 0);
        count++;
    }
    return count != 0;
}

// The other Swirl per-frame path: moves the single sprite to each live point in turn rather than
// emitting particles. Returns 1 unconditionally.
s32 func_ov010_02126fdc(Enm006Swirl* data) {
    s32 flag = func_ov003_020c37f8((u8*)data + 4) != 0;
    s32 i;
    for (i = 0; i < 8; i++) {
        if (data->unk_80[i] == 0) {
            continue;
        }
        s32 y;
        s32 x;
        s16 a;
        s16 b;
        func_ov010_02126c94(data, &y, &x, i);
        func_ov003_02084348(flag, &a, &b, x, y, 0);
        CombatSprite_SetPosition((CombatSprite*)((u8*)data + 4), a, b);
        func_ov003_02082730((CombatSprite*)((u8*)data + 4), 0x7FFFFFFF - y);
        CombatSprite_Render((CombatSprite*)((u8*)data + 4));
    }
    return 1;
}

// The Swlo stage-0 constructor. `unk_00` from the spawn arguments is a *mode flag*, not a
// pointer: it picks which of two target records gets the engine bits and the health value, and
// the register holding that choice is reused for the whole tail.
s32 func_ov010_021271c0(Enm006Swlo* data, Enm006Spawn* args) {
    MI_CpuSet(data, 0, 0x34);
    data->unk_00 = args->unk_00;
    data->unk_04 = args->unk_04;
    void* target;
    if (data->unk_00 == NULL) {
        target       = args->unk_08;
        data->unk_08 = target;
        s32 hit      = func_ov003_020c37f8((u8*)data->unk_04 + 0x84) != 0;
        data->unk_0C = hit ? (BtlEnm006*)args->unk_08 : (BtlEnm006*)args->unk_0C;
        s16 mode     = hit ? 0x52 : 0x4D;
        if (func_ov003_020c5b0c(mode, data->unk_0C, *(s32*)((u8*)data->unk_10 + 0x28)) != 1) {
            return 0;
        }
    } else {
        target       = args->unk_0C;
        data->unk_0C = target;
        func_ov003_02084694((u8*)target + 0x144, 1);
        func_ov003_02082f1c(target, 1);
        func_ov010_021256d0(data->unk_04, data->unk_0C);
    }
    data->unk_30 = (data->unk_30 & ~1) | 1;
    *(u32*)((u8*)target + 0x54) |= 0x10000000;
    *(u16*)((u8*)target + 0x10) = 0x258;
    data->unk_10                = *(s32*)((u8*)args->unk_04 + 0x28);
    data->unk_14                = *(s32*)((u8*)args->unk_04 + 0x2C);
    data->unk_2C                = 0x4000;
    data->unk_2E                = 0;
    data->unk_18                = (RNG_Next(9) - 4) << 12;
    data->unk_1C                = (RNG_Next(9) - 4) << 12;
    return 1;
}

// The Swlo per-frame body. The angle `unk_2C` climbs by 0x600 a frame and saturates at 0xC000;
// the frame it saturates on is the one that runs the "wrap" effect, and the return value is 1 on
// every frame before that. `unk_00` is the mode flag, tested twice: once to pick the record that
// takes the position, and again to pick which of the two wrap effects runs.
//
// Two things here are load-bearing for the shape of the code. The position stores go through
// `data->unk_08` / `data->unk_0C` directly rather than through a local, because the original
// re-derives the pointer for each of the three stores; with a local used instead, the two mode
// arms come out identical from the first store on and MWCC merges far more of them than the
// original does. And the `unk_30` store is duplicated rather than shared: the `sub / str` pair
// that both arms end on is itself the tail merge the original has, not a written-out statement.
s32 func_ov010_021272e0(Enm006Swlo* data) {
    data->unk_2C = data->unk_2C + 0x600;
    s32 result   = 1;
    if (data->unk_2C >= 0xC000) {
        data->unk_2C = 0xC000;
        result       = 0;
    }
    s32 x;
    s32 y;
    func_ov003_020cbcb4(&x, &y, data->unk_2C, 0x20000, 0x400);
    if (*(s32*)((u8*)data->unk_04 + 0x24) == 0) {
        x = -x;
    }
    if (data->unk_00 == NULL) {
        // The temp is load-bearing: without it MWCC hoists the `unk_10` load above the mode
        // branch (it is used on both arms), which the original does not do.
        s32 dx                          = x + data->unk_10;
        *(s32*)((u8*)data->unk_08 + 0x28) = dx + data->unk_18;
        *(s32*)((u8*)data->unk_08 + 0x2C) = y + data->unk_14;
        *(s32*)((u8*)data->unk_08 + 0x30) = data->unk_1C - 0x8000;
    } else {
        s32 dx              = x + data->unk_10;
        data->unk_0C->unk_28 = dx + data->unk_18;
        data->unk_0C->unk_2C = y + data->unk_14;
        data->unk_0C->unk_30 = data->unk_1C - 0x8000;
    }
    if (result == 0) {
        BtlEnm006* self;
        if (data->unk_00 == NULL) {
            self                      = (BtlEnm006*)data->unk_08;
            *(u16*)((u8*)self + 0x10) = 0;
            // Re-derived rather than `self`: the original reloads `data->unk_08` for these three
            // uses, and folding them onto the kept copy parks all of them on `r5` instead and
            // drops the four loads.
            *(s32*)((u8*)data->unk_08 + 0x40) = *(s32*)((u8*)data->unk_08 + 0x24) == 0 ? 0x1800 : -0x1800;
            *(s32*)((u8*)data->unk_08 + 0x38) = -0x5000;
            func_ov003_02082f1c(data->unk_08, 2);
        } else {
            self = data->unk_0C;
            self->sprite.unk_8C |= 0x1000;
            func_ov003_020c4fc8(data->unk_0C);
        }
        self->unk_54 &= ~0x10000000;
    }
    return result;
}
