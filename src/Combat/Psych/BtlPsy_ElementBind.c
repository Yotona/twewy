#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32                          func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern const u8                     data_ov003_020dc87c[4];
extern const s32                    data_ov003_020dc880[2];
extern const Ov003ElementBindSounds data_ov003_020dc894[2];

extern const TaskHandle data_ov003_020dc888;

s32 func_ov003_020af75c(Ov003EffectState* effect, Ov003EnemyActor* unit, s32 kind, s32 attackId) {
    Ov003ElementBindArgs args;
    args.effect   = effect;
    args.unit     = unit;
    args.kind     = kind;
    args.attackId = attackId;
    func_ov003_020a6000(&data_ov003_020dc888, &args);
    return 1;
}

// Nonmatching: sound descriptor words are one array instead of three cross-indexed objects.
s32 func_ov003_020af790(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003ElementBindArgs* arg = args;
    Ov003ElementBind*     self;
    switch (stage) {
        default:
        case 0: {
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect   = arg->effect;
            self->unit     = arg->unit;
            self->kind     = arg->kind;
            self->attackId = arg->attackId;
            self->timer    = 0;
            func_ov003_020a3498(&self->sprite, 2, 1, data_ov003_020dc880[self->kind], 1);
            func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            func_ov003_020a6164(arg->unit, 3);
            s32 x     = self->unit->actor.position.x;
            s32 sound = data_ov003_020dc894[self->kind].start;
            if (sound < 0x56C)
                func_ov003_02087f28(sound, x);
            return 1;
        }
        case 1: {
            self                  = task->data;
            Ov003EnemyActor* unit = self->unit;
            switch (self->sprite.animTableIndex) {
                case 0:
                    self->timer++;
                    if (self->timer == data_ov003_020dc87c[self->kind]) {
                        s32 sound = data_ov003_020dc894[self->kind].hold;
                        s32 x     = self->unit->actor.position.x;
                        if (sound < 0x56C)
                            func_ov003_02087f28(sound, x);
                    }
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        func_ov003_020a35b8(&self->sprite, data_ov003_020dc880[self->kind] + 1, 0);
                        self->timer = 0;
                    }
                    break;
                case 1:
                    if (func_ov003_020a60a8(unit, 3)) {
                        if (++self->timer != self->effect->params[0])
                            break;
                        func_ov003_020a35b8(&self->sprite, data_ov003_020dc880[self->kind] + 2, 1);
                        self->position.x = unit->actor.position.x;
                        self->position.y = unit->actor.position.y;
                        self->position.z = unit->actor.position.z;
                        self->height     = unit->actor.unk_76;
                        s32 sound        = data_ov003_020dc894[self->kind].release;
                        if (sound < 0x56C)
                            func_ov003_02087f28(sound, self->position.x);
                        func_ov003_020a6198(unit, 3);
                    } else if ((unit->actor.flags & 0x200) && unit->actor.unk_58 == 5) {
                        func_ov003_020a6b84(&self->effect->streak, (u16)self->attackId, unit);
                    } else {
                        func_ov003_020a35b8(&self->sprite, data_ov003_020dc880[self->kind] + 2, 1);
                        self->position.x = unit->actor.position.x;
                        self->position.y = unit->actor.position.y;
                        self->position.z = unit->actor.position.z;
                        self->height     = unit->actor.unk_76;
                        s32 sound        = data_ov003_020dc894[self->kind].release;
                        if (sound < 0x56C)
                            func_ov003_02087f28(sound, self->position.x);
                    }
                    break;
                case 2:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
                        return 0;
                    break;
            }
            if (func_ov003_020a60a8(unit, 3)) {
                s32 width = unit->actor.unk_70;
                func_ov003_02083ca0(&unit->actor.position.x, &unit->actor.position.y, width * 4096, (width / 2) * 4096);
            }
            CombatSprite_Update(&self->sprite);
            return 1;
        }
        case 2:
            self = task->data;
            if (self->sprite.animTableIndex == 2) {
                s32 x = func_ov003_020843b0(0, self->position.x);
                s32 y = func_ov003_020843ec(0, self->position.y, self->position.z);
                CombatSprite_SetPosition(&self->sprite, x, (s16)(y - self->height));
            } else {
                CombatActor* actor = &self->unit->actor;
                CombatSprite_SetPosition(&self->sprite, actor->screenX, (s16)(actor->screenY - actor->unk_76));
            }
            func_ov003_02082730(&self->sprite, self->unit->actor.position.y + 0x1F4000);
            CombatSprite_Render(&self->sprite);
            return 1;
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            return 1;
    }
}

const u8         data_ov003_020dc87c[4] = {21, 25, 0, 0};
const s32        data_ov003_020dc880[2] = {0xB7, 0xBA};
extern char      data_ov003_020e4c64[24];
const TaskHandle data_ov003_020dc888 = {data_ov003_020e4c64, func_ov003_020af790, sizeof(Ov003ElementBind)};
// Retail's 020dc898 and 020dc89c names point inside these two sound records.
const Ov003ElementBindSounds data_ov003_020dc894[2] = {
    {0x5E, 0x5F, 0x60},
    {0x61, 0x62, 0x63}
};
char data_ov003_020e4c64[24] = "Tsk_BtlPsy_ElementBind";
