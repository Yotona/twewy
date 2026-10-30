#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
#include <nitro/mi/cpumem.h>

extern const BinIdentifier data_ov003_020d6fac;
extern char                data_ov003_020e3ad4[12];

s32 func_ov003_0208ee20(TaskPool* pool, Task* task, void* args) {
    BtlIcon* icon = task->data;

    MI_CpuSet(icon, 0, sizeof(*icon));
    icon->unk_6C    = 0;
    icon->animState = 0;
    func_ov003_0208ecb4(icon, NULL);
    CombatSprite_LoadDirect(2, &icon->sprite, &data_ov003_020d6fac, 1, 3, 2, 4, 0);
    CombatSprite_SetAnim(&icon->sprite, 0, 0);
    func_ov003_02082738(&icon->sprite, 0);
    func_ov003_02082730(&icon->sprite, 0x3F2000);
    return 1;
}

s32 func_ov003_0208eebc(TaskPool* pool, Task* task, void* args) {
    BtlIcon* icon = task->data;

    func_ov003_0208ecbc(icon);
    if (icon->update != NULL) {
        icon->update(icon);
    }
    if (icon->unk_6C != 0) {
        icon->x = data_ov003_020e71b8->touchWork.x;
        icon->y = data_ov003_020e71b8->touchWork.y;
        CombatSprite_Update(&icon->sprite);
    }
    return 1;
}

s32 func_ov003_0208ef28(TaskPool* pool, Task* task, void* args) {
    BtlIcon* icon = task->data;

    if (icon->unk_6C != 0) {
        CombatSprite_SetPosition(&icon->sprite, icon->x, icon->y);
        CombatSprite_Render(&icon->sprite);
    }
    return 1;
}

s32 func_ov003_0208ef5c(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

extern const TaskStages data_ov003_020d78fc;

s32 func_ov003_0208ef70(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d78fc;
    return stages.iter[stage](pool, task, args);
}

const TaskHandle data_ov003_020d78f0 = {data_ov003_020e3ad4, func_ov003_0208ef70, sizeof(BtlIcon)};
const TaskStages data_ov003_020d78fc = {
    func_ov003_0208ee20,
    func_ov003_0208eebc,
    func_ov003_0208ef28,
    func_ov003_0208ef5c,
};

extern char data_ov003_020e3ac4[16];
char*       data_ov003_020e3ac0     = data_ov003_020e3ac4;
char        data_ov003_020e3ac4[16] = "Seq_Battle()";
