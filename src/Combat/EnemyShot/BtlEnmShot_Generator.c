#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Combat/Core/CombatList.h"
#include <nitro/mi/cpumem.h>

// The generator owner: an enemy actor block that also carries the shot emitter state.
typedef struct Ov003EnemyShotGenerator {
    /* 0x000 */ Ov003EnemyActor      actor;
    /* 0x1C0 */ Ov003EnemyShotCore   core;        // shot state of the generator itself
    /* 0x1DC */ Ov003EnemyShotSource paramSource; // the shot it emits
    /* 0x208 */ u16                  angle;       // rotation of the emitted shot
    /* 0x20A */ u16                  angleStep;   // added to `angle` per shot
    /* 0x20C */ s16                  shotSpeed;   // per-shot speed
    /* 0x20E */ u16                  delayFrames; // re-fire delay
    /* 0x210 */ s8                   delayTimer;
    /* 0x211 */ s8                   phase;
    /* 0x212 */ s16                  spreadStep;  // per-frame offset along x while it moves out
    /* 0x214 */ s16                  speedYStep;  // per-frame offset along y while it moves out
    /* 0x216 */ s16                  spreadDecay; // per-frame decay of `spreadStep`
    /* 0x218 */ s16                  speedYDecay; // per-frame decay of `speedYStep`
    /* 0x21A */ s16                  speedScale;  // per-shot scale of `speedYStep`
} Ov003EnemyShotGenerator;                        // Size: 0x21C

typedef struct Ov003EnemyShotGeneratorArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ u16                   moveAngle;
    /* 0x06 */ u16                   reserved06;
    /* 0x08 */ s32                   moveSpeed;
    /* 0x0C */ s32                   moveDecay;
    /* 0x10 */ Ov003EnemyShotSource* paramSource;
    /* 0x14 */ u16                   angle;
    /* 0x16 */ u16                   angleStep;
    /* 0x18 */ s32                   shotSpeed;
    /* 0x1C */ u16                   delayFrames;
    /* 0x1E */ s16                   lifetime;
    /* 0x20 */ s32                   speedScale;
} Ov003EnemyShotGeneratorArgs; // Size: 0x24

extern u32              func_ov003_02082f2c(void);
extern const TaskHandle data_ov003_020e0c50;

s32 func_ov003_020cec78(Ov003EnemyShotSource* source, u16 moveAngle, s32 moveSpeed, s32 moveDecay,
                        Ov003EnemyShotSource* paramSource, u16 angle, u16 angleStep, s32 shotSpeed, u16 delayFrames,
                        s16 lifetime, s32 speedScale) {
    Ov003EnemyShotGeneratorArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source      = source;
    args.moveAngle   = moveAngle;
    args.moveSpeed   = moveSpeed;
    args.moveDecay   = moveDecay;
    args.paramSource = paramSource;
    args.angle       = angle;
    args.angleStep   = angleStep;
    args.shotSpeed   = shotSpeed;
    args.delayFrames = delayFrames;
    args.lifetime    = lifetime;
    args.speedScale  = speedScale;
    source->core.flags |= 0x40;
    TaskPool* pool;
    if (source->core.owner->base.sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0c50, NULL, 0, NULL, &args);
}

s32 func_ov003_020ced48(Ov003EnemyShotGenerator* self, Ov003EnemyShotGeneratorArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    CombatSprite*               sprite = &self->actor.sprite;
    s32                         engine = args->source->core.owner->base.sprite.sprite.bits_0_1;
    MI_CpuSet(self, 0, sizeof(*self));
    func_ov003_020cc8b0(&self->core, args->source);
    MI_CpuCopyU8(args->paramSource, &self->paramSource, sizeof(Ov003EnemyShotSource));
    self->angle       = args->angle;
    self->angleStep   = args->angleStep;
    self->shotSpeed   = args->shotSpeed;
    self->delayFrames = args->delayFrames;
    self->speedScale  = args->speedScale;
    func_ov003_020c41e8((Ov003CutinActor*)self);
    self->actor.actor.unk_70    = *(s8*)&args->source->core.owner->base.actor.unk_1C;
    self->actor.actor.unk_72    = *((s8*)&args->source->core.owner->base.actor.unk_1C + 1);
    self->actor.actor.maxHp     = 1;
    self->actor.actor.currentHp = 1;
    if (engine == 0)
        func_ov003_020c49c8(&self->actor);
    else
        func_ov003_020c4988(&self->actor);
    self->spreadStep  = FX_Mul(FX_CosIdx(args->moveAngle), args->moveSpeed);
    self->speedYStep  = FX_Mul(FX_SinIdx(args->moveAngle), args->moveSpeed);
    self->spreadDecay = FX_Mul(FX_CosIdx(args->moveAngle), args->moveDecay);
    self->speedYDecay = FX_Mul(FX_SinIdx(args->moveAngle), args->moveDecay);
    self->core.timer  = args->lifetime;
    CombatSprite_LoadDirect(engine, sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex, entry->frameDataIndex,
                            visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(sprite, args->source->animation, args->source->animationMode);
    self->core.sprite = sprite;
    func_ov003_020cdff4(&self->core, self->spreadStep > 0, 0);
    func_ov003_020cccf0(engine, args->source);
    return 1;
}

s32 func_ov003_020cef74(Ov003EnemyShotGenerator* self) {
    if (data_ov003_020e71b8->unk3D878 & 0x40)
        return 1;
    switch (func_ov003_02082f2c()) {
        default:
            break;
        case 1:
        case 3:
        case 4:
        case 5:
            return 0;
    }
    if (func_ov003_020ccc00(&self->core, &self->actor.sprite))
        return 0;
    func_ov003_020cdff4(&self->core, 0, 1);
    switch (self->phase) {
        case 0:
            self->core.position.x += self->spreadStep;
            if (self->spreadStep > 0) {
                self->spreadStep -= self->spreadDecay;
                if (self->spreadStep < 0)
                    self->spreadStep = 0;
            } else if (self->spreadStep < 0) {
                self->spreadStep -= self->spreadDecay;
                if (self->spreadStep > 0)
                    self->spreadStep = 0;
            }
            self->core.position.y += self->speedYStep;
            if (self->speedYStep > 0) {
                self->speedYStep -= self->speedYDecay;
                if (self->speedYStep < 0)
                    self->speedYStep = 0;
            } else if (self->speedYStep < 0) {
                self->speedYStep -= self->speedYDecay;
                if (self->speedYStep > 0)
                    self->speedYStep = 0;
            }
            if (self->spreadStep == 0 && self->speedYStep == 0) {
                self->delayTimer = 0;
                self->phase++;
            }
            break;
        case 1:
            self->delayTimer--;
            if (self->delayTimer < 0) {
                self->delayTimer                  = self->delayFrames;
                self->paramSource.core.position.x = self->core.position.x;
                self->paramSource.core.position.y = self->core.position.y;
                self->paramSource.core.position.z = self->core.position.z;
                self->spreadStep                  = FX_Mul(FX_CosIdx(self->angle), self->shotSpeed);
                self->speedYStep                  = FX_Mul(FX_SinIdx(self->angle), self->shotSpeed);
                self->speedYStep                  = FX_Mul(self->speedYStep, self->speedScale);
                func_ov003_020d1ec4(&self->paramSource, self->spreadStep, self->speedYStep, 0, 1);
                self->angle += self->angleStep;
            }
            break;
    }
    self->actor.actor.position.x = self->core.position.x;
    self->actor.actor.position.y = self->core.position.y;
    self->actor.actor.position.z = self->core.position.z;
    CombatSprite_Update(&self->actor.sprite);
    return 1;
}

s32 func_ov003_020cf228(Ov003EnemyShotGenerator* self) {
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->actor.sprite) ? 1 : 0, &x, &y, self->core.position.x, self->core.position.y,
                        self->core.position.z);
    CombatSprite_SetPosition(&self->actor.sprite, x, y);
    func_ov003_02082730(&self->actor.sprite, 0x7FFFFFFF - self->core.position.y);
    CombatSprite_Render(&self->actor.sprite);
    return 1;
}

s32 func_ov003_020cf2a0(Ov003EnemyShotGenerator* self) {
    self->actor.flags18C = (self->actor.flags18C | 0x40) & ~0x20;
    self->actor.actor.flags |= 4;
    func_ov003_020c4a08((Ov003CutinActor*)self);
    if (data_ov003_020e71b8->unk3D8A0 == self)
        data_ov003_020e71b8->unk3D8A0 = NULL;
    CombatSprite_Release(&self->actor.sprite);
    return 1;
}

s32 func_ov003_020cf2fc(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotGenerator* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020ced48(self, args);
        case 1:
            return func_ov003_020cef74(self);
        case 2:
            return func_ov003_020cf228(self);
        case 3:
            return func_ov003_020cf2a0(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6c78[28] = "Tsk_BtlEnmShot_Generator";
const TaskHandle data_ov003_020e0c50     = {data_ov003_020e6c78, func_ov003_020cf2fc, sizeof(Ov003EnemyShotGenerator)};
