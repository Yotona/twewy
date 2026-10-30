#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020e0c44;

/// The task parameter: the shot to fire, the heading and speed it leaves with, and the point it turns toward.
typedef struct Ov003EnemyShotFocusArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ u16                   angle;
    /* 0x06 */ u16                   reserved06;
    /* 0x08 */ s32                   speed;
    /* 0x0C */ s32                   spread; // deceleration, also sets how long the shot flies straight
    /* 0x10 */ s32                   accel;  // speed it picks up after turning toward the target
    /* 0x14 */ s32                   targetX;
    /* 0x18 */ s32                   targetY;
    /* 0x1C */ s32                   targetZ;
} Ov003EnemyShotFocusArgs; // Size: 0x20

typedef struct Ov003EnemyShotFocus {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ CombatSprite         sprite;
    /* 0x8C */ s16                  phase;
    /* 0x8E */ s16                  turnTimer; // frames of slowing down left before the turn
    /* 0x90 */ s32                  velocityX;
    /* 0x94 */ s32                  velocityY;
    /* 0x98 */ s32                  brakeX;
    /* 0x9C */ s32                  brakeY;
    /* 0xA0 */ s32                  targetX;
    /* 0xA4 */ s32                  targetY;
    /* 0xA8 */ s32                  targetZ;
    /* 0xAC */ s32                  accel;
} Ov003EnemyShotFocus; // Size: 0xB0

s32 func_ov003_020ce39c(Ov003EnemyShotSource* source, u16 angle, s32 speed, s32 spread, s32 accel, s32 targetX, s32 targetY,
                        s32 targetZ) {
    Ov003EnemyShotFocusArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source  = source;
    args.angle   = angle;
    args.speed   = speed;
    args.spread  = spread;
    args.accel   = accel;
    args.targetX = targetX;
    args.targetY = targetY;
    args.targetZ = targetZ;
    TaskPool* pool;
    if (source->core.owner->base.sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0c44, NULL, 0, NULL, &args);
}

/// Fires two fans of three focus shots, one on each side of the owner, each shot starting a step further out.
void func_ov003_020ce448(Ov003EnemyShotSource* source, s32* ids, s32 offsetX, s32 offsetY, s32 speed, s32 spread, s32 accel,
                         s32 targetX, s32 targetY, s32 targetZ) {
    s32 savedX = source->core.position.x;
    s32 savedY = source->core.position.y;
    u16 base   = (source->core.owner->base.sprite.flags46 & 1) ? 0 : 0x8000;
    u16 angle;
    s32 i;

    angle = base + 0x800;
    for (i = 0; i < 3; i++) {
        source->core.position.x += FX_Mul(FX_CosIdx(angle), offsetX);
        source->core.position.y += FX_Mul(FX_Mul(FX_SinIdx(angle), offsetX), offsetY);
        if (ids != NULL)
            *ids++ = func_ov003_020ce39c(source, (u16)((base ^ 0x8000) + (base - angle)), speed, spread, accel, targetX,
                                         targetY, targetZ);
        else
            func_ov003_020ce39c(source, (u16)((base ^ 0x8000) + (base - angle)), speed, spread, accel, targetX, targetY,
                                targetZ);
        source->core.position.x = savedX;
        source->core.position.y = savedY;
        angle += 0x1000;
    }
    angle = base - 0x800;
    for (i = 0; i < 3; i++) {
        source->core.position.x += FX_Mul(FX_CosIdx(angle), offsetX);
        source->core.position.y += FX_Mul(FX_Mul(FX_SinIdx(angle), offsetX), offsetY);
        if (ids != NULL)
            *ids++ = func_ov003_020ce39c(source, (u16)((base ^ 0x8000) + (base - angle)), speed, spread, accel, targetX,
                                         targetY, targetZ);
        else
            func_ov003_020ce39c(source, (u16)((base ^ 0x8000) + (base - angle)), speed, spread, accel, targetX, targetY,
                                targetZ);
        source->core.position.x = savedX;
        source->core.position.y = savedY;
        angle -= 0x1000;
    }
}

s32 func_ov003_020ce7d4(Ov003EnemyShotFocus* self, Ov003EnemyShotFocusArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    s32                         engine = args->source->core.owner->base.sprite.sprite.bits_0_1;
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->source, sizeof(self->source));
    self->velocityX = FX_Mul(FX_CosIdx(args->angle), args->speed);
    self->velocityY = FX_Mul(FX_SinIdx(args->angle), args->speed);
    self->brakeX    = FX_Mul(FX_CosIdx(args->angle), args->spread);
    self->brakeY    = FX_Mul(FX_SinIdx(args->angle), args->spread);
    self->turnTimer = args->speed / args->spread;
    self->accel     = args->accel;
    self->targetX   = args->targetX;
    self->targetY   = args->targetY;
    self->targetZ   = args->targetZ;
    CombatSprite_LoadDirect(engine, &self->sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                            entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, self->source.animation, self->source.animationMode);
    self->source.core.sprite = &self->sprite;
    func_ov003_020cdfbc(&self->source, args->source->core.owner->base.sprite.flags46 & 1, 0);
    func_ov003_020cccf0(engine, &self->source);
    return 1;
}

s32 func_ov003_020ce99c(Ov003EnemyShotFocus* self) {
    s16 x, y;
    s16 targetX, targetY;
    s32 engine = func_ov003_020c37f8(&self->sprite) ? 1 : 0;
    if (func_ov003_020ccb9c(&self->source, &self->sprite))
        return 0;
    func_ov003_020cdfbc(&self->source, 0, 1);
    switch (self->phase) {
        case 0:
            self->velocityX -= self->brakeX;
            self->velocityY -= self->brakeY;
            self->turnTimer--;
            if (self->turnTimer < 0) {
                func_ov003_02084348(engine, &x, &y, self->source.core.position.x, self->source.core.position.y,
                                    self->source.core.position.z);
                func_ov003_02084348(engine, &targetX, &targetY, self->targetX, self->targetY, self->targetZ);
                u16 angle    = (u16)(FX_Atan2Idx((s32)targetY - y, (s32)targetX - x) >> 4);
                self->brakeX = FX_Mul(self->accel, FX_CosIdx(angle));
                self->brakeY = FX_Mul(self->accel, FX_SinIdx(angle));
                self->phase++;
            }
            break;
        case 1:
            self->velocityX += self->brakeX;
            self->velocityY += self->brakeY;
            break;
    }
    self->source.core.position.x += self->velocityX;
    self->source.core.position.y += self->velocityY;
    func_ov003_02084348(engine, &x, &y, self->source.core.position.x, self->source.core.position.y,
                        self->source.core.position.z);
    self->source.core.hitResult = func_ov003_0208a1a4(func_ov003_0208a114(self->source.core.attackId),
                                                      &self->source.core.owner->base.actor.baseArgs, x, y);
    CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020ceb9c(Ov003EnemyShotFocus* self) {
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.core.position.x,
                        self->source.core.position.y, self->source.core.position.z);
    CombatSprite_SetPosition(&self->sprite, x, y);
    func_ov003_02082730(&self->sprite, 0x7FFFFFFF - self->source.core.position.y);
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020cec14(Ov003EnemyShotFocus* self) {
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020cec28(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotFocus* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020ce7d4(self, args);
        case 1:
            return func_ov003_020ce99c(self);
        case 2:
            return func_ov003_020ceb9c(self);
        case 3:
            return func_ov003_020cec14(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6c60[24] = "Tsk_BtlEnmShot_Focus";
const TaskHandle data_ov003_020e0c44     = {data_ov003_020e6c60, func_ov003_020cec28, sizeof(Ov003EnemyShotFocus)};
