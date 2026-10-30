#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"

#include <nitro/mi/cpumem.h>

// The star-info panel: a small HUD sprite cluster that mirrors the charge-command state held at
// `Ov003Global + 0x3D8E8` -- the current charge step, its progress and the length of the next
// step -- as two-digit number pairs plus a filled/empty star once the step sequence is done.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The panel's task data (0x74 bytes). All the odd offsets below are sprite animation indices;
/// `func_ov003_0209d5ac` turns a number into the `digit + 2` pair used by the two digits.
typedef struct BtlInf_Star {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s16          starAnim;   // star sprite animation: 0 = filled, 1 = empty
    /* 0x62 */ s16          stepAnim;   // current charge step as an animation index
    /* 0x64 */ s16          progressLo; // progress digits, least significant first
    /* 0x66 */ s16          progressHi;
    /* 0x68 */ s16          nextLo;     // length of the next step, least significant first
    /* 0x6A */ s16          nextHi;
    /* 0x6C */ s8           step;       // last seen charge step
    /* 0x6D */ s8           progress;   // last seen progress
    /* 0x6E */ s8           next;       // last seen step length
    /* 0x6F */ char         unk_6F;
    /* 0x70 */ s32          complete;   // 1 once the step counter reached the step count
} BtlInf_Star;                          // Size: 0x74

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern const TaskStages    data_ov003_020d8164;
extern const BinIdentifier data_ov003_020d7800;

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Splits `value` (clamped to 0..99) into two decimal digits, least significant first, and
/// returns how many digits are needed to show it.
s32 func_ov003_0209d51c(s32* digits, s32 value) {
    s32 v = value;

    if (v < 0) {
        v = 0;
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

void func_ov003_0209d570(CombatSprite* sprite, u16 anim, s32 posX, s32 posY) {
    CombatSprite_SetAnim(sprite, anim, 0);
    CombatSprite_SetPosition(sprite, posX, posY);
    CombatSprite_Update(sprite);
    CombatSprite_Render(sprite);
}

/// Writes the two animation indices for `value`'s decimal digits to `digits`.
void func_ov003_0209d5ac(s16* digits, s32 value) {
    s32 split[2];

    func_ov003_0209d51c(split, value);
    digits[0] = (s16)(split[0] + 2);
    digits[1] = (s16)(split[1] + 2);
}

/// Refreshes the panel from the charge-command state; each element is only reloaded when its
/// source value changed.
void func_ov003_0209d5e0(BtlInf_Star* data) {
    Ov003EffectCmd* charge = data_ov003_020e71b8->effectCmd;

    if (data->step != charge->current) {
        data->step     = charge->current;
        data->stepAnim = (s16)(data->step + 2);
        if (data->step >= charge->count) {
            data->complete = 1;
            data->starAnim = 1;
        } else {
            data->complete = 0;
            data->starAnim = 0;
        }
    }
    if (charge->progress != data->progress) {
        data->progress = charge->progress;
        func_ov003_0209d5ac(&data->progressLo, data->progress);
    }
    if (charge->next != data->next) {
        data->next = charge->next;
        func_ov003_0209d5ac(&data->nextLo, data->next);
    }
}

/// Whether the charge-command task asked for the HUD to be hidden this frame.
s32 func_ov003_0209d688(void) {
    if (data_ov003_020e71b8->effectCmd->flags & 1) {
        return 1;
    }
    return 0;
}

s32 func_ov003_0209d6b0(TaskPool* pool, Task* task, void* args) {
    BtlInf_Star* data = task->data;

    MI_CpuSet(data, 0, sizeof(BtlInf_Star));
    data->step     = 0;
    data->progress = 0;
    data->next     = 0;
    data->complete = 0;

    CombatSprite_LoadDirect(1, &data->sprite, &data_ov003_020d7800, 0x19, 0x1B, 0x1A, 0x3F, 0);
    func_ov003_02082738(&data->sprite, 0);

    data->starAnim = 0;
    data->stepAnim = (s16)(data->step + 2);
    func_ov003_0209d5ac(&data->progressLo, data->progress);
    func_ov003_0209d5ac(&data->nextLo, data->next);
    return 1;
}

s32 func_ov003_0209d758(TaskPool* pool, Task* task, void* args) {
    BtlInf_Star* data = task->data;

    if (func_ov003_0209d688() != 0) {
        return 1;
    }
    func_ov003_0209d5e0(data);
    return 1;
}

s32 func_ov003_0209d780(TaskPool* pool, Task* task, void* args) {
    BtlInf_Star* data = task->data;

    if (func_ov003_0209d688() != 0) {
        return 1;
    }
    if (data->complete == 0) {
        func_ov003_0209d570(&data->sprite, data->progressHi, 0x6E, 1);
        func_ov003_0209d570(&data->sprite, data->progressLo, 0x73, 1);
        func_ov003_0209d570(&data->sprite, data->nextHi, 0x7E, 1);
        func_ov003_0209d570(&data->sprite, data->nextLo, 0x83, 1);
    }
    func_ov003_0209d570(&data->sprite, data->stepAnim, 0x96, 1);
    func_ov003_0209d570(&data->sprite, data->starAnim, 0x80, 0);
    return 1;
}

s32 func_ov003_0209d824(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

/// Task dispatcher: calls the stage function for `stage`, which is a no-op for a panel that
/// finished this frame.
s32 func_ov003_0209d838(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8164;

    if (func_ov003_0208b690((u16)stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

char data_ov003_020e3d0c[0x10] = "Tsk_BtlInf_Star";

const TaskStages data_ov003_020d8164 = {
    .initialize = func_ov003_0209d6b0,
    .update     = func_ov003_0209d758,
    .render     = func_ov003_0209d780,
    .cleanup    = func_ov003_0209d824,
};

const TaskHandle data_ov003_020d8158 = {data_ov003_020e3d0c, func_ov003_0209d838, sizeof(BtlInf_Star)};
