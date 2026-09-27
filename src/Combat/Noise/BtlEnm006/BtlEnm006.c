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
extern s32 func_ov010_02125780(void*, void*);
extern s32 func_ov010_02126a20(void*, void*);
extern s32 func_ov010_02126a94(void*);
extern s32 func_ov010_02126d54(void*, void*);
extern s32 func_ov010_02126e58(void*);
extern s32 func_ov010_02126fdc(void*);
extern s32 func_ov010_021271c0(void*, void*);
extern s32 func_ov010_021272e0(void*);
extern s32 func_ov010_02128e80(void*);

// ov003 helpers.
extern void func_ov003_020c48b0(void*);
extern void func_ov003_020c4878(void*);
extern void func_ov003_020c48fc(void*);
extern void func_ov003_020c492c(void*);
extern s32  func_ov003_020c703c(void*);
extern void func_ov003_020c427c(void*);
extern void func_ov003_020cba54(s32, s32, s32, s32, s32, s32);
extern s32  func_ov003_020cb3c4(s32, s32);
extern s32  func_ov003_020c5bfc(void*);
extern s32  func_ov003_020c62c4(void*, s32);
extern s32  func_ov003_020c65cc(void*, s32);
extern s32  func_ov003_020c72b4(void*, s32, s32);
extern s32  func_ov003_020c37f8(void*);
extern s32  func_ov003_020c7070(void*);
extern s32  func_ov003_020c4e0c(void*);
extern s32  func_ov003_020cc354(void*);
extern s32  func_ov003_020cc38c(void*, s32, s32, s32, s32, s32, s32);
extern s32  func_ov003_020c3c28(void*);
extern s32  func_ov003_020c3efc(void*, void*);
extern void func_ov003_020c4520(void*);
extern void func_ov003_020c4b5c(void*);
extern void func_ov003_020c4668(void*);
extern void func_ov003_02084348(s32, void*, void*, s32, s32, s32);
extern void func_ov003_02082724(void*, s16, s16);
extern void func_ov003_02082b64(void*);
extern void func_ov003_02084694(void*, s32);
extern s32  func_ov003_020cb910(void*, void*, s32, s32, s32, s32, s32, s32, s32, void*);
extern void func_ov003_020cb498(s32, s32, void*, void*);
extern s32  func_ov003_020843b0(s32, s32);
extern void Mini108_VBlank(CombatSprite*, u16, s32);
extern s32  func_ov010_02128820(BtlEnm006*);

// Per-instance callbacks passed to the init helpers.
extern void func_ov010_021259e8(void);
extern void func_ov010_02127550(void);
extern void func_ov010_02127764(void);
extern void func_ov010_02125c80(void);
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
            return func_ov010_02126a20(data, args);
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
            return func_ov010_02126d54(data, args);
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
            return func_ov010_021272e0(data);
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
    Enm006SpriteAlt2* alt = (Enm006SpriteAlt2*)&data->sprite;
    if (arg1 == 0) {
        alt->unk_F0 = 0;
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
