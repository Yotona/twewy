#include "Combat/Core/CombatPsychEffects.h"

#include "Combat/Core/CombatApi.h"

extern const TaskHandle data_ov003_020dc760;
extern const TaskStages data_ov003_020dc76c;

// Nonmatching: coordinate-load scheduling and redundant halfword narrowing differ.
s32 func_ov003_020acb74(Ov003ChargeLaser* self, Ov003EnemyActor* unit) {
    s32 x = func_ov003_020843b0(0, self->x);
    if (func_ov003_020a6568(&self->effect->streak, 0x16D, unit, x, 0, 0) == 1) {
        Ov003PsychEffectArgs effect;
        if (unit->actor.flags & 0x20000000)
            return 1;
        s32 x = unit->actor.screenX;
        if (x < self->x)
            x += unit->actor.unk_70 / 2;
        else
            x -= unit->actor.unk_70 / 2;
        func_ov003_020a8908(&effect, (s16)x, (s16)self->y, 2, self->animationKind + 0xEE);
        if (unit->actor.screenX < self->x)
            func_ov003_020a89e4(&effect);
        func_ov003_020a899c(&effect, 1);
        func_ov003_020a88a8(&effect);
        return 1;
    }
    return 0;
}

void func_ov003_020acc60(Ov003ChargeLaser* self) {
    Ov003EnemyActor* unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    while (unit != NULL) {
        s32 distance = unit->actor.screenY - unit->actor.unk_76 - self->y;
        if (distance < 0)
            distance = -distance;
        if (distance <= unit->actor.unk_70 + 16) {
            if ((!self->facingLeft && unit->actor.screenX > self->x) || (self->facingLeft && unit->actor.screenX < self->x))
                func_ov003_020acb74(self, unit);
        }
        unit = func_ov003_02084984(&unit->node);
    }
}

// Nonmatching: the task's initializer ignores its argument; pass NULL instead of a stack placeholder.
void func_ov003_020acd04(void) {
    s32 args[4];

    func_ov003_020a6000(&data_ov003_020dc760, args);
}

s32 func_ov003_020acd24(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020dc76c;
    return stages.iter[stage](pool, task, args);
}

char             data_ov003_020e4b90[16] = "Tsk_BtlPsy_Blur";
const TaskHandle data_ov003_020dc760     = {data_ov003_020e4b90, func_ov003_020acd24, sizeof(Ov003Blur)};
const TaskStages data_ov003_020dc76c     = {
    {func_ov003_020acd6c, func_ov003_020acdc0, func_ov003_020ace54, func_ov003_020acebc}
};
