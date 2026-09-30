#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Tutorial/BtlTutorial.h"
#include "Engine/EasyTask.h"

#include <nitro/mi/cpumem.h>

extern void func_ov003_02083ab0(s32, s32, s32, s32);
extern s32  func_ov003_0208b690(u16);

s32  func_ov008_020e7374(TaskPool*, Task*, void*);
s32  func_ov008_020e742c(TaskPool*, Task*, void*);
s32  func_ov008_020e7464(TaskPool*, Task*, void*);
s32  func_ov008_020e746c(TaskPool*, Task*, void*);
s32  func_ov008_020e7474(TaskPool*, Task*, void*, s32);
void func_ov008_020e7360(BtlPlayerNone*, void (*)(BtlPlayerNone*));

const TaskHandle Tsk_BtlPlayerNone = {"Tsk_BtlPlayerNone", func_ov008_020e7474, 0x84};

static const TaskStages data_ov008_020e7a10 = {
    .initialize = func_ov008_020e7374,
    .update     = func_ov008_020e742c,
    .render     = func_ov008_020e7464,
    .cleanup    = func_ov008_020e746c,
};

void func_ov008_020e7360(BtlPlayerNone* data, void (*callback)(BtlPlayerNone*)) {
    data->unk_7C = callback;
    data->unk_80 = 0;
    data->unk_82 = 0;
}

s32 func_ov008_020e7374(TaskPool* pool, Task* task, void* args) {
    BtlPlayerNone* data = task->data;

    MI_CpuSet(data, 0, sizeof(BtlPlayerNone));
    CombatActor_Init(&data->actor, 0);
    data_ov003_020e71b8->unk3D89C = data;
    data->actor.isFlipped         = FALSE;
    data->actor.position.x        = data_ov003_020e71b8->unk3D7C0[1].unk_20;
    data->actor.position.y        = data_ov003_020e71b8->unk3D7C0[1].unk_24;
    data->actor.position.z        = 0;
    data->actor.zGravity          = 0x800;
    data->actor.unk_70            = 0xC;
    data->actor.unk_72            = 0x30;
    data->actor.unk_76            = 0x18;
    data->actor.flags |= 0x10;
    func_ov003_02083ab0(1, data->actor.position.x, data->actor.position.y, data->actor.position.z);
    func_ov008_020e7360(data, NULL);
    return 1;
}

s32 func_ov008_020e742c(TaskPool* pool, Task* task, void* args) {
    BtlPlayerNone* data = task->data;

    if (data->unk_7C != NULL) {
        data->unk_7C(data);
    }
    CombatActor_UpdateEffects(1, &data->actor);
    CombatActor_UpdatePhysics(&data->actor);
    return 1;
}

s32 func_ov008_020e7464(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov008_020e746c(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov008_020e7474(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov008_020e7a10;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}
