#include "Combat/Core/CombatCutin.h"

extern const TaskStages data_ov003_020dd844;
extern const TaskHandle data_ov003_020dd838;

s32 func_ov003_020be658(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov003_020be660(TaskPool* pool, Task* task, void* args) {
    Ov003CutinTask* self = task->data;
    func_ov003_020be194(self);
    func_ov003_020be100(self);
    func_ov003_020be20c(self);
    return 1;
}

s32 func_ov003_020be688(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020dd844;
    return stages.iter[stage](pool, task, args);
}

s32 func_ov003_020be6d0(TaskPool* pool) {
    Ov003CutinArgs args;
    args.kind    = 2;
    args.partner = data_ov003_020e71b8->unk3D88D;
    return EasyTask_CreateTask(pool, &data_ov003_020dd838, NULL, 0, NULL, &args);
}

char data_ov003_020e533c[16] = "Tsk_BtlCutIn00";
