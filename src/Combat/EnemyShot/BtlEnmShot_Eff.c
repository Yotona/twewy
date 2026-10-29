#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

// Retail evaluates separate random draws for the sign and rounded coordinate.
#define EFFECT_RANDOM_OFFSET()                                                                                 \
    ((RNG_Next(12) - 6) > 0 ? (f32)((RNG_Next(12) - 6) << 12) + 0.5f : (f32)((RNG_Next(12) - 6) << 12) - 0.5f)

extern s32                 func_ov003_020c3c28(void);
extern s32                 func_ov003_020c37f8(CombatSprite*);
extern void                func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern const BinIdentifier data_ov003_020e0c30;
extern const TaskHandle    data_ov003_020e0c38;
Ov003EnemyShotEffectCounts data_ov003_020e7340;

s32 func_ov003_020cde74(CombatSprite* source, s32 x, s32 y, s32 z, s32 flip, s32 kind) {
    if (kind == 1 && data_ov003_020e7340.activeParticles > 48)
        return -1;
    Ov003EnemyShotEffectArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source     = source;
    args.position.x = x;
    args.position.y = y;
    args.position.z = z;
    args.flip       = flip;
    args.kind       = kind;
    TaskPool* pool;
    if (source->sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0c38, NULL, 0, NULL, &args);
}

s32 func_ov003_020cdf34(u32 flags, s32 x, s32 y, s32 z, CombatSprite* source, const s16* timer, s32 flip, s32 kind) {
    switch (kind) {
        case 0:
            if (flags & 0x8000)
                return func_ov003_020cde74(source, x, y, z, flip, kind);
            break;
        case 1:
            if ((flags & 0x4000) && *timer % 4 == 0)
                return func_ov003_020cde74(source, x, y, z, 0, kind);
            break;
    }
    return -1;
}

s32 func_ov003_020cdfbc(Ov003EnemyShotSource* source, s32 flip, s32 kind) {
    return func_ov003_020cdf34(source->core.flags, source->core.position.x, source->core.position.y, source->core.position.z,
                               source->core.sprite, &source->core.timer, flip, kind);
}

s32 func_ov003_020cdff4(Ov003EnemyShotCore* source, s32 flip, s32 kind) {
    return func_ov003_020cdf34(source->flags, source->position.x, source->position.y, source->position.z, source->sprite,
                               &source->timer, flip, kind);
}

s32 func_ov003_020ce02c(Ov003EnemyShotEffect* self, Ov003EnemyShotEffectArgs* args) {
    u16 animation;
    MI_CpuSet(self, 0, sizeof(*self));
    self->position.x = args->position.x;
    self->position.y = args->position.y;
    self->position.z = args->position.z;
    self->kind       = args->kind;
    switch (args->kind) {
        case 0:
            animation = 0;
            CombatSprite_LoadDirect(args->source->sprite.bits_0_1, &self->sprite, &data_ov003_020e0c30, 1, 3, 2, 0, 0);
            break;
        case 1:
            animation = RNG_Next(2);
            CombatSprite_LoadDirect(args->source->sprite.bits_0_1, &self->sprite, &data_ov003_020e0c30, 4, 6, 5, 0, 0);
            self->position.x = args->position.x + (s32)EFFECT_RANDOM_OFFSET();
            self->position.y = args->position.y + (s32)EFFECT_RANDOM_OFFSET();
            data_ov003_020e7340.activeParticles++;
            break;
    }
    Sprite_ChangePalette(&self->sprite.sprite, args->source->sprite.bits_0_1, args->source->sprite.unk3C, 5, 1);
    self->sprite.basePalette       = self->sprite.sprite.paletteData;
    self->sprite.basePaletteSource = self->sprite.sprite.unk3C;
    CombatSprite_SetAnim(&self->sprite, animation, 1);
    if (args->kind == 0)
        CombatSprite_SetFlip(&self->sprite, args->flip);
    return 1;
}

s32 func_ov003_020ce24c(Ov003EnemyShotEffect* self) {
    if (func_ov003_020c3c28())
        return 0;
    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
        return 0;
    CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020ce288(Ov003EnemyShotEffect* self) {
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->position.x, self->position.y,
                        self->position.z);
    CombatSprite_SetPosition(&self->sprite, x, y);
    s32 offset = (self->position.y + (self->kind == 1)) ? -10 : 10;
    func_ov003_02082730(&self->sprite, 0x7FFFFFFF - offset);
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020ce318(Ov003EnemyShotEffect* self) {
    CombatSprite_Release(&self->sprite);
    if (self->kind == 1)
        data_ov003_020e7340.activeParticles--;
    return 1;
}

s32 func_ov003_020ce34c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotEffect* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020ce02c(self, args);
        case 1:
            return func_ov003_020ce24c(self);
        case 2:
            return func_ov003_020ce288(self);
        case 3:
            return func_ov003_020ce318(self);
        default:
            return 0;
    }
}

char                data_ov003_020e6c2c[20] = "Tsk_BtlEnmShot_Eff";
char                data_ov003_020e6c40[32] = "Apl_Hor/Grp_BtlEnmShot_Eff.bin";
const BinIdentifier data_ov003_020e0c30     = {3, data_ov003_020e6c40};
const TaskHandle    data_ov003_020e0c38     = {data_ov003_020e6c2c, func_ov003_020ce34c, sizeof(Ov003EnemyShotEffect)};
