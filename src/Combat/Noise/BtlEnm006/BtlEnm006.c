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
extern s32  func_ov010_02125780(Enm006DeadEff*, Enm006Spawn*);
extern void func_ov010_02126a20(BtlEnm006*, void*);
extern s32  func_ov010_02126a94(BtlEnm006*);
extern void func_ov010_02126d54(Enm006Swirl*, Enm006Spawn*);
extern s32  func_ov010_02126e58(Enm006Swirl*);
extern s32  func_ov010_02126fdc(Enm006Swirl*);
extern s32  func_ov010_021271c0(Enm006Swlo*, Enm006Spawn*);
extern s32  func_ov010_021272e0(Enm006Swlo*);
extern s32  func_ov010_02128e80(BtlEnm006*);

// ov003 helpers.
extern void func_ov003_020c48b0(void*);
extern void func_ov003_020c4878(void*);
extern void func_ov003_020c48fc(void*);
extern void func_ov003_020c492c(void*);
extern s32  func_ov003_020c703c(void*);
extern void func_ov003_020c427c(void*);
extern s32  func_ov003_020cba54(s32, s32, s32, s32, s32, s32);
// Bearing from (x0, y0) to (x1, y1): FX_Atan2Idx(y1 - y0, x1 - x0).
extern s32                   func_ov003_020cba14(s32, s32, s32, s32);
extern void                  func_ov003_020cbcb4(s32*, s32*, s16, s32, s32);
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
extern s32                   func_ov003_020cb498(s32, s32, void*, void*);
extern s32                   func_ov003_020843b0(s32, s32);
extern void                  Mini108_VBlank(CombatSprite*, u16, s32);
extern void                  func_ov010_02128820(BtlEnm006*, s32);
extern void                  CombatSprite_SetPaletteSource(CombatSprite*, s32);
extern void                  func_ov003_020c4ab4(BtlEnm006*, s32);
extern s32                   func_ov003_020c6230(void*);
extern s32                   func_ov003_020c4c1c(void*);
extern void                  func_ov003_020c4ee0(void*);
extern s32                   func_ov003_020c5b2c(s32, void*, s32, s32, s32);
extern const SpriteAnimEntry data_ov010_02129238[3];
extern const SpriteAnimEntry data_ov010_021292f4[3];

extern s32   func_ov010_02128cbc(s32, void*, void*);
extern void  func_ov003_020c4c5c(BtlEnm006*);
extern s32   func_ov003_020cc300(BtlEnm006*);
extern void  func_ov003_020cb578(BtlEnm006*, s32);
extern s32   func_ov010_02128bcc(void*, void*);
extern void  func_ov010_02127110(void*, void*);
extern void  func_ov010_02127cc0(void);
extern void  func_ov010_0212847c(s32*, s32*, BtlEnm006*, s32);
extern s32   func_ov003_020cba2c(s32, s32, s32, s32);
extern s32   func_ov003_020cb764(s32);
extern s32   func_ov003_020cb784(s32);
extern void  func_ov003_020c4c9c(BtlEnm006*);
extern s32   func_ov010_021270a8(void*, void*);
extern s32   func_ov003_020cb744(s32);
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

/// A 16-byte record out of the 0x02129250..0x021292B0 blob. `data_ov010_02129250`,
/// `data_ov010_02129254`, `data_ov010_02129258` and `data_ov010_0212925c` are four *overlapping*
/// views of that one 0x60-byte block (each is read with a 16-byte stride), so they cannot all be
/// declared with their real element type. The block is tiled below by non-overlapping objects to
/// keep the byte image -- and therefore the .rodata match -- intact, and the strided reads go
/// through casts of those objects.
typedef struct Enm006PhaseRec {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
} Enm006PhaseRec;

// Per-instance callbacks passed to the init helpers.
extern void func_ov010_021259e8(BtlEnm006*);
extern void func_ov010_02127764(BtlEnm006*);
extern void func_ov010_02125c80(BtlEnm006*);
extern void func_ov010_02125b28(BtlEnm006*);
extern void func_ov010_02125938(BtlEnm006*);
extern s32  func_ov010_0212643c(BtlEnm006*);
extern s32  func_ov010_021265ac(BtlEnm006*, s32);
extern s32  func_ov003_020cb7a4(s32);
extern void func_ov003_020c44ac(BtlEnm006*);
extern void func_ov003_020c4b1c(BtlEnm006*);
extern void func_ov003_020c4628(BtlEnm006*);
extern s32  func_ov003_020c3bf0(void);
extern s32  func_ov010_02126420(void*);
extern void func_ov010_0212636c(BtlEnm006*);
extern void func_ov010_021263c4(BtlEnm006*);
extern void func_ov010_02125998(BtlEnm006*);
extern void func_ov010_02127550(BtlEnm006*);
extern void func_ov010_02125de4(BtlEnm006*);

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
// the two stack arguments are evaluated first. Both let `func_ov003_020cba54`'s result fall
// through in r0 -- that is what `func_ov010_02128820` compares against 0x2800.
s32 func_ov010_0212688c(BtlEnm006* data) {
    return func_ov003_020cba54(data->unk_28, data->unk_2C, data->unk_30, data->unk_1D0, data->unk_1D4, data->unk_1D8);
}

s32 func_ov010_02128ac8(BtlEnm006* data) {
    return func_ov003_020cba54(data->unk_28, data->unk_2C, data->unk_30, data->unk_1D0, data->unk_1D4, data->unk_1D8);
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

// Two rounds of the same list walk. The first just counts the eligible nodes; if there were some,
// a 20% coin flip then draws one of them and walks the list again to find it. The 8-byte local is
// that second walk's out-param, not a local pair: `func_ov010_02128c6c` counts down from its first
// word and stores the winning node into its second, and the function returns that second word
// without ever writing it itself. The default answer -- used when nothing matched, or when the coin
// flip came up short -- is a field of the global block, reached by dereferencing the global pointer
// first, so the two offsets have to stay separate. The draw has to sit *after* the default via an
// explicit `goto`: written as a nested `if` MWCC inverts the test and places the (cold) draw last
// with a `blo` over it, which also reorders the three literal-pool words.
s32 func_ov010_02128b48(void) {
    s32 hits = func_ov003_020cb498(0, 0x3C, (void*)func_ov010_02128c3c, 0);
    s32 local[2];
    if (hits != 0 && RNG_Next(0x64) >= 0x14) {
        goto pick;
    }
    return *(s32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898);
pick:
    local[0] = RNG_Next(hits);
    func_ov003_020cb498(0, 0x3C, (void*)func_ov010_02128c6c, local);
    return local[1];
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

// The DeadEff stage driver: a dense seven-case switch on sprite.unk_C4, the same shape as
// func_ov010_02125b28 below with four more phases. Its cases have to stay dense and in order --
// the original's dispatch is a jump table (`cmp #6 / addls pc, pc, r1, lsl #2` over seven `b`s),
// where func_ov010_02125b28's three cases get a compare chain.
//
// The two `unk_8C |= 1` writes in cases 2 and 5 are genuinely duplicated in the original, with
// the `unk_54` update wedged between them, so they are spelled twice here rather than factored.
void func_ov010_02125de4(BtlEnm006* data) {
    CombatSprite* cs = (CombatSprite*)((u8*)data + 0x84);
    switch (data->sprite.unk_C4) {
        case 0:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                // The low two bits of unk_1F4 pick the variant: 1 on the short draw, 0 on the
                // long one. Both arms read the field once and the whole thing if-converts
                // (`biclo / orrlo / bichs`), so the test is the unsigned one the original wants.
                if ((u32)RNG_Next(0x64) < 0x1E) {
                    data->unk_1F4 = (data->unk_1F4 & ~3) | 1;
                } else {
                    data->unk_1F4 = data->unk_1F4 & ~3;
                }
                data->unk_1F8 = -1;
            }
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 1;
            return;
        case 1:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                Mini108_VBlank(cs, 3, 1);
                func_ov003_020c4c1c(data);
                func_ov003_02087f00(0x1CC, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_28));
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 2;
            return;
        case 2:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_8C |= 1;
                data->unk_54 |= 0x20;
                data->sprite.unk_8C |= 1;
                func_ov003_020cb578(data, 0);
                // Three phase lengths, keyed on the low two bits of unk_1F4; a value of 3 keeps
                // whatever was already there. An if/else-if chain rather than a `switch`: the
                // original's first two arms are relocated behind forward branches and only the
                // last one is inline and predicated, which is how a chain lays out.
                s32 v = (u32)((s32)data->unk_1F4 << 30) >> 30;
                switch (v) {
                    case 0:
                        data->sprite.unk_C2 = 0x3C;
                        break;
                    case 1:
                        data->sprite.unk_C2 = 0x78;
                        break;
                    case 2:
                        data->sprite.unk_C2 = 0x1E;
                        break;
                }
            }
            if (data->sprite.unk_C0 < data->sprite.unk_C2) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 3;
            return;
        case 3:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                Mini108_VBlank(cs, 0xD, 1);
                data->sprite.unk_8C &= ~1;
                // The two arms re-derive the global rather than sharing one fetch, and the `+`
                // arm is the fall-through with the `-` arm relocated behind a forward branch.
                if (data->unk_24 == 0) {
                    data->unk_28 = data_ov003_020e71b8->unk3D838 + 0x10000;
                } else {
                    data->unk_28 = data_ov003_020e71b8->unk3D838 - 0x10000;
                }
                data->unk_2C = data_ov003_020e71b8->unk3D83C;
                data->unk_30 = 0;
                if (data->unk_24 == 1) {
                    data->unk_28 = data->unk_28 - 0xB000;
                } else {
                    data->unk_28 = data->unk_28 + 0xB000;
                }
            }
            if (data->unk_9A == 0xF && data->unk_8C == 1) {
                func_ov003_02087f00(0x1CD, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_28));
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 4;
            return;
        case 4:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                Mini108_VBlank(cs, 0xE, 1);
                data->unk_54 &= ~0x20;
                func_ov003_020cb578(data, 1);
                func_ov003_020c4c9c(data);
                func_ov003_02087f00(0x1C9, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_28));
            }
            if (data->unk_9A == 6 && data->unk_8C == 1) {
                func_ov003_02087f00(0x1CA, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_28));
            }
            if (data->unk_9A == 9 && data->unk_8C == 1) {
                func_ov003_02087f00(0x1CB, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_28));
            }
            if (data->unk_9A == 4 && data->unk_8C == 1) {
                // The one local here really does live across two calls, so it takes r5 -- which
                // is why the original pushes it. The address form has to be the two-step
                // `&symbol / [reg] / + 0x3D000 / [reg, #off]`, i.e. a real struct field.
                void* t = data_ov003_020e71b8->unk3D89C;
                if (func_ov010_021268c4(data, t) != 0) {
                    data->unk_1F8 = func_ov010_021270a8(data, t);
                }
            }
            if (data->unk_9A < 9) {
                if (data->unk_24 == 0) {
                    data->unk_28 = data->unk_28 + 0x800;
                } else {
                    data->unk_28 = data->unk_28 - 0x800;
                }
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            data->sprite.unk_C0 = 0;
            // The shift pair has to be a *value* (the `== 1` keeps its `cmp`, because the test is
            // a conjunction and not a lone branch), and the `-1` is a fresh `sub` off the zero
            // that is already in the register from the store above.
            if ((u32)((s32)data->unk_1F4 << 30) >> 30 == 1 && data->unk_1F8 == -1) {
                data->unk_1F4       = (data->unk_1F4 & ~3) | 2;
                data->sprite.unk_C4 = 2;
                return;
            }
            data->sprite.unk_C4 = 5;
            return;
        case 5:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_8C |= 1;
                data->unk_54 |= 0x20;
                func_ov003_020cb578(data, 0);
                data->sprite.unk_C2 = 0x3C;
            }
            if (data->sprite.unk_C0 < data->sprite.unk_C2) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 6;
            return;
        case 6:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                data->sprite.unk_8C &= ~1;
                data->unk_54 &= ~0x20;
                Mini108_VBlank(cs, 4, 1);
                func_ov003_020cb578(data, 1);
                if (data->unk_24 == 0) {
                    data->unk_28 = func_ov003_020cb764(1) + 0x60000;
                } else {
                    data->unk_28 = func_ov003_020cb764(1) - 0x60000;
                }
                data->unk_2C = func_ov003_020cb784(1);
                data->unk_30 = 0;
                func_ov003_020c4b1c(data);
                func_ov003_02087f00(0x1CC, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_28));
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            func_ov010_02125910(data, (void*)func_ov010_021259e8);
            func_ov003_020c4ee0(data);
            return;
    }
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

// The two-phase variant of func_ov010_02125b28: a swoop across the play area, one phase of
// setup and then one of motion. Phase 0 does nothing but arm phase 1, and only while the engine
// is not already animating; phase 1 is the one that picks a target 0x60000 either side of the
// screen centre, asks func_ov010_02126830 how many frames the move takes, and drives the frame
// counter until it does. Dispatched through `unk_1C8`, which is why the signature is
// `(BtlEnm006*)` and not nullary.
//
// Three things here are re-derived per use rather than cached, because the original re-derives
// them: the screen bound behind the `unk_1D0` bias is fetched three times (once for the guard,
// once inside each arm), the two velocity copies are plain stores rather than reads into locals,
// and the `unk_C0 < unk_C2` guard re-reads both fields instead of reusing the phase setup.
void func_ov010_02125c80(BtlEnm006* data) {
    switch (data->sprite.unk_C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 1;
            return;
        case 1:
            if (data->sprite.unk_C0 == 0) {
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0xC, 0);
                // 4.12 fixed point: the target x is the screen centre offset by 0x60000, plus if
                // the sprite is left of the centre and minus if it is right of it. The `+` arm is
                // the fall-through and the `-` arm is the branch target, and the bound is
                // re-fetched inside whichever arm runs. The guard's `bge` is *signed*, so both
                // operands stay signed -- a `(u32)` here turns it into `bhs`.
                if (data->unk_28 < func_ov003_020cb744(1) >> 1) {
                    data->unk_1D0 = (func_ov003_020cb744(1) >> 1) + 0x60000;
                } else {
                    data->unk_1D0 = (func_ov003_020cb744(1) >> 1) - 0x60000;
                }
                data->unk_1D4       = data->unk_2C;
                data->unk_1D8       = data->unk_30;
                data->sprite.unk_C2 = func_ov010_02126830(data, (void*)0x4000);
                func_ov003_020c4c1c(data);
            }
            if (data->unk_9A == 1 && data->unk_8C == 1) {
                // The second parameter of func_ov003_02087f00 is declared as a callback pointer, but
                // the original really calls 020843b0 here and passes the result.
                func_ov003_02087f00(0x1CF, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_28));
            }
            // (cmd, owner, x, y, z). The stack argument is the one MWCC evaluates *first*, so
            // z is the load that reaches [sp] ahead of r2/r3 -- passing x/y/z in the wrong order
            // still looks plausible but swaps two loads.
            func_ov003_020c5b2c(0x51, data, data->unk_28, data->unk_2C, data->unk_30);
            if (data->sprite.unk_C0 < data->sprite.unk_C2) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                return;
            }
            func_ov010_02125910(data, (void*)func_ov010_021259e8);
            func_ov003_020c4ee0(data);
            return;
    }
}

// The near-twin of func_ov010_02125c80's phase-1 body -- this is the other callback handed to
// func_ov010_02127488, so the signature is `(BtlEnm006*)` and not nullary. On the first frame it
// arms the swoop, then mirrors the sprite off the sign of the z velocity; on every frame it fires
// the one-shot effect, hands the position to the sound helper and advances the phase counter.
//
// Two things are the mirror image of func_ov010_02125c80 and are load-bearing:
//
//  - The screen bound is fetched **once** and both arms reuse the `unk_28` already in a register,
//    so this is the if-converted `addlt`/`subge` form. func_ov010_02125c80 puts a second call in
//    each arm (three calls in total), which is also why it branches there instead of predicating:
//    its arms are too big to if-convert, while these two are a single add each.
//  - The mirror test is the one place where the original branches instead of predicating, because
//    both of its arms are calls. The `0` call is the fall-through and the `1` call is the forward
//    branch target, so the condition is spelled `>= 0` and the `1` arm is the `then`.
void func_ov010_02127650(BtlEnm006* data) {
    if (data->sprite.unk_C0 == 0) {
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0xC, 0);
        s32 mid = func_ov003_020cb744(0);
        if (data->unk_28 < mid >> 1) {
            data->unk_1D0 = data->unk_28 + 0x80000;
        } else {
            data->unk_1D0 = data->unk_28 - 0x80000;
        }
        data->unk_1D4       = data->unk_2C;
        data->unk_1D8       = data->unk_30;
        data->sprite.unk_C2 = func_ov010_02128a6c(data, (void*)0x4000);
        if (data->unk_1DC < 0) {
            func_ov003_020c4ab4(data, 0);
        } else {
            func_ov003_020c4ab4(data, 1);
        }
    }
    if (data->unk_9A == 1 && data->unk_8C == 1) {
        // As in func_ov010_02125c80: the second parameter is declared as a callback pointer, but
        // the original really calls 020843b0 here and passes the result.
        func_ov003_02087f00(0x1CF, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_28));
    }
    // (cmd, owner, x, y, z). The stack argument is the one MWCC evaluates *first*, so z is the
    // load that reaches [sp] ahead of r2/r3.
    func_ov003_020c5b2c(0x4C, data, data->unk_28, data->unk_2C, data->unk_30);
    if (data->sprite.unk_C0 < data->sprite.unk_C2) {
        data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127550);
}

// The six-phase behaviour, the sibling of func_ov010_02127650 and the other one
// func_ov010_02127488 hands to the init helper, so the signature is `(BtlEnm006*)`.
//
// Like func_ov010_02125de4 the six cases are dense, in order and gapless, so the original's
// dispatch is a jump table, and every phase re-enters its one-time setup only while
// `sprite.unk_C0` is still zero. Phases 1 and 4 are the fixed-length "wobble" phases with a
// duplicated `unk_8C |= 1`; phase 2 is the only one that borrows a *second* instance, via
// func_ov010_02128b48, and that pointer is what the original keeps in a callee-saved register
// across six calls. Phase 5's tail is the only one that does not advance the phase counter --
// it hands off to func_ov010_02127550 instead.
//
// The sound indices: 0x1CC is a rotated immediate and stays a `mov`, while 0x1C9/0x1CA/0x1CB/
// 0x1CD are not and come from the literal pool. The pool order follows first reference, so
// the C order of the blocks *is* the pool order.
void func_ov010_02127764(BtlEnm006* data) {
    switch (data->sprite.unk_C4) {
        case 0:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 3, 1);
                func_ov003_020c4c5c(data);
                // The `else` arm is the one behind the forward branch: it is five instructions
                // to the `then` arm's two, and the single `ldrb` ahead of the branch feeds both.
                if (data->unk_54 & 0x40000000) {
                    data->unk_1F5 |= 2;
                } else {
                    data->unk_1F5 &= ~2;
                    data->unk_54 |= 0x40000000;
                }
                func_ov003_02087f00(0x1CC, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_28));
            }
            if (SpriteMgr_IsAnimationFinished(&((CombatSprite*)((u8*)data + 0x84))->sprite) == 0) {
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
                func_ov003_020cb578(data, 0);
                data->sprite.unk_C2 = 0x3C;
            }
            if (data->sprite.unk_C0 < data->sprite.unk_C2) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 2;
            return;
        case 2: {
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                BtlEnm006* other    = (BtlEnm006*)func_ov010_02128b48();
                if (other != NULL) {
                    // An if/else, not a ternary: the original *predicates* both stores
                    // (`moveq` / `streqb` / `movne` / `strneb`), and a ternary collapses into a
                    // phi instead -- one word shorter, and a different encoding.
                    if (*(s32*)other == 1) {
                        data->unk_1F4 = 1;
                    } else {
                        data->unk_1F4 = 0;
                    }
                }
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0xD, 1);
                data->sprite.unk_8C &= ~1;
                // Both arms are calls, so this branches rather than predicating; the `bge`
                // target is the later block, which is the polarity the original has.
                if (data->unk_28 < other->unk_28) {
                    func_ov003_020c4ab4(data, 1);
                } else {
                    func_ov003_020c4ab4(data, 0);
                }
                data->unk_2C = other->unk_2C;
                data->unk_30 = 0;
                if (data->unk_2C < 0x20000) {
                    data->unk_2C = 0x20000;
                }
                // The two arms re-derive the other's x rather than sharing one fetch, and both
                // spellings of it if-convert (`addeq` / `streq` / `subne` / `strne`).
                if (data->unk_24 == 0) {
                    data->unk_28 = other->unk_28 + 0x1C000;
                } else {
                    data->unk_28 = other->unk_28 - 0x1C000;
                }
                if (func_ov003_020cc300(data) == 0) {
                    func_ov003_020c4c9c(data);
                    if (data->unk_24 == 0) {
                        data->unk_28 = other->unk_28 + 0x1C000;
                    } else {
                        data->unk_28 = other->unk_28 - 0x1C000;
                    }
                }
            }
            if (data->unk_9A == 0xF && data->unk_8C == 1) {
                func_ov003_02087f00(0x1CD, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_28));
            }
            if (SpriteMgr_IsAnimationFinished(&((CombatSprite*)((u8*)data + 0x84))->sprite) == 0) {
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 3;
            return;
        }
        case 3:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0xE, 1);
                data->unk_54 &= ~0x20;
                if (data->unk_1F4 == 0) {
                    func_ov003_020cb578(data, 1);
                }
                func_ov003_020c4c9c(data);
                func_ov003_02087f00(0x1C9, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_28));
            }
            if (data->unk_9A == 6 && data->unk_8C == 1) {
                func_ov003_02087f00(0x1CA, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_28));
            }
            if (data->unk_9A == 9 && data->unk_8C == 1) {
                func_ov003_02087f00(0x1CB, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_28));
            }
            if (data->unk_9A == 4 && data->unk_8C == 1) {
                // The one local here really does live across two calls, so it takes a
                // callee-saved register -- which is why the original pushes it. The address
                // form has to be the two-step `&symbol / [reg] / + 0x3D000 / [reg, #off]`,
                // i.e. a real struct field.
                void* t = data_ov003_020e71b8->unk3D898;
                if (func_ov010_02128bcc(data, t) != 0) {
                    // The result really is discarded: the original goes straight from this
                    // call to the next block's argument setup, with no store.
                    func_ov010_021270a8(data, t);
                }
                ((Enm006SpriteAlt4*)&data->sprite)->unk_F6 = func_ov003_020cb498(0, 0x3C, (void*)func_ov010_02128cbc, data);
                ((Enm006SpriteAlt4*)&data->sprite)->unk_F8 = ((Enm006SpriteAlt4*)&data->sprite)->unk_F6;
            }
            if (data->unk_9A < 9) {
                if (data->unk_24 == 0) {
                    data->unk_28 = data->unk_28 + 0x800;
                } else {
                    data->unk_28 = data->unk_28 - 0x800;
                }
            }
            if (SpriteMgr_IsAnimationFinished(&((CombatSprite*)((u8*)data + 0x84))->sprite) == 0) {
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 4;
            return;
        case 4:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_8C |= 1;
                data->unk_54 |= 0x20;
                func_ov003_020cb578(data, 0);
                data->sprite.unk_C2 = 0x3C;
            }
            if (data->sprite.unk_C0 < data->sprite.unk_C2) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                return;
            }
            data->sprite.unk_C0 = 0;
            data->sprite.unk_C4 = 5;
            return;
        case 5:
            if (data->sprite.unk_C0 == 0) {
                data->sprite.unk_C0 = data->sprite.unk_C0 + 1;
                data->sprite.unk_8C &= ~1;
                data->unk_54 &= ~0x20;
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 4, 1);
                func_ov003_020cb578(data, 1);
                func_ov003_020c4c5c(data);
                // Random 4.12 cell, one cell in from the two screen edges. Both bounds are
                // fetched as separate calls, each shifted down before the `+ 1`.
                data->unk_28 = RNG_Next((func_ov003_020cb744(0) >> 12) + 1) << 12;
                data->unk_2C = RNG_Next((func_ov003_020cb7a4(0) >> 12) + 1) << 12;
                data->unk_30 = 0;
                func_ov003_02087f00(0x1CC, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_28));
            }
            if (SpriteMgr_IsAnimationFinished(&((CombatSprite*)((u8*)data + 0x84))->sprite) == 0) {
                return;
            }
            // The bit test is a shift pair with the `(u32)` on the outside; the inner `lsl`
            // has to be the unsigned one. Nothing follows but the hand-off.
            if (((u32)data->unk_1F5 << 30) >> 31 == 0) {
                data->unk_54 &= ~0x40000000;
            }
            func_ov010_02127460(data, (void*)func_ov010_02127550);
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
    func_ov010_02128820(data, arg1);
    if ((data->unk_9A - 1) % 4 == 0) {
        if (data->unk_8C == 1) {
            // The second parameter of func_ov003_02087f00 is declared as a callback pointer, but
            // the original really calls 020843b0 here and passes the result.
            func_ov003_02087f00(0x1CE, (s32(*)(s32, s32))func_ov003_020843b0(0, data->unk_28));
        }
    }
    return 1;
}

// Finds the nearest of the six waypoints and records which side of the segment the owner is on.
// The return value is the waypoint's index, 0..5, and it is what `func_ov010_021265ac` stores in
// `unk_1EC` on the first frame of a re-seek.
//
// The table's coordinates are stored relative to the centre of the play area, so every use
// re-derives `bound >> 1`: the *raw* bound is what the two calls' results stay in, and the halving
// happens at each of the four places it is used (twice in the loop's guards, once in the distance
// call, once in the fallback compare).
s32 func_ov010_0212643c(BtlEnm006* data) {
    // Declared ahead of the two bound getters: the two constant seeds have to be materialised
    // *before* the first call, and only this declaration order gets `mvn r9` / `sub r6` there.
    s32 bestD = 0x7FFFFFFF;
    s32 best  = 0x7FFFFFFF - 0x80000000;
    s32 cx    = func_ov003_020cb744(1);
    s32 cy    = func_ov003_020cb7a4(1);
    s32 i;

    for (i = 0; i < 6; i++) {
        // Two sequential guards sharing the one mirror-flag load, not an `else if`: the original
        // falls out of the first into the second. A waypoint on the wrong side of the owner is
        // skipped entirely; `unk_24` selects which side that is.
        if (data->unk_24 == 0 && ((const Enm006PhaseRec*)&data_ov010_02129250)[i].unk_00 + (cx >> 1) > data->unk_28) {
            continue;
        }
        if (data->unk_24 == 1 && ((const Enm006PhaseRec*)&data_ov010_02129250)[i].unk_00 + (cx >> 1) < data->unk_28) {
            continue;
        }
        // Owner first, waypoint second: the two waypoint halves land in r2/r3 as the *third* and
        // *fourth* arguments, which is what lets them collapse into the original's single
        // `ldmia` of the record's two words. Read into locals, or the pair does not merge.
        s32 px = ((const Enm006PhaseRec*)&data_ov010_02129250)[i].unk_00;
        s32 py = ((const Enm006PhaseRec*)&data_ov010_02129250)[i].unk_04;
        s32 d  = func_ov003_020cba2c(data->unk_28, data->unk_2C, px + (cx >> 1), py + (cy >> 1));
        if (d < bestD) {
            bestD = d;
            best  = i;
        }
    }
    if (best == 0x7FFFFFFF - 0x80000000) {
        // Nothing was on the right side at all: fall back to the leftmost or rightmost waypoint.
        best = (data->unk_28 < (cx >> 1)) ? 3 : 0;
    }

    // The neighbour of the best waypoint, wrapping: `0x2aaaaaab` with no trailing shift is a
    // divide by six. Two statements, not one `= (best + 1) % 6` -- the one-statement form moves
    // `best` out of r6 and costs eight instructions.
    s32 next = best + 1;
    next %= 6;
    // `toOwner` is declared first so that it gets the *higher* of the two 12-byte frame slots
    // (0xC) and `edge` the lower one (0x0) -- the reverse swaps both the stores and the two
    // pointer arguments to Vec_DotProduct. The z components are explicit zeros.
    //
    // The y component of a record lives in the *overlapping* 0x02129254 view, which is why the y
    // terms name a second symbol: that is what puts `data_ov010_02129254` in the literal pool
    // between the reciprocal and the 0x02129250 reload, as the original has it.
    //
    // The original re-reads each `best` word twice here -- once for `toOwner` and once for `edge`
    // -- six loads in all, where the folded version needs four. Forcing the second read back
    // (a `volatile` cast does it) costs more than it saves: the two extra live values push the
    // reciprocal's dividend out of the callee-saved register the original keeps it in, and the
    // whole tail re-colours. Left folded; that hunk is the known residue.
    Vec toOwner;
    Vec edge;
    toOwner.x = data->unk_28 - (((const Enm006PhaseRec*)&data_ov010_02129250)[best].unk_00 + (cx >> 1));
    toOwner.y = data->unk_2C - (((const Enm006PhaseRec*)&data_ov010_02129254)[best].unk_00 + (cy >> 1));
    toOwner.z = 0;
    edge.x    = ((const Enm006PhaseRec*)&data_ov010_02129250)[next].unk_00 -
             ((const Enm006PhaseRec*)&data_ov010_02129250)[best].unk_00;
    edge.y = ((const Enm006PhaseRec*)&data_ov010_02129254)[next].unk_00 -
             ((const Enm006PhaseRec*)&data_ov010_02129254)[best].unk_00;
    edge.z = 0;
    // A dot product of at least zero faces right. Bit 2 of `unk_1F4` is what `func_ov010_021265ac`
    // reads on the next frame, so it is set *or cleared* here rather than only set.
    if (Vec_DotProduct(&toOwner, &edge) >= 0) {
        data->unk_1F4 |= 4;
    } else {
        data->unk_1F4 &= ~4;
    }
    return best;
}

// The waypoint chase. `arg1` is the sprite's frame counter: on the first frame (0) it re-seeks the
// nearest waypoint with func_ov010_0212643c, and only then does it pick a velocity target from the
// phase tables. The phase counter `unk_1EC` is a six-entry ring, so the wrap is `(x + 6) % 6`, not
// `% 10` -- the 0x2aaaaaab reciprocal with no trailing shift is a divide by six.
//
// `unk_1F4` bit 2 is set by func_ov010_0212643c from the sign of the owner/edge dot product, and it
// both picks the direction of the phase step and selects between the two indexings of the phase
// tables. The test is the shift pair `lsl #29 / lsr #31`, so it has to be spelled with a `(u32)`.
//
// `v` is the halfword handed to the sprite restart as its second argument. The original's value is
// dead -- the `moveq r1, #1 / moveq r1, #2` chain in the reflected arm is all that is left of it --
// and on the `arg1 == 0` path it is whatever the register happened to hold, which is why that arm
// stores the zero written to `unk_1D8` rather than reloading it.
//
// Both nested tests are `goto`s rather than if/else: MWCC if-converts an if/else into predicated
// loads, and the original branches past the cold arm in both cases. The `!= 1` in the second one
// is load-bearing: `== 0` lets MWCC fold the compare into the shift's own Z flag and emits
// `movs / beq` instead of `cmp r0, #0x1 / bne`.
//
// The mirror flag is written back through `arg1` rather than through a local of its own. The
// original's `cmp r4, #0` is the last read of the argument and r4 is the register the flag lands
// in, so a separate local costs a whole extra callee-saved register here (and with it r5/r6 for
// `data`, which moves every `data`-relative load in the function).
s32 func_ov010_021265ac(BtlEnm006* data, s32 arg1) {
    if (arg1 == 0) {
        ((Enm006SpriteAlt2*)&data->sprite)->unk_F0 = 0;
        data->unk_1EC                              = func_ov010_0212643c(data);
        if (((u32)data->unk_1F4 << 29) >> 31 == 1) {
            data->unk_1EC = data->unk_1EC + 1;
        } else {
            data->unk_1EC = data->unk_1EC - 1;
        }
        // Two statements, not one `= (x + 6) % 6`. Same arithmetic, but only the second form
        // keeps the dividend in r12 across the reciprocal multiply instead of in r3.
        data->unk_1EC += 6;
        data->unk_1EC %= 6;
    }
    if (((Enm006SpriteAlt2*)&data->sprite)->unk_F0 == 0) {
        u16 v;
        if (((u32)data->unk_1F4 << 29) >> 31 == 1) {
            data->unk_1EC = data->unk_1EC - 1;
        } else {
            data->unk_1EC = data->unk_1EC + 1;
        }
        data->unk_1EC += 6;
        data->unk_1EC %= 6;
        data->unk_1D0 = ((const Enm006PhaseRec*)&data_ov010_02129250)[data->unk_1EC].unk_00 + (func_ov003_020cb744(1) >> 1);
        data->unk_1D4 = ((const Enm006PhaseRec*)&data_ov010_02129254)[data->unk_1EC].unk_00 + (func_ov003_020cb7a4(1) >> 1);
        data->unk_1D8 = 0;
        if (arg1 == 0) {
            // `v` is dead here, but it still has to be *assigned* on every path that reaches the
            // sprite restart: left undefined in one arm, MWCC extends its live range back to the
            // function entry, gives it a callee-saved register and shifts `arg1`/`data` up to
            // r5/r6, which moves every `data`-relative load in the function. This costs nothing
            // -- the register already holds the zero written to unk_1D8.
            v = 0;
            // Both arms move, so the `<` arm has to name the register that already holds the
            // zero written to unk_1D8: writing `arg1 = 0; if (...) arg1 = 1;` lets MWCC fold the
            // conditional away as redundant and drops the `movlt`.
            if (data->unk_1D0 < data->unk_28) {
                arg1 = 0;
            } else {
                arg1 = 1;
            }
        } else {
            if (((u32)data->unk_1F4 << 29) >> 31 != 1) {
                goto reflected;
            }
            {
                s32 idx = (data->unk_1EC + 1) % 6;
                v       = ((const u16*)&data_ov010_02129258)[idx * 8];
                arg1    = ((const Enm006PhaseRec*)&data_ov010_0212925c)[idx].unk_00 ^ 1;
                if (v == 2) {
                    v = 1;
                    goto restart;
                }
                if (v == 1) {
                    v = 2;
                }
            }
            goto restart;
        reflected: {
            s32 idx = data->unk_1EC;
            v       = ((const u16*)&data_ov010_02129258)[idx * 8];
            arg1    = ((const Enm006PhaseRec*)&data_ov010_0212925c)[idx].unk_00;
        }
        restart:;
        }
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), v, 0);
        func_ov003_020c4ab4(data, arg1);
    }
    func_ov010_02126830(data, (void*)0x2000);
    if ((data->unk_9A - 1) % 4 == 0) {
        if (data->unk_8C == 1) {
            func_ov003_02087f00(0x1CE, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_28));
        }
    }
    if (func_ov010_0212688c(data) >= 0x2000) {
        goto bump;
    }
    // Save the position the seek was launched from, then adopt the target. The split between r0
    // and r3 across these six pairs is a register-colouring tie-break around the return value:
    // the original keeps r0 for four of them, we keep it for two. Four spellings did not move it.
    data->sprite.unk_AC                        = data->unk_28;
    data->sprite.unk_B0                        = data->unk_2C;
    data->sprite.unk_B4                        = data->unk_30;
    data->unk_28                               = data->unk_1D0;
    data->unk_2C                               = data->unk_1D4;
    data->unk_30                               = data->unk_1D8;
    data->unk_1E4                              = 0;
    data->unk_1E0                              = 0;
    data->unk_1DC                              = 0;
    ((Enm006SpriteAlt2*)&data->sprite)->unk_F0 = 0;
    return 1;
bump:
    // `= x + 1`, not `+= 1`, for the register choice.
    ((Enm006SpriteAlt2*)&data->sprite)->unk_F0 = ((Enm006SpriteAlt2*)&data->sprite)->unk_F0 + 1;
    return 0;
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
s32 func_ov010_021270a8(void* arg0, void* arg1) {
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

// Computes sample point `index` of the ten-point outline that `func_ov010_02128820` walks,
// writing it through the two out-pointers in x-then-y order. `outX` is the first: that is the
// one `func_ov010_02128624` compares against the owner's `unk_28`, and the one `func_ov010_02128820`
// reads as `toOwner.x`.
//
// The ten points only take five distinct x values and three distinct y values, all of them offset
// from either the rounded 4.12 radius `rad` or one of the two screen bounds, so the shape is a
// flat ten-sided outline rather than a circle -- the offsets do not scale with the radius.
//
// Four details are load-bearing for the codegen:
//
// - The negative-index fold is `i + 10 * (|i - 5| / 6)`, *not* a plain `i + 10`. It is what
//   produces `subs #5 / rsbmi / smull 0x2aaaaaab / mla #10`: 0x2aaaaaab is ceil(2^32/6) with no
//   trailing shift, so the divisor is six and *not* five, and the `+` (not `-`) is what keeps
//   every index inside 0..9. The 0x66666667 further down, with its `asr #2`, is the divide by
//   ten. Writing the multiply as `i - 10 * q` also costs the `mla`: it becomes `mul` + `sub`.
// - `unk_70` is 4.12 and is scaled into 16.16 by a float round trip. The `+ 0.5f` / `- 0.5f`
//   is a rounding that can never actually round, but the original spells it that way and the
//   two soft-float arms are two separate `bl`s rather than one. The `(s32)` has to sit
//   *outside* the ternary, or each arm grows its own trailing `_ffix` call. Reading the field
//   into a local is also load-bearing: named directly, the compiler keeps the shifted value in
//   a callee-saved register and spills.
// - Both screen-bound getters are called *before* the switch, and their halves are computed
//   before the jump, so they have to be plain locals the switch reads rather than expressions
//   rewritten into the arms. `midLo` is assigned before `midHi` so the two `asr #1` come out in
//   the original's order.
void func_ov010_0212847c(s32* outX, s32* outY, BtlEnm006* data, s32 index) {
    s32 i = index;
    s32 lo;
    s32 hi;
    s32 midLo;
    s32 midHi;
    s32 rad;

    if (i < 0) {
        s32 d = i - 5;
        i     = i + 10 * ((d < 0 ? -d : d) / 6);
    }
    i %= 10;

    s32 v = data->unk_70;
    rad   = (s32)(v > 0 ? (f32)(v * 0x1000) + 0.5f : (f32)(v * 0x1000) - 0.5f);

    lo    = func_ov003_020cb744(0);
    hi    = func_ov003_020cb7a4(0);
    midLo = lo >> 1;
    midHi = hi >> 1;

    switch (i) {
        case 0:
            *outX = rad;
            *outY = midHi;
            return;
        case 1:
            *outX = rad + 0x30000;
            *outY = midHi - 0x20000;
            return;
        case 2:
            *outX = midLo - 0x30000;
            *outY = midHi - 0x20000;
            return;
        case 3:
            *outX = midLo + 0x30000;
            *outY = midHi + 0x20000;
            return;
        case 4:
            *outX = lo - 0x31000 - rad;
            *outY = midHi + 0x20000;
            return;
        case 5:
            *outX = lo - 0x1000 - rad;
            *outY = midHi;
            return;
        case 6:
            *outX = lo - 0x31000 - rad;
            *outY = midHi - 0x20000;
            return;
        case 7:
            *outX = midLo + 0x30000;
            *outY = midHi - 0x20000;
            return;
        case 8:
            *outX = midLo - 0x30000;
            *outY = midHi + 0x20000;
            return;
        case 9:
            *outX = rad + 0x30000;
            *outY = midHi + 0x20000;
            return;
        default:
            return;
    }
}

// Finds the index of the nearest of ten sample points. The x-axis cull is two-sided: the
// mirror flag decides which side of the owner counts as "behind".
s32 func_ov010_02128624(BtlEnm006* data) {
    s32 bestD = 0x7FFFFFFF;
    s32 bestI = -1;
    s32 x;
    s32 y;
    s32 i;
    for (i = 0; i < 10; i++) {
        // `func_ov010_0212847c` fills x first and y second -- confirmed by `func_ov010_02128820`,
        // which reads the first out-param as `toOwner.x` against the owner's `unk_28`.
        func_ov010_0212847c(&x, &y, data, i);
        // Two sequential guards, not `else if`, and `flip` is read once: the original loads
        // unk_24 into a register after the call and tests that same register twice, so the
        // "unk_24 == 0 and in range" path falls straight through into the second test.
        s32 flip = data->unk_24;
        if (flip == 0) {
            if (x > data->unk_28) {
                continue;
            }
        }
        if (flip == 1) {
            if (x < data->unk_28) {
                continue;
            }
        }
        s32 d = func_ov003_020cba2c(data->unk_28, data->unk_2C, x, y);
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

// Sector lookup. The four halfword bounds live in the 8-byte frame, built from two base values
// and their +0x8000 copies; the loop keeps the first index whose u16-truncated distance from the
// bearing falls outside [0xAAA, 0xF6BE], so i == 4 means "no sector" and lands on the default
// block. The return value is the value just stored through `outWord` -- on the default path that
// is `unk_28` itself in the not-less case, which is why `v` is a phi and not a second load.
s32 func_ov010_021286e8(BtlEnm006* data, s32 x1, s32 y1, s16* outHalf, s32* outWord) {
    u16 sector[4];
    u16 a;
    u16 b;
    s32 angle;
    s32 i;

    angle = func_ov003_020cba14(data->unk_28, data->unk_2C, x1, y1);
    // The two bases have to be plain locals: read back out of the array, MWCC spills them to the
    // frame and reloads them, while the original keeps both in registers across the two stores.
    a         = 0x182D;
    b         = 0x67D2;
    sector[0] = a;
    sector[1] = b;
    sector[2] = a + 0x8000;
    sector[3] = b + 0x8000;

    for (i = 0; i < 4; i++) {
        // Both tests share the one truncation, and they are an `||`, not an `&&`: `cmpls r3, r2`
        // runs the second compare only when the first one did *not* break, and the shared `bhi`
        // then breaks on either the first compare's HI (0xAAA > d) or the second (d > 0xF6BE).
        u32 d = (u16)(sector[i] - angle);
        if (d < 0xAAA || d > 0xF6BE) {
            break;
        }
    }

    switch (i) {
        case 0:
            *outHalf = 1;
            *outWord = 1;
            return 1;
        case 1:
            *outHalf = 1;
            *outWord = 0;
            return 0;
        case 2:
            *outHalf = 2;
            *outWord = 0;
            return 0;
        case 3:
            *outHalf = 2;
            *outWord = 1;
            return 1;
    }
    *outHalf = 0;
    s32 v    = data->unk_28;
    if (v < x1) {
        v        = 1;
        *outWord = 1;
    } else {
        *outWord = 0;
    }
    return v;
}

// The per-frame body. On the first frame it latches the owner's own sample index, then each
// frame it reads the two sample points either side of that index, turns them into a target
// triple, mirrors the sprite from the sign of a dot product, and finally advances or clears the
// counter at 0x1F0 against the one at 0x1F2.
//
// The four sample-point outputs are the *first* thing the two `func_ov010_0212847c` calls fill,
// and every use afterwards re-loads them from the frame -- `unk_1EC` is re-read for the second
// call's index, `o1a`/`o1b` for the two stores and the two call arguments. Caching any of them
// in a variable makes MWCC park it in a callee-saved register and the loads disappear.
//
// The declaration order of the six frame locals is load-bearing: MWCC hands out the slots
// top-down in *reverse* declaration order, so `h` at 0x04 and `w` at 0x08 require the four
// points to be declared `o1a, o1b, o2a, o2b` and `w` last. Getting this backwards costs ~15%.
void func_ov010_02128820(BtlEnm006* data, s32 arg1) {
    // h is the halfword the sector lookup writes; w is the word beside it, whose address goes
    // into `sp+0` as the outgoing fifth argument. Neither is initialised in the original.
    s16 h;
    s32 o1a;
    s32 o1b;
    s32 o2a;
    s32 o2b;
    s32 w;

    if (arg1 == 0) {
        data->unk_1EC                              = func_ov010_02128624(data);
        ((Enm006SpriteAlt2*)&data->sprite)->unk_F0 = 0;
    }
    if (((Enm006SpriteAlt2*)&data->sprite)->unk_F0 != 0) {
        goto tail;
    }

    func_ov010_0212847c(&o1a, &o1b, data, data->unk_1EC);
    func_ov010_0212847c(&o2a, &o2b, data, data->unk_1EC + 1);

    if (arg1 == 0) {
        // `toOwner` is declared first so that it gets the *higher* of the two 12-byte frame
        // slots (0x28) and `edge` the lower one (0x1C) -- the reverse swaps both the stores and
        // the two pointer arguments to Vec_DotProduct. The z components are explicit zeros.
        Vec toOwner;
        Vec edge;
        toOwner.x = data->unk_28 - o1a;
        toOwner.y = data->unk_2C - o1b;
        toOwner.z = 0;
        edge.x    = o2a - o1a;
        edge.y    = o2b - o1b;
        edge.z    = 0;
        // The single `ldrb` stays ahead of the branch and feeds both arms, and only the "faces
        // left" arm is if-converted (`biclt`/`strltb`): the other is a `blt` past a block placed
        // just after it. Spelling the test as `>= 0` with the clear-bit in the `else` is what
        // makes MWCC branch instead of predicating both arms; `< 0` with the arms the other way
        // round predicates both and loses four instructions.
        if (Vec_DotProduct(&toOwner, &edge) >= 0) {
            data->unk_1F5 = (data->unk_1F5 & ~1) | 1;
        } else {
            data->unk_1F5 &= ~1;
        }
    }

    data->unk_1D0 = o1a;
    data->unk_1D4 = o1b;
    data->unk_1D8 = 0;
    func_ov010_021286e8(data, o1a, o1b, &h, &w);
    Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), h, 0);
    func_ov003_020c4ab4(data, w);
    ((Enm006SpriteAlt2*)&data->sprite)->unk_F2 = func_ov010_02128a6c(data, (void*)0x2800);

    // A bit test on the byte is a shift pair, and the inner shift has to be the *signed* one --
    // `(u32)x << 31 >> 31` folds to `and #1`, `(u32)(x << 31) >> 31` does not.
    if ((u32)(data->unk_1F5 << 31) >> 31 == 1) {
        data->unk_1EC -= 1;
    } else {
        data->unk_1EC += 1;
    }
    // Two statements, not one `= (x + 10) % 10`. Same arithmetic, but MWCC keeps the dividend in
    // r12 across the reciprocal multiply rather than in r3.
    data->unk_1EC += 10;
    data->unk_1EC %= 10;

tail:
    // Both arms return from here, so the "bump" arm has to be a forward branch to a block placed
    // last, and the clearing arm is the fall-through of the second test.
    if (((Enm006SpriteAlt2*)&data->sprite)->unk_F0 < ((Enm006SpriteAlt2*)&data->sprite)->unk_F2) {
        if (func_ov010_02128ac8(data) >= 0x2800) {
            goto bump;
        }
    }
    ((Enm006SpriteAlt2*)&data->sprite)->unk_F0 = 0;
    data->unk_1E4                              = 0;
    data->unk_1E0                              = 0;
    data->unk_1DC                              = 0;
    return;
bump:
    // `= x + 1`, not `+= 1`: the compound form picks r1 for the address, the plain one r0.
    ((Enm006SpriteAlt2*)&data->sprite)->unk_F0 = ((Enm006SpriteAlt2*)&data->sprite)->unk_F0 + 1;
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
// 2 share a block, and 0/4/5 fall through to the tail untouched.
//
// The `flag` is spelled as a plain zero plus a conditional assignment rather than one comparison
// expression: the original hoists the `mov r5, #0` up next to the first guard and sinks the
// `moveq r5, #1` down to the call, which is what two statements give and what
// `flag = (a == b)` does not.
s32 func_ov010_02126a94(BtlEnm006* data) {
    if (data->unk_1C8 != (void*)func_ov010_02125998) {
        if (func_ov003_020c3bf0() != 0) {
            return 1;
        }
    }
    s32 flag = 0;
    if (data->unk_1C8 == (void*)func_ov010_02125de4) {
        flag = 1;
    }
    switch (CombatActor_PopPendingCommand((CombatActor*)data)) {
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
            // Same shape as 02128e80's case 6: the original reuses the flags its own `orrs`
            // leaves behind, so the `|= 1` comes first and the test is on the whole word.
            data->unk_54 |= 1;
            if (data->unk_54 != 0) {
                break;
            }
            func_ov010_02125910(data, (void*)func_ov010_021263c4);
            break;
    }
    data->unk_28 += data->unk_1DC;
    data->unk_2C += data->unk_1E0;
    data->unk_30 += data->unk_1E4;
    if (data->unk_1C8 != NULL) {
        // The callee takes the instance: spelled as a nullary call, MWCC leaves the callback
        // itself in r0 and `blx r0` hands it the function pointer instead of `data`.
        ((void (*)(BtlEnm006*))data->unk_1C8)(data);
    }
    u8* p = (u8*)data->sprite.unk_88;
    // The flag is passed on to the animation helper as its increment: the original already has
    // it in r1 from the `ldrh` that tested it, and passing a literal 0 would both add a
    // `mov r1, #0` the original does not have and advance the phase by the wrong amount.
    u16 step = *(u16*)(p + 2);
    if (step != 0) {
        if ((data->sprite.unk_8C & 1) || data->unk_C8 == 0xD) {
            func_ov010_02126934(data, step);
            *(u16*)(p + 2) = 0;
        }
    }
    data->unk_54 |= 0x80000000;
    func_ov003_020c4628(data);
    return data->unk_1CC;
}

// The near-twin of func_ov010_02126a94, for the UG task. Same prologue, same tail, but the four
// stage cases do not share a block and the callback set is the UG one; cases 0/4/5 and anything
// above 6 fall straight through to the tail, so there is no `default:`.
//
// Two details are load-bearing:
//
//  - Case 6 tests the *whole* word, and does so *after* the `|= 1`. The original reuses the
//    flags its own `orrs` leaves behind (`orrs / str / bne`), and `x | 1` can never be zero,
//    so the callback arm is dead in the original binary -- that is what the disassembly says,
//    and this is the spelling that reproduces it. The reading that looks semantically right,
//    `if (unk_54 & 1) break; unk_54 |= 1;`, compiles to `tst r0, #1 / bne` plus a late
//    `orr`/`str`: one instruction more, in the wrong order, and it would call the callback.
//  - The 0x1F6 halfword is handed straight to the animation helper as its increment argument.
//    The original already has it in r1 from the `ldrh` that tested it, so the C has to pass the
//    *value* rather than a literal 0 -- a literal costs an extra `mov r1, #0` that the
//    original does not have. (Same story in 02126a94, where the flag comes from `unk_88 + 2`.)
//
// The switch discriminant is `CombatActor_PopPendingCommand`, not a `func_ov003_*` name: that
// is what `config/usa/arm9/overlays/ov003/symbols.txt` calls 0x02082f2c, and objdiff scores a
// `bl` whose two sides disagree about the symbol name.
//
// The 0x1F6 halfword itself is read through the overlapping `Enm006SpriteAlt3` view so the
// address is built as base+0x100 then +0xF6, not as one outer-struct access, and so the same
// base register can serve the following `unk_8C` load.
s32 func_ov010_02128e80(BtlEnm006* data) {
    data->unk_38 = 0;
    if (data->unk_1C8 != (void*)func_ov010_02127500) {
        if (func_ov003_020c3bf0() != 0) {
            return 1;
        }
    }
    switch (CombatActor_PopPendingCommand((CombatActor*)data)) {
        case 1:
            func_ov010_02127460(data, (void*)func_ov010_021282b8);
            data->unk_38 = 0;
            break;
        case 2:
            func_ov010_02127460(data, (void*)func_ov010_02128314);
            break;
        case 3:
            func_ov010_02127460(data, (void*)func_ov010_02128434);
            break;
        case 6:
            data->unk_54 |= 1;
            if (data->unk_54 != 0) {
                break;
            }
            func_ov010_02127460(data, (void*)func_ov010_021283c0);
            break;
    }
    data->unk_28 += data->unk_1DC;
    data->unk_2C += data->unk_1E0;
    data->unk_30 += data->unk_1E4;
    if (data->unk_1C8 != NULL) {
        ((void (*)(BtlEnm006*))data->unk_1C8)(data);
    }
    u16 flag = ((Enm006SpriteAlt3*)&data->sprite)->unk_F6;
    if (flag != 0) {
        if ((data->sprite.unk_8C & 1) || data->unk_C8 == 0xD || data->unk_C8 == 0xF) {
            func_ov010_02128d20(data, flag);
            // Re-derived, not reused from `flag`: the address has to be rebuilt after the call.
            ((Enm006SpriteAlt3*)&data->sprite)->unk_F6 = 0;
        }
    }
    data->unk_54 |= 0x80000000;
    func_ov003_020c4668(data);
    return data->unk_1CC;
}

// The Swirl stage-0 constructor. The x coordinate carries a 0x20000 bias, and the mirror case
// *normalises* it: subtract the bias, and bounce straight back if that lands exactly on zero.
// Written as an `if (x == 0x20000) x += 0x20000` it compiles to the same thing but reads worse.
//
// `02082a04` takes **seven** arguments, not six: the anim table is the fourth, and the three
// stack words are the element offset (0), the palette index and 0x30. Passing six leaves the
// callee reading an uninitialised [sp+8] and dereferencing a NULL anim table.
void func_ov010_02126d54(Enm006Swirl* data, Enm006Spawn* args) {
    MI_CpuSet(data, 0, 0xA0);
    BtlEnm006* d = args->unk_00;
    u16        v = d->unk_80;
    u32        m = (u32)((s32) * (s32*)((u8*)d + 0x84) << 30) >> 30;
    CombatSprite_LoadFromTable(m, (CombatSprite*)((u8*)data + 4), func_ov010_021256c0(v), data_ov010_021292c4, 0,
                               (u16)data_ov010_021292b0[v], 0x30);
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
        // `CombatActor_SetPendingCommand` is what config/usa/.../ov003/symbols.txt calls
        // 0x02082f1c, and objdiff scores a `bl` whose two sides disagree about the name.
        CombatActor_SetPendingCommand((CombatActor*)target, 1);
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
//
// The residue at 95.9% is all register choice: MWCC pushes two callee-saved registers where the
// original pushes three (it keeps an `r3` this version has no use for), it hoists the shared
// `unk_10` load above the mode branch, and it picks `r1`/`r2` the other way round for the zero
// and the base pointer in the mode-0 wrap. A block-scoped `s32 dx = x + data->unk_10;` temp was
// tried to block the hoist and does not; MWCC hoists the load regardless of the temp.
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
        *(s32*)((u8*)data->unk_08 + 0x28) = x + data->unk_10 + data->unk_18;
        *(s32*)((u8*)data->unk_08 + 0x2C) = y + data->unk_14;
        *(s32*)((u8*)data->unk_08 + 0x30) = data->unk_1C - 0x8000;
    } else {
        data->unk_0C->unk_28 = x + data->unk_10 + data->unk_18;
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
            CombatActor_SetPendingCommand((CombatActor*)data->unk_08, 2);
        } else {
            self = data->unk_0C;
            self->sprite.unk_8C |= 0x1000;
            func_ov003_020c4fc8(data->unk_0C);
        }
        self->unk_54 &= ~0x10000000;
    }
    return result;
}

// The DeadEff stage-0 constructor. Structurally the same as `func_ov010_02126d54` -- clear, read
// the variant and the two-bit selector out of the owner, hand them to `02082a04`, prime the
// sprite -- but the clear is 0x6C rather than 0xA0, the CombatSprite is at offset *zero*, and
// there is no mirror-bias normalisation. The flip is tested against 1, not against 0.
//
// `func_ov003_02082a04` (CombatSprite_LoadFromTable) takes **seven** arguments: r0-r3 are the
// palette mode, the sprite, the BinIdentifier and the anim table, and the three stack words at
// [sp], [sp+4], [sp+8] are the anim-table element offset, the palette index and 0x30. The
// `func_ov010_02126d54` call site spells it as a six-argument call, which is wrong.
//
// The two-bit selector is a shift *pair*, not a mask: `& 3` gives `and r7, r1, #3`. The `(s32)`
// on the inside of the `<< 30` is what keeps `lsr` rather than `asr` on the outside, exactly as
// the `<< 30 >> 31` recipe in the brief says -- MWCC only strength-reduces the unsigned form.
//
// `m` is live across the call, so it takes a callee-saved register (r7) and `v` takes r4, which
// the original then reuses for the 0x30 argument constant once `v` is dead. `data` is in r6 and
// `args` in r5 from the first instruction. `args->unk_00` is re-derived at each of its five uses
// rather than kept in a local: a local that is only *read* on a path where it is never assigned
// takes a callee-saved register and pushes everything else up one, which desynchronises the
// whole function.
//
// Residue (81.9%): one 13-instruction hunk, the `02082a04` argument setup, and the literal-pool
// order that follows from it. The original emits the anim-table pair first
// (`ldr r1, [pc] / mov r2, r0 / ldr r3, [r1, r4, lsl #2]`) and the palette-index shift pair into
// r12 (`lsl ip, r4, #1 / ldrh ip, [r1, ip]`); ours emits the zero, then the callback, then the
// palette lookup into r3/lr, then the anim table. Thirty C spellings of the call (locals for each
// argument, `register`, `const`/`u16`/`s32`/`u16*` casts, a block-scope prototype with `s32`
// tails instead of `u16`, reordering the two locals, the unprototyped implicit declaration, the
// argument spelled through a pointer local, moving one of the tail stores) all produce a
// byte-identical word stream, so this is an allocator/scheduler tie-break, not a spelling.
s32 func_ov010_02125780(Enm006DeadEff* data, Enm006Spawn* args) {
    MI_CpuSet(data, 0, 0x6C);
    u16 v = args->unk_00->unk_80;
    u32 m = (u32)((s32) * (s32*)((u8*)args->unk_00 + 0x84) << 30) >> 30;
    CombatSprite_LoadFromTable(m, (CombatSprite*)data, func_ov010_021256c0(v), data_ov010_02129380[v], 0,
                               (u16)data_ov010_02129200[v], 0x30);
    Mini108_VBlank((CombatSprite*)data, 0, 1);
    CombatSprite_SetFlip((CombatSprite*)data, args->unk_00->unk_24 == 1);
    data->unk_60 = args->unk_00->unk_28;
    data->unk_64 = args->unk_00->unk_2C;
    data->unk_68 = args->unk_00->unk_30;
    return 1;
}
