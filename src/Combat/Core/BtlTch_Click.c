#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlTchClick {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s32          active;
} BtlTchClick; // Size: 0x64

extern const BinIdentifier data_ov003_020d6fac;

s32 func_ov003_0208efb8(TaskPool* pool, Task* task, void* args) {
    BtlTchClick*   effect = task->data;
    BtlTouchInput* input  = &data_ov003_020e71b8->touchInput;

    MI_CpuSet(effect, 0, sizeof(*effect));
    effect->active = 1;
    CombatSprite_LoadDirect(2, &effect->sprite, &data_ov003_020d6fac, 1, 3, 2, 4, 1);
    if (input->tapCount < 1) {
        CombatSprite_SetAnim(&effect->sprite, 1, 1);
    } else if (input->tapCount < 5) {
        CombatSprite_SetAnim(&effect->sprite, 2, 1);
    } else {
        CombatSprite_SetAnim(&effect->sprite, 3, 1);
    }
    CombatSprite_SetPosition(&effect->sprite, input->previousX, input->previousY);
    func_ov003_02082738(&effect->sprite, 0);
    func_ov003_02082730(&effect->sprite, 0x3F2000);
    return 1;
}

s32 func_ov003_0208f098(TaskPool* pool, Task* task, void* args) {
    BtlTchClick* effect = task->data;

    if (SpriteMgr_IsAnimationFinished(&effect->sprite.sprite)) {
        effect->active = 0;
    }
    CombatSprite_Update(&effect->sprite);
    return effect->active;
}

s32 func_ov003_0208f0c4(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Render(task->data);
    return 1;
}

s32 func_ov003_0208f0d8(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

extern const TaskStages data_ov003_020d7918;
s32                     func_ov003_0208f0ec(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7918;
    return stages.iter[stage](pool, task, args);
}

// The preceding TU's descriptor references the first string in this TU.
char             data_ov003_020e3ad4[12] = "Task_BtlTch";
char             data_ov003_020e3ae0[20] = "Tsk_BtlTch_Click";
const TaskHandle data_ov003_020d790c     = {data_ov003_020e3ae0, func_ov003_0208f0ec, sizeof(BtlTchClick)};
const TaskStages data_ov003_020d7918     = {
    func_ov003_0208efb8,
    func_ov003_0208f098,
    func_ov003_0208f0c4,
    func_ov003_0208f0d8,
};
