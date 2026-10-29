#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

// The damage/heal number popup: a task that floats a signed amount above the actor it hit.
// The amount is split into up to five decimal digits (`func_ov003_0209af30`), each digit is
// drawn with the animation of its decimal value offset by the element's base animation, and
// the popup rises to its apex before falling back and expiring.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// Task data of the number popup (`Tsk_BtlPop_Num`, 0x94 bytes).
typedef struct BtlPop_Num {
    /* 0x00 */ s32          engine;    // 0 = main screen, 1 = sub screen
    /* 0x04 */ CombatSprite sprite;
    /* 0x64 */ s16          digits[5]; // decimal digits, least significant first, already
                                       // offset by the element's animation base
    /* 0x6E */ s16 critAnim;           // animation drawn right of the number on a critical hit
    /* 0x70 */ s16 elementAnim;        // animation drawn left of the number for the element
    /* 0x74 */ s32 x;
    /* 0x78 */ s32 y;
    /* 0x7C */ s32 z;
    /* 0x80 */ s32 zVelocity; // per-frame z step, halved once the apex is reached
    /* 0x84 */ s32 xVelocity;
    /* 0x88 */ s32 yVelocity;
    /* 0x8C */ s8  digitCount;
    /* 0x8D */ u8  flags; // bit 0: crit icon, bit 1: element icon
    /* 0x8E */ s16 timer; // frames spent alive
    /* 0x90 */ s32 active;
} BtlPop_Num;             // Size: 0x94

/// The damage record the popup task is created with; `func_ov003_0209b564`/`b61c` fill it in.
typedef struct BtlPop_NumArgs {
    /* 0x00 */ s32 engine;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ s32 value;    // amount shown (a negative amount is a hit)
    /* 0x14 */ u8  element;  // element animation base, 0..3
    /* 0x15 */ u8  subType;  // non-zero: draw the element icon
    /* 0x18 */ s32 critical; // non-zero: draw the critical icon
} BtlPop_NumArgs;            // Size: 0x1C

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern s32 func_ov003_020843b0(s32 engine, s32 x);
extern s32 func_ov003_020843ec(s32 engine, s32 y, s32 z);

extern const BinIdentifier data_ov003_020d7800;
extern const TaskHandle    data_ov003_020d7ffc;
extern const TaskStages    data_ov003_020d8008;

/// The retail call sites pass the `s32` animation variant straight through
/// `CombatSprite_LoadDirect`'s `u16` parameter (no prototype was in scope there).
#define CombatSprite_LoadDirectNoProto                                                                     \
    ((void (*)(s32, CombatSprite*, const BinIdentifier*, s32, s32, s32, s32, s32))CombatSprite_LoadDirect)

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

s32 func_ov003_0209b0a0(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209b294(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209b318(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209b498(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209b4fc(TaskPool* pool, Task* task, void* args, s32 stage);

/// Splits `value` (clamped to 0..99999) into decimal digits, least significant first, and
/// returns how many digits are needed to show it. A value of -1 stores a lone `-` glyph.
s32 func_ov003_0209af30(s32* digits, s32 value) {
    s32 v = value;
    s32 rest;

    if (v == -1) {
        digits[0] = 30;
        return 1;
    }
    if (v > 99999) {
        v = 99999;
    }

    rest      = v % 10000;
    digits[4] = v / 10000;
    digits[3] = rest / 1000;
    rest      = rest % 1000;
    digits[2] = rest / 100;
    rest      = rest % 100;
    digits[1] = rest / 10;
    digits[0] = rest % 10;

    if (v >= 10000) {
        return 5;
    }
    if (v >= 1000) {
        return 4;
    }
    if (v >= 100) {
        return 3;
    }
    if (v >= 10) {
        return 2;
    }
    return 1;
}

/// Loads the popup sprite and starts it on the digit animation set of `engine`.
void func_ov003_0209b04c(s32 engine, CombatSprite* sprite, s32 arg2, s32 variant) {
    CombatSprite_LoadDirectNoProto(engine, sprite, &data_ov003_020d7800, 0x25, 0x27, 0x26, variant, arg2);
    func_ov003_02082738(sprite, 0);
    func_ov003_02082730(sprite, 1);
}

/// Popup initialiser: splits the amount into digits, scatters its launch velocities and picks
/// the element's animation base for the digits and the two decoration icons.
s32 func_ov003_0209b0a0(TaskPool* pool, Task* task, void* args) {
    BtlPop_Num*     data = task->data;
    BtlPop_NumArgs* p    = args;
    s32             digits[5];
    s32             offset = 0;
    s32             animIndex;
    s32             i;

    MI_CpuSet(data, 0, sizeof(BtlPop_Num));
    data->active     = 1;
    data->flags      = 0;
    data->digitCount = func_ov003_0209af30(digits, p->value);
    data->engine     = p->engine;
    data->zVelocity  = RNG_Next(0x4CE) - 0x2333;
    data->x          = p->x;
    data->y          = p->y;
    data->z          = p->z;
    data->xVelocity  = RNG_Next(0x199B) - 0xCCD;
    data->yVelocity  = RNG_Next(0xCCD) - 0x666;

    switch (p->element) {
        case 0:
            offset = 0;
            break;
        case 1:
            offset = 10;
            break;
        case 2:
            offset = 20;
            break;
        case 3:
            offset = 30;
            break;
    }

    if (data->engine != 1) {
        animIndex                     = 0x3D;
        data_ov003_020e71b8->unk3D8FC = data_ov003_020e71b8->unk3D8FC + 1;
    } else {
        animIndex                     = 0x3F;
        data_ov003_020e71b8->unk3D8FD = data_ov003_020e71b8->unk3D8FD + 1;
    }
    func_ov003_0209b04c(data->engine, &data->sprite, 0, animIndex);

    for (i = 0; i < data->digitCount; i++) {
        data->digits[i] = (s16)(offset + digits[i]);
    }

    if (p->element != 3) {
        if (p->critical != 0) {
            data->flags |= 1;
            data->critAnim = (s16)((p->element * 3) + 0x28);
        }
        if (p->subType != 0) {
            data->flags |= 2;
            data->elementAnim = (s16)(p->subType + ((p->element * 3) + 0x28));
        }
    }
    return 1;
}

/// Per-frame motion: rise until the apex (z reaches 0), then drift along the scatter velocity
/// while the popup counts down its lifetime.
s32 func_ov003_0209b294(TaskPool* pool, Task* task, void* args) {
    BtlPop_Num* data = task->data;

    data->zVelocity += 0x4CD;
    data->z += data->zVelocity;
    if (data->z >= 0) {
        data->z         = 0;
        data->zVelocity = -data->zVelocity / 2;
    } else {
        data->x += data->xVelocity;
        data->y += data->yVelocity;
    }
    if (data->timer >= 0x2A) {
        data->active = 0;
    } else {
        data->timer++;
    }
    return data->active;
}

/// Draws the digits of the number above the actor, plus the two optional icons.
// Nonmatching: `posY`, `half` and the loop base sit in a rotated register triple
// (target r6/r8/r9, ours r9/r6/r8); the instruction sequence is otherwise identical.
s32 func_ov003_0209b318(TaskPool* pool, Task* task, void* args) {
    BtlPop_Num* data = task->data;
    s32         posX;
    s32         screenY;
    s16         posY;
    s32         offset;
    s32         i;
    s32         half;

    if (data->engine != 1) {
        posX    = func_ov003_020843b0(0, data->x);
        screenY = func_ov003_020843ec(0, data->y, data->z);
    } else {
        posX    = func_ov003_020843b0(1, data->x);
        screenY = func_ov003_020843ec(1, data->y, data->z);
    }
    posY = screenY - 8;
    half = (data->digitCount * 7) / 2 - 7;

    for (i = 0, offset = 0; i < data->digitCount; i++, offset += 7) {
        CombatSprite_SetAnim(&data->sprite, data->digits[i], 0);
        CombatSprite_Update(&data->sprite);
        CombatSprite_SetPosition(&data->sprite, (s16)(half + posX - offset), posY);
        CombatSprite_Render(&data->sprite);
    }
    if (data->flags & 2) {
        CombatSprite_SetAnim(&data->sprite, data->elementAnim, 0);
        CombatSprite_Update(&data->sprite);
        CombatSprite_SetPosition(&data->sprite, (s16)(half + posX - ((data->digitCount - 1) * 7)), posY);
        CombatSprite_Render(&data->sprite);
    }
    if (data->flags & 1) {
        CombatSprite_SetAnim(&data->sprite, data->critAnim, 0);
        CombatSprite_Update(&data->sprite);
        CombatSprite_SetPosition(&data->sprite, (s16)(half + posX + 7), posY);
        CombatSprite_Render(&data->sprite);
    }
    return 1;
}

s32 func_ov003_0209b498(TaskPool* pool, Task* task, void* args) {
    BtlPop_Num* data = task->data;

    CombatSprite_Release(&data->sprite);
    if (data->engine != 1) {
        data_ov003_020e71b8->unk3D8FC = data_ov003_020e71b8->unk3D8FC - 1;
    } else {
        data_ov003_020e71b8->unk3D8FD = data_ov003_020e71b8->unk3D8FD - 1;
    }
    return 1;
}

/// Task dispatcher: calls the stage function for `stage`, which is a no-op for a popup that
/// finished this frame.
s32 func_ov003_0209b4fc(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8008;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns a number popup for `value` above `actor` on `engine`'s screen, unless ten popups of
/// that screen are already alive.
void func_ov003_0209b564(s32 engine, s32 value, CombatActor* actor, s32 element) {
    BtlPop_NumArgs p;
    s32            count;

    if (engine != 1) {
        count = data_ov003_020e71b8->unk3D8FC;
    } else {
        count = data_ov003_020e71b8->unk3D8FD;
    }
    // At most ten popups may be alive per screen. The original materialises the test into a
    // value before switching on it, so this is a `switch` rather than an `if`.
    switch (count < 10) {
        case 0:
            return;
    }

    p.x        = actor->position.x + (actor->unk_74 << 12);
    p.y        = actor->position.y;
    p.z        = actor->position.z - (actor->unk_76 << 12);
    p.value    = value;
    p.engine   = engine;
    p.element  = element;
    p.subType  = 0;
    p.critical = 0;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d7ffc, NULL, 0, NULL, &p);
}

/// Spawns a number popup for the damage `value` dealt to `actor`, picking the element and the
/// critical/subelement decorations out of the actor's hit flags.
// Nonmatching: the two trailing argument stores and the `unk_12` load are one register pair
// apart from the target (p.engine/p.value store order and r0/r1); logic and instruction
// multiset are identical.
void func_ov003_0209b61c(s32 engine, s32 value, CombatActor* actor) {
    BtlPop_NumArgs p;
    s32            count;

    if (engine != 1) {
        count = data_ov003_020e71b8->unk3D8FC;
    } else {
        count = data_ov003_020e71b8->unk3D8FD;
    }
    // At most ten popups may be alive per screen (see func_ov003_0209b564).
    switch (count < 10) {
        case 0:
            return;
    }

    p.x = actor->position.x + (actor->unk_74 << 12);
    if (value < 0) {
        value = -value;
    }
    p.y      = actor->position.y;
    p.z      = actor->position.z - (actor->unk_76 << 12);
    p.value  = value;
    p.engine = engine;

    // bits 2..3: element, bits 4..5: element variant, bit 1: critical hit
    p.subType  = ((u32)(actor->unk_12 << 28) >> 30) == 0 ? 0 : (u8)((u32)(actor->unk_12 << 26) >> 30);
    p.critical = ((u32)(actor->unk_12 << 30) >> 31) == 1 ? 1 : 0;
    switch ((u32)(actor->unk_12 << 28) >> 30) {
        case 1:
            p.element = 1;
            break;
        case 2:
            p.element = 0;
            break;
        case 3:
            p.element = 2;
            break;
        default:
            p.element = 0;
            break;
    }
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d7ffc, NULL, 0, NULL, &p);
}

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

char data_ov003_020e3c90[0x10] = "Tsk_BtlPop_Num";

const TaskHandle data_ov003_020d7ffc = {data_ov003_020e3c90, func_ov003_0209b4fc, sizeof(BtlPop_Num)};

const TaskStages data_ov003_020d8008 = {
    func_ov003_0209b0a0,
    func_ov003_0209b294,
    func_ov003_0209b318,
    func_ov003_0209b498,
};
