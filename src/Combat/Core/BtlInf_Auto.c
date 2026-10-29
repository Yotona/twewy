#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"

#include "Combat/Core/CombatApi.h"
extern const BinIdentifier data_ov003_020d7800;

s32 func_ov003_0209dcdc(TaskPool* pool, Task* task, void* args) {
    CombatSprite* sprite = task->data;

    CombatSprite_LoadDirect(1, sprite, &data_ov003_020d7800, 1, 3, 2, 0x3F, 0);
    CombatSprite_SetPosition(sprite, 0x80, 0xB8);
    func_ov003_02082738(sprite, 0);
    return 1;
}

s32 func_ov003_0209dd48(TaskPool* pool, Task* task, void* args) {
    CombatSprite* sprite = task->data;
    if (data_ov003_020e71b8->unk3D878 & 0x10) {
        CombatSprite_Update(sprite);
    }
    return 1;
}

s32 func_ov003_0209dd78(TaskPool* pool, Task* task, void* args) {
    CombatSprite* sprite = task->data;
    if (data_ov003_020e71b8->unk3D878 & 0x10) {
        CombatSprite_Render(sprite);
    }
    return 1;
}

s32 func_ov003_0209dda8(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

extern const TaskStages data_ov003_020d819c;
s32                     func_ov003_0209ddbc(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d819c;
    if (func_ov003_0208b690((u16)stage)) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

char             data_ov003_020e3d2c[16] = "Tsk_BtlInf_Auto";
const TaskHandle data_ov003_020d8190     = {data_ov003_020e3d2c, func_ov003_0209ddbc, sizeof(CombatSprite)};
const TaskStages data_ov003_020d819c     = {
    func_ov003_0209dcdc,
    func_ov003_0209dd48,
    func_ov003_0209dd78,
    func_ov003_0209dda8,
};
