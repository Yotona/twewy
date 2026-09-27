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
extern s32 func_ov010_02125878(void*);
extern s32 func_ov010_02126a20(void*, void*);
extern s32 func_ov010_02126a94(void*);
extern s32 func_ov010_02126d54(void*, void*);
extern s32 func_ov010_02126e58(void*);
extern s32 func_ov010_02126fdc(void*);
extern s32 func_ov010_021271c0(void*, void*);
extern s32 func_ov010_021272e0(void*);
extern s32 func_ov010_02128e0c(void*, void*);
extern s32 func_ov010_02128e80(void*);

// ov003 helpers.
extern void func_ov003_020c48b0(void*);
extern void func_ov003_020c4878(void*);
extern void func_ov003_020c48fc(void*);
extern void func_ov003_020c492c(void*);
extern s32  func_ov003_020c703c(void*);

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
