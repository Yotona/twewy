#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Combat/Core/CombatList.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

/// The task parameter: the root of the beam to follow and how the beam grows along it.
typedef struct Ov003EnemyShotLaserSubArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ CombatList*           list;  // the roots of the beam
    /* 0x08 */ Ov003EnemyShotRoot*   root;  // the root this segment lies on
    /* 0x0C */ u16                   animation;
    /* 0x0E */ s16                   inset; // half width of the beam, pixels
    /* 0x10 */ s16                   length;
    /* 0x14 */ s32                   step;  // growth per frame
    /* 0x18 */ s16                   delay;
} Ov003EnemyShotLaserSubArgs;               // Size: 0x1C

typedef struct Ov003EnemyShotLaserSub {
    /* 0x000 */ CombatSprite          sprite;  // the main beam
    /* 0x060 */ CombatSprite          sprite2; // the glow
    /* 0x0C0 */ Ov003EnemyShotSource* source;
    /* 0x0C4 */ CombatList*           list;
    /* 0x0C8 */ Ov003EnemyShotRoot*   root; // the next root, spawns the next segment when this one has finished
    /* 0x0CC */ u16                   direction;
    /* 0x0CE */ u16                   reservedCE;
    /* 0x0D0 */ s32                   x;
    /* 0x0D4 */ s32                   y;
    /* 0x0D8 */ s32                   z;
    /* 0x0DC */ s32                   step;
    /* 0x0E0 */ s16                   inset;
    /* 0x0E2 */ s16                   length;
    /* 0x0E4 */ u16                   animation;
    /* 0x0E6 */ s16                   delay;
    /* 0x0E8 */ s16                   timer;
    /* 0x0EA */ s16                   phase;
    /* 0x0EC */ s32                   growX;
    /* 0x0F0 */ s32                   growY;
    /* 0x0F4 */ s32                   growTotal;
    /* 0x0F8 */ s32                   targetX;
    /* 0x0FC */ s32                   targetY;
} Ov003EnemyShotLaserSub; // Size: 0x100

extern const TaskHandle data_ov003_020e0c8c;

void func_ov003_020d07a8(Ov003EnemyShotSource* source, CombatList* list, Ov003EnemyShotRoot* root, u16 animation, s16 inset,
                         s16 length, s32 step, s16 delay) {
    Ov003EnemyShotLaserSubArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source    = source;
    args.list      = list;
    args.root      = root;
    args.animation = animation;
    args.inset     = inset;
    args.length    = length;
    args.step      = step;
    args.delay     = delay;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &data_ov003_020e0c8c, NULL, 0, NULL, &args);
}

s32 func_ov003_020d0834(Ov003EnemyShotLaserSub* self, Ov003EnemyShotLaserSubArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    MI_CpuSet(self, 0, sizeof(*self));
    self->source    = args->source;
    self->list      = args->list;
    self->growX     = 0x1000;
    self->growY     = 0x1000;
    self->animation = args->animation;
    self->inset     = args->inset;
    self->length    = args->length;
    self->step      = args->step;
    self->delay     = args->delay;
    CombatSprite_LoadDirect(2, &self->sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                            entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, args->source->animation, 0);
    CombatSprite_LoadDirect(2, &self->sprite2, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                            entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite2, self->animation, 0);
    if (args->root == NULL) {
        func_ov003_020d117c(self->list);
        return 0;
    }
    self->root =
        func_ov003_020d11b8(args->root, &self->direction, &self->targetX, &self->targetY, &self->x, &self->y, &self->z);
    return 1;
}

s32 func_ov003_020d0990(Ov003EnemyShotLaserSub* self) {
    Ov003AttackInfo info;
    s16             x, y;
    s32             boxX, boxY, boxZ;
    s32             halfWidth, halfHeight;

    if (data_ov003_020e71b8->unk3D878 & 0x40)
        return 1;
    if (func_ov003_020c3c28()) {
        func_ov003_020d117c(self->list);
        return 0;
    }
    switch (self->phase) {
        case 0:
            if (self->direction == 0x4000 || self->direction == 0xC000) {
                self->growY += self->step;
                self->growTotal += self->step;
                if (self->growY >= self->targetY) {
                    self->growY     = self->targetY;
                    self->growTotal = self->targetY;
                    self->phase++;
                    if (self->direction == 0x4000)
                        self->y -= FX_Mul(self->targetY, FX32_CONVERT(self->inset));
                    else
                        self->y += FX_Mul(self->targetY, FX32_CONVERT(self->inset));
                    self->direction += 0x8000;
                }
            } else {
                self->growX += self->step;
                self->growTotal += self->step;
                if (self->growX >= self->targetX) {
                    self->growX     = self->targetX;
                    self->growTotal = self->targetX;
                    self->phase++;
                    if (self->direction == 0)
                        self->x -= FX_Mul(self->targetX, FX32_CONVERT(self->inset));
                    else
                        self->x += FX_Mul(self->targetX, FX32_CONVERT(self->inset));
                    self->direction += 0x8000;
                }
            }
            break;
        case 1:
            if (self->direction == 0x4000 || self->direction == 0xC000) {
                self->growY -= self->step;
                if (self->growY <= 0x1000) {
                    self->phase++;
                    self->growX = 0;
                    self->growY = 0;
                }
            } else {
                self->growX -= self->step;
                if (self->growX <= 0x1000) {
                    self->phase++;
                    self->growX = 0;
                    self->growY = 0;
                }
            }
            break;
        case 2:
            self->timer++;
            if (self->timer > self->delay) {
                if (self->root != NULL) {
                    func_ov003_020d07a8(self->source, self->list, self->root, self->animation, self->inset, self->length,
                                        self->step, self->delay);
                    func_ov003_020cccf0(0, self->source);
                } else {
                    func_ov003_020d117c(self->list);
                }
                self->phase++;
            }
            break;
        case 3:
            return 0;
    }
    info      = *func_ov003_0208a114(self->source->core.attackId);
    halfWidth = FX_Mul(self->growX, FX32_CONVERT(self->inset)) >> 1;
    if (halfWidth < 0x1000)
        halfWidth = 0x1000;
    halfHeight = FX_Mul(self->growY, FX32_CONVERT(self->inset)) >> 1;
    if (halfHeight < 0x1000)
        halfHeight = 0x1000;
    info.unk_05 = halfWidth >> 12;
    info.unk_06 = halfHeight >> 12;
    boxX        = self->x;
    boxY        = self->y;
    boxZ        = self->z;
    switch (self->direction) {
        case 0:
            boxX -= FX_Mul(self->growX, FX32_CONVERT(self->inset)) >> 1;
            break;
        case 0x8000:
            boxX += FX_Mul(self->growX, FX32_CONVERT(self->inset)) >> 1;
            break;
        case 0x4000:
            boxY -= FX_Mul(self->growY, FX32_CONVERT(self->inset)) >> 1;
            break;
        case 0xC000:
            boxY += FX_Mul(self->growY, FX32_CONVERT(self->inset)) >> 1;
            break;
    }
    func_ov003_02084348(0, &x, &y, boxX, boxY, boxZ);
    func_ov003_0208a1a4(&info, &self->source->core.owner->base.actor.baseArgs, x, y);
    CombatSprite_Update(&self->sprite);
    CombatSprite_Update(&self->sprite2);
    return 1;
}

s32 func_ov003_020d0ff4(Ov003EnemyShotLaserSub* self) {
    s16 x, y;
    s32 scaleX  = self->growX;
    s32 yOffset = 0;
    s32 xOffset = 0;
    s32 scaleY  = self->growY;
    if (self->direction == 0x4000 || self->direction == 0xC000) {
        scaleX = self->growY;
        scaleY = self->growX;
    }
    switch (self->direction) {
        case 0x8000:
            yOffset = -1;
            break;
        case 0x4000:
            xOffset = -1;
            break;
    }
    func_ov003_02084348(0, &x, &y, self->x, self->y, self->z);
    CombatSprite_SetPosition(&self->sprite, (s16)(x + xOffset), (s16)(y + yOffset));
    CombatSprite_SetAffineTransform(&self->sprite, self->direction, scaleX, scaleY, 1);
    func_ov003_02082730(&self->sprite, 0x96000);
    CombatSprite_Render(&self->sprite);
    func_ov003_02084348(0, &x, &y, self->x, self->y, self->z);
    CombatSprite_SetPosition(&self->sprite2, x, y);
    func_ov003_02082730(&self->sprite2, 0x12C000);
    CombatSprite_Render(&self->sprite2);
    return 1;
}

s32 func_ov003_020d1110(Ov003EnemyShotLaserSub* self) {
    CombatSprite_Release(&self->sprite);
    CombatSprite_Release(&self->sprite2);
    return 1;
}

s32 func_ov003_020d112c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotLaserSub* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d0834(self, args);
        case 1:
            return func_ov003_020d0990(self);
        case 2:
            return func_ov003_020d0ff4(self);
        case 3:
            return func_ov003_020d1110(self);
        default:
            return 1;
    }
}

void func_ov003_020d117c(CombatList* list) {
    Ov003EnemyShotRoot* node = func_ov003_0208495c(list);
    if (node == NULL)
        return;
    for (; node != NULL; node = func_ov003_02084984(&node->node))
        node->active = 0;
    func_ov003_020849ac(list);
}

Ov003EnemyShotRoot* func_ov003_020d11b8(Ov003EnemyShotRoot* root, u16* angle, s32* scaleX, s32* scaleY, s32* x, s32* y,
                                        s32* z) {
    if (root == NULL)
        return NULL;
    if (angle != NULL)
        *angle = root->angle;
    if (scaleX != NULL)
        *scaleX = root->scaleX;
    if (scaleY != NULL)
        *scaleY = root->scaleY;
    if (x != NULL)
        *x = root->position.x;
    if (y != NULL)
        *y = root->position.y;
    if (z != NULL)
        *z = root->position.z;
    return func_ov003_02084984(&root->node);
}

char             data_ov003_020e6cfc[24] = "Tsk_BtlEnmShot_LaserSub";
const TaskHandle data_ov003_020e0c8c     = {data_ov003_020e6cfc, func_ov003_020d112c, sizeof(Ov003EnemyShotLaserSub)};
