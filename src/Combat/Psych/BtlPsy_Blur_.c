#include "Combat/Core/CombatPsychEffects.h"

extern const TaskStages data_ov003_020dcaf4;

extern const TaskHandle data_ov003_020dcae8;

void func_ov003_020b6978(s32 animation, s32 x, s32 y, s32 z, s32 angle) {
    Ov003ShockWaveBlurArgs args;
    args.animation  = animation;
    args.position.x = x;
    args.position.y = y;
    args.position.z = z;
    args.angle      = angle;
    func_ov003_020a6000(&data_ov003_020dcae8, &args);
}

s32 func_ov003_020b69b0(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020dcaf4;
    return stages.iter[stage](pool, task, args);
}

char             data_ov003_020e4f0c[16] = "Tsk_BtlPsy_Blur";
const TaskHandle data_ov003_020dcae8     = {data_ov003_020e4f0c, func_ov003_020b69b0, sizeof(Ov003ShockWaveBlur)};
