#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"

// The star/score popup: a short-lived task that shows a number (1..99) as star-digit sprites over
// the unit it belongs to, rising slightly while it fades out.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// Task data of the star popup (`Tsk_BtlPop_Star`, 0x78 bytes).
typedef struct BtlPop_Star {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ u16          digits[2];  // digit sprite indices, least significant first
    /* 0x64 */ u16          starAnim;   // star sprite drawn over the last digit
    /* 0x66 */ s16          digitCount; // 1 or 2
    /* 0x68 */ s16          x;
    /* 0x6A */ s16          y;
    /* 0x6C */ s32          riseOffset; // subpixel rise, eased towards -0xC000
    /* 0x70 */ u16          pad_70;
    /* 0x72 */ s16          timer;
    /* 0x74 */ s32          active;
} BtlPop_Star; // Size: 0x78

/// The payload `func_ov003_0209d038` hands to the task.
typedef struct BtlPop_StarArgs {
    /* 0x00 */ s16 value; // amount shown
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
} BtlPop_StarArgs; // Size: 0x6

// ---------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

char data_ov003_020e3cec[0x10] = "Tsk_BtlPop_Star";

const TaskHandle data_ov003_020d8120 = {data_ov003_020e3cec, func_ov003_0209cff0, 0x78};

const TaskStages data_ov003_020d812c = {
    .initialize = func_ov003_0209cdd4,
    .update     = func_ov003_0209ce80,
    .render     = func_ov003_0209cef0,
    .cleanup    = func_ov003_0209cfdc,
};

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern const BinIdentifier data_ov003_020d7800;
extern void                func_020265d4(s32* dst, s32 target, u16 weight);

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Splits `value` (clamped to 0..99) into decimal digits, least significant first, and returns
/// how many digits are needed. A value of -1 stores a lone `-` glyph.
s32 func_ov003_0209cd0c(s32* digits, s32 value) {
    s32 v = value;

    if (v == -1) {
        digits[0] = 30;
        return 1;
    }
    if (v > 99) {
        v = 99;
    }
    digits[1] = v / 10;
    digits[0] = v % 10;
    if (v >= 10) {
        return 2;
    }
    return 1;
}

/// Loads the popup sprite on the star-digit animation set and parks it under the number.
void func_ov003_0209cd74(CombatSprite* sprite, s32 variant) {
    CombatSprite_LoadDirectNoProto(1, sprite, &data_ov003_020d7800, 0x25, 0x27, 0x26, 0x3F, variant);
    func_ov003_02082738(sprite, 0);
    func_ov003_02082730(sprite, 1);
}

s32 func_ov003_0209cdd4(TaskPool* pool, Task* task, void* args) {
    BtlPop_Star*     data = task->data;
    BtlPop_StarArgs* p    = args;
    s32              digits[2];
    s32              i;

    MI_CpuSet(data, 0, sizeof(BtlPop_Star));
    data->digitCount = (s16)func_ov003_0209cd0c(digits, p->value);
    data->x          = p->x;
    data->y          = p->y;
    data->riseOffset = 0;
    data->pad_70     = 0;
    data->timer      = 30;
    data->active     = 1;
    func_ov003_0209cd74(&data->sprite, 0);
    data->starAnim = 0x31;
    for (i = 0; i < data->digitCount; i++) {
        data->digits[i] = (u16)(digits[i] + 20);
    }
    return 1;
}

s32 func_ov003_0209ce80(TaskPool* pool, Task* task, void* args) {
    BtlPop_Star* data = task->data;

    if (data_ov003_020e71b8->unk3D878 & 6) {
        return 0;
    }
    if (data->timer > 0) {
        func_020265d4(&data->riseOffset, -0xC000, (u16)data->timer);
        data->timer--;
    }
    if (data->timer <= 0) {
        data->active = 0;
    }
    return data->active;
}

// Nonmatching: `posY`, `half` and the loop offset end up in r9/r6/r8 instead of r6/r8/r9;
// every instruction and operand is otherwise identical (pure register allocation).
s32 func_ov003_0209cef0(TaskPool* pool, Task* task, void* args) {
    BtlPop_Star* data = task->data;
    s16          posY;
    s32          posX;
    s32          half;
    s32          i;
    s32          offset;

    posY = (s16)(data->y + (data->riseOffset >> 12));
    posX = data->x;
    half = (data->digitCount * 7) / 2 - 7;

    for (i = 0, offset = 0; i < data->digitCount; i++, offset += 7) {
        CombatSprite_SetAnim(&data->sprite, data->digits[i], 0);
        CombatSprite_Update(&data->sprite);
        CombatSprite_SetPosition(&data->sprite, (s16)(half + posX - offset), posY);
        CombatSprite_Render(&data->sprite);
    }
    CombatSprite_SetAnim(&data->sprite, data->starAnim, 0);
    CombatSprite_Update(&data->sprite);
    CombatSprite_SetPosition(&data->sprite, (s16)(half + posX - ((data->digitCount - 1) * 7)), posY);
    CombatSprite_Render(&data->sprite);
    return 1;
}

s32 func_ov003_0209cfdc(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

/// Task dispatcher: calls the stage function for `stage`.
s32 func_ov003_0209cff0(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d812c;

    return stages.iter[stage](pool, task, args);
}

/// Spawns a star popup showing `value` at (`x`, `y`).
s32 func_ov003_0209d038(s32 value, s16 x, s32 y) {
    BtlPop_StarArgs p;

    p.value = (s16)value;
    p.x     = x;
    p.y     = (s16)y;
    return EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d8120, NULL, 0, NULL, &p);
}
