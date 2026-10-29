#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32 func_ov003_0208442c(s32, s32);
extern s32 func_ov003_02084460(s32, s32, s32);
extern s32 func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);

extern const TaskHandle data_ov003_020dc62c;

s32 func_ov003_020a88a8(Ov003PsychEffectArgs* args) {
    return func_ov003_020a6000(&data_ov003_020dc62c, args);
}

void func_ov003_020a88c0(Ov003PsychEffectArgs* args, s32 x, s32 y, s32 z, s32 engine, s32 kind) {
    MI_CpuSet(args, 0, sizeof(*args));
    args->position.x = x;
    args->position.y = y;
    args->position.z = z;
    args->engine     = engine;
    args->unk_18     = 1;
    args->kind       = kind;
}

void func_ov003_020a8908(Ov003PsychEffectArgs* args, s32 x, s32 y, s32 engine, s32 kind) {
    MI_CpuSet(args, 0, sizeof(*args));
    args->flags |= 0x400;
    args->position.x = x;
    args->position.y = y;
    args->position.z = 0x1000;
    args->engine     = engine;
    args->unk_18     = 1;
    args->kind       = kind;
}

void func_ov003_020a8958(Ov003PsychEffectArgs* args, CombatActor* actor, s32 engine, s32 kind) {
    MI_CpuSet(args, 0, sizeof(*args));
    args->flags |= 0x10;
    args->actor  = actor;
    args->engine = engine;
    args->unk_18 = 1;
    args->kind   = kind;
}

void func_ov003_020a899c(Ov003PsychEffectArgs* args, s32 sortKey) {
    args->flags |= 8;
    args->sortKey = sortKey;
}

void func_ov003_020a89b0(Ov003PsychEffectArgs* args, s32 lifetime) {
    args->flags |= 1;
    args->opacity = lifetime << 12;
}

void func_ov003_020a89c8(Ov003PsychEffectArgs* args, s32 angle, s32 scaleX, s32 scaleY) {
    args->flags |= 2;
    args->angle  = angle;
    args->scaleX = scaleX;
    args->scaleY = scaleY;
}

void func_ov003_020a89e4(Ov003PsychEffectArgs* args) {
    args->flags |= 4;
}

void func_ov003_020a89f4(Ov003PsychEffectArgs* args, s32 lifetime, s32 value) {
    args->flags |= 0x21;
    args->opacity     = lifetime;
    args->opacityStep = value;
}

void func_ov003_020a8a0c(Ov003PsychEffectArgs* args, s32 x, s32 y, s32 z) {
    args->flags |= 0x40;
    args->velocity.x = x;
    args->velocity.y = y;
    args->velocity.z = z;
}

void func_ov003_020a8a28(Ov003PsychEffectArgs* args, u16 value) {
    args->flags |= 0x100;
    args->unk_50 = value;
}

s32 func_ov003_020a8a3c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychEffectArgs* arg = args;
    Ov003PsychAnimation*  anim;
    switch (stage) {
        default:
        case 0: {
            u16 autoFinish;
            anim = task->data;
            MI_CpuSet(anim, 0, sizeof(*anim));
            anim->flags    = arg->flags;
            anim->position = arg->position;
            if ((arg->flags & 0x400) && !(arg->flags & 0x800)) {
                anim->position.x = func_ov003_0208442c(0, (s16)anim->position.x);
                anim->position.y = func_ov003_02084460(0, (s16)anim->position.y, anim->position.z);
            }
            if (anim->flags & 0x10)
                anim->actor = arg->actor;
            if (anim->flags & 1) {
                anim->opacity     = arg->opacity;
                anim->opacityStep = arg->opacityStep;
            }
            if (anim->flags & 0x40) {
                anim->velocity     = arg->velocity;
                anim->acceleration = arg->unk_44;
            }
            if (anim->flags & 8)
                anim->sortKey = arg->sortKey;
            if (anim->flags & 2) {
                anim->angle  = arg->angle;
                anim->scaleX = arg->scaleX;
                anim->scaleY = arg->scaleY;
            }
            autoFinish = 1;
            if (anim->flags & 0x100) {
                anim->timer = arg->unk_50;
                autoFinish  = 0;
            }
            func_ov003_020a3498(&anim->sprite, arg->engine, arg->unk_18, arg->kind, autoFinish);
            return 1;
        }
        case 1:
            anim = task->data;
            if (anim->flags & 0x100) {
                anim->timer--;
                if (anim->timer == 0)
                    return 0;
            } else if (SpriteMgr_IsAnimationFinished(&anim->sprite.sprite))
                return 0;
            if (anim->flags & 0x80) {
                if (anim->flags & 0x40) {
                    Vec_Add(&anim->position, &anim->velocity, &anim->position);
                    Vec_Add(&anim->velocity, &anim->acceleration, &anim->velocity);
                }
                if (anim->flags & 0x20) {
                    anim->opacity += anim->opacityStep;
                    if (anim->opacity > 0x1F000)
                        anim->opacity = 0x1F000;
                    else if (anim->opacity < 0x1000)
                        anim->opacity = 0x1000;
                }
            }
            if ((anim->flags & 0x10) && (anim->actor->flags & 4)) {
                anim->position.x = anim->actor->position.x;
                anim->position.y = anim->actor->position.y;
                anim->position.z = anim->actor->position.z;
                anim->flags &= ~0x10;
            }
            CombatSprite_Update(&anim->sprite);
            anim->flags |= 0x80;
            return 1;
        case 2: {
            Ov003PsychAnimation* anim = task->data;
            s32                  y, x;
            if (anim->flags & 0x10) {
                x = func_ov003_020843b0(0, anim->actor->position.x);
                y = func_ov003_020843ec(0, anim->actor->position.y, anim->actor->position.z);
                func_ov003_02082730(&anim->sprite, anim->actor->position.y + 0x1F4000);
            } else if (anim->flags & 0x800) {
                x = (s16)anim->position.x;
                y = (s16)anim->position.y;
                func_ov003_02082730(&anim->sprite, (func_ov003_02084460(0, y, anim->position.z) << 12) + 0x384000);
            } else {
                x = func_ov003_020843b0(0, anim->position.x);
                y = func_ov003_020843ec(0, anim->position.y, anim->position.z);
                func_ov003_02082730(&anim->sprite, anim->position.y + 0x1F4000);
            }
            CombatSprite_SetPosition(&anim->sprite, x, y);
            if (anim->flags & 8)
                func_ov003_02082730(&anim->sprite, (anim->sortKey << 12) + 0x384000);
            if (anim->flags & 2) {
                s32 scaleY            = anim->scaleY;
                s32 scaleX            = anim->scaleX;
                anim->affine.rotation = anim->angle;
                if (anim->sprite.flags46 & 1)
                    scaleX = -scaleX;
                anim->affine.scaleX               = scaleX;
                anim->affine.scaleY               = scaleY;
                anim->affine.unk_0C               = 0;
                anim->affine.unk_0E               = 0;
                anim->sprite.affine               = &anim->affine;
                anim->sprite.sprite.unk_0A.unk_00 = 1;
            }
            if (anim->flags & 4)
                CombatSprite_SetFlip(&anim->sprite, 1);
            if (anim->flags & 1)
                CombatSprite_RenderWithPalette(&anim->sprite, anim->opacity >> 12);
            else
                CombatSprite_Render(&anim->sprite);
            return 1;
        }
        case 3:
            anim = task->data;
            CombatSprite_Release(&anim->sprite);
            return 1;
    }
}

char             data_ov003_020e49a8[20] = "CmnTsk_BtlPsy_Anm";
const TaskHandle data_ov003_020dc62c     = {data_ov003_020e49a8, func_ov003_020a8a3c, sizeof(Ov003PsychAnimation)};
