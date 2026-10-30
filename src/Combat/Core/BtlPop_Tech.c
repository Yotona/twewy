#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"

#include <nitro/mi/cpumem.h>

// The technique-name popup: a task that floats the tech id of the last used psych above the
// main actor. The value is split into its decimal digits (`func_ov003_0209d088`), each digit
// picks a sprite animation, and the popup rises towards the actor before awarding the exp.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The popup's task data (0x80 bytes). Digits are stored least significant first and rendered
/// right to left, each as the animation index `digit + 0x14`.
typedef struct BtlPop_Tech {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s16          digits[4];
    /* 0x68 */ s16          digitCount;
    /* 0x6A */ s16          techId;
    /* 0x6C */ s32          x; // world position of the main actor when the popup spawned
    /* 0x70 */ s32          y;
    /* 0x74 */ s32          z; // current height, interpolated towards `targetZ`
    /* 0x78 */ s32          targetZ;
    /* 0x7C */ s16          state;
    /* 0x7E */ s16          timer;
} BtlPop_Tech; // Size: 0x80

/// The popup task is created with a single `s16` tech id (see `func_ov003_0209d4b0`); this is the
/// view the stage functions read it through.
typedef struct BtlPop_Tech_Args {
    /* 0x0 */ s16 techId;
} BtlPop_Tech_Args;

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern void func_020265d4(s32* dst, s32 target, u16 weight);

extern const TaskHandle    data_ov003_020d813c;
extern const TaskStages    data_ov003_020d8148;
extern const BinIdentifier data_ov003_020d7800;

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Splits `value` (clamped to 0..9999) into decimal digits, least significant first, and returns
/// how many digits are needed to show it.
s32 func_ov003_0209d088(s32* digits, s32 value) {
    s32 v = value;
    s32 rest;

    if (v < 0) {
        v = 0;
    }
    if (v > 9999) {
        v = 9999;
    }

    digits[3] = v / 1000;
    rest      = v % 1000;
    digits[2] = rest / 100;
    rest      = rest % 100;
    digits[1] = rest / 10;
    digits[0] = rest % 10;

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

/// Loads the popup's sprite (the "tech name" plate) and starts it on its first animation.
void func_ov003_0209d154(CombatSprite* sprite, s32 variant) {
    CombatSprite_LoadDirectNoProto(0, sprite, &data_ov003_020d7800, 0x25, 0x27, 0x26, 0x3D, variant);
    func_ov003_02082738(sprite, 0);
    func_ov003_02082730(sprite, 1);
}

s32 func_ov003_0209d1b4(TaskPool* pool, Task* task, void* args) {
    BtlPop_Tech*      data   = task->data;
    BtlPop_Tech_Args* params = args;
    s32               digits[4];
    s32               i;

    MI_CpuSet(data, 0, sizeof(BtlPop_Tech));

    data->techId     = params->techId;
    data->digitCount = func_ov003_0209d088(digits, params->techId);
    data->x          = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x;
    data->y          = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.y;
    data->z          = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.z -
              (((CombatActor*)data_ov003_020e71b8->unk3D898)->unk_76 << 12);
    data->targetZ = data->z - 0x20000;
    data->state   = 0;
    data->timer   = 0;

    func_ov003_0209d154(&data->sprite, 0);
    for (i = 0; i < 4; i++) {
        data->digits[i] = (s16)(digits[i] + 0x14);
    }
    return 1;
}

s32 func_ov003_0209d284(TaskPool* pool, Task* task, void* args) {
    BtlPop_Tech* data = task->data;

    switch (data->state) {
        case 0:
            if (data->timer == 0) {
                data->timer = 30;
            }
            func_020265d4(&data->z, data->targetZ, (u16)data->timer);
            data->timer--;
            if (data->timer <= 0) {
                data->state++;
                data->timer = 0;
            }
            break;

        case 1:
            if (data->timer == 0) {
                data->timer = 8;
            }
            data->timer--;
            if (data->timer <= 0) {
                func_ov003_0208afcc(data->techId);
                return 0;
            }
            break;
    }
    return 1;
}

// Nonmatching: pure register allocation -- `posY`, `half` and `right` take r9/r6/r8 instead of
// the retail build's r6/r7/r9, a rotation of the same callee-saved set.
s32 func_ov003_0209d334(TaskPool* pool, Task* task, void* args) {
    BtlPop_Tech* data = task->data;
    s32          posX = func_ov003_020843b0(0, data->x);
    s16          posY = (s16)(func_ov003_020843ec(0, data->y, data->z) - 8);
    s32          half = (data->digitCount * 7) / 2 - 7;
    s32          offset;
    s32          i;
    s32          right;

    CombatSprite_SetAnim(&data->sprite, 0x35, 0);
    CombatSprite_Update(&data->sprite);

    right = half + posX;
    CombatSprite_SetPosition(&data->sprite, (s16)(right - (data->digitCount - 1) * 7), posY);
    CombatSprite_Render(&data->sprite);

    offset = 0;
    for (i = 0; i < data->digitCount; i++) {
        CombatSprite_SetAnim(&data->sprite, data->digits[i], 0);
        CombatSprite_Update(&data->sprite);
        CombatSprite_SetPosition(&data->sprite, (s16)(right - offset), posY);
        CombatSprite_Render(&data->sprite);
        offset += 7;
    }
    return 1;
}

s32 func_ov003_0209d434(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

/// Task dispatcher: calls the stage function for `stage`, which is a no-op for a popup that
/// finished this frame.
s32 func_ov003_0209d448(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8148;

    if (func_ov003_0208b690((u16)stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Creates the popup task if a tech was used this frame. The task parameter is a bare `s16` tech
/// id (a `BtlPop_Tech_Args`), passed by address.
void func_ov003_0209d4b0(void) {
    s16 techId;

    if (data_ov003_020e71b8->unk3D8FA <= 0) {
        return;
    }
    techId = data_ov003_020e71b8->unk3D8FA;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d813c, NULL, 0, NULL, &techId);
    data_ov003_020e71b8->unk3D8FA = 0;
}

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

char data_ov003_020e3cfc[0x10] = "Tsk_BtlPop_Tech";

const TaskStages data_ov003_020d8148 = {
    .initialize = func_ov003_0209d1b4,
    .update     = func_ov003_0209d284,
    .render     = func_ov003_0209d334,
    .cleanup    = func_ov003_0209d434,
};

const TaskHandle data_ov003_020d813c = {data_ov003_020e3cfc, func_ov003_0209d448, sizeof(BtlPop_Tech)};
