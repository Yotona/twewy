#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020e0c98;

s32 func_ov003_020d1228(Ov003EnemyShotSource* source, CombatList* list, u16 angle, s32 x, s32 y, s32 z, s32 scaleX,
                        s32 scaleY) {
    Ov003EnemyShotRootArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source     = source;
    args.list       = list;
    args.angle      = angle;
    args.position.x = x;
    args.position.y = y;
    args.position.z = z;
    args.scaleX     = scaleX;
    args.scaleY     = scaleY;
    return EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &data_ov003_020e0c98, NULL, 0, NULL, &args);
}

s32 func_ov003_020d12b4(Ov003EnemyShotRoot* self, Ov003EnemyShotRootArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    MI_CpuSet(self, 0, sizeof(*self));
    self->angle      = args->angle;
    self->position.x = args->position.x;
    self->position.y = args->position.y;
    self->position.z = args->position.z;
    self->active     = 1;
    self->scaleX     = args->scaleX;
    self->scaleY     = args->scaleY;
    func_ov003_020849bc(args->list, &self->node, self);
    if (visual != NULL) {
        const SpriteAnimEntry* entry = &visual->entries[visual->entry];
        CombatSprite_LoadDirect(2, &self->sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                                entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
        CombatSprite_SetAnim(&self->sprite, args->source->animation, args->source->animationMode);
    }
    func_ov003_020cccf0(0, args->source);
    return 1;
}

s32 func_ov003_020d1398(Ov003EnemyShotRoot* self) {
    if (func_ov003_020c3c28())
        return 0;
    if (self->sprite.sprite.resourceData != NULL) {
        CombatSprite_Update(&self->sprite);
        if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
            CombatSprite_Release(&self->sprite);
            self->sprite.sprite.resourceData = NULL;
        }
    }
    return self->active;
}

s32 func_ov003_020d13ec(Ov003EnemyShotRoot* self) {
    if (self->sprite.sprite.resourceData != NULL) {
        s32 scaleX  = self->scaleX;
        s32 xOffset = 0;
        s32 yOffset = 0;
        s32 scaleY  = self->scaleY;
        if (self->angle == 0x4000 || self->angle == 0xC000) {
            scaleX = self->scaleY;
            scaleY = self->scaleX;
        }
        switch (self->angle) {
            case 0x4000:
                xOffset = -1;
                break;
            case 0x8000:
                yOffset = -1;
                break;
        }
        s16 x, y;
        func_ov003_02084348(0, &x, &y, self->position.x, self->position.y, self->position.z);
        CombatSprite_SetPosition(&self->sprite, (s16)(x + xOffset), (s16)(y + yOffset));
        CombatSprite_SetAffineTransform(&self->sprite, self->angle, scaleX, scaleY, 1);
        func_ov003_02082730(&self->sprite, 0);
        CombatSprite_Render(&self->sprite);
    }
    return 1;
}

s32 func_ov003_020d14cc(Ov003EnemyShotRoot* self) {
    if (self->sprite.sprite.resourceData != NULL)
        CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020d14e8(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotRoot* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d12b4(self, args);
        case 1:
            return func_ov003_020d1398(self);
        case 2:
            return func_ov003_020d13ec(self);
        case 3:
            return func_ov003_020d14cc(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6d14[24] = "Tsk_BtlEnmShot_RootSub";
const TaskHandle data_ov003_020e0c98     = {data_ov003_020e6d14, func_ov003_020d14e8, sizeof(Ov003EnemyShotRoot)};
