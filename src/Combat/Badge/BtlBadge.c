#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
#include "Engine/EasyTask.h"

#include <nitro/mi/cpumem.h>

// The plain badge effect (`Tsk_BtlBadge`): the touch actor plus one extra badge sprite drawn
// behind it.

/// The battle sprite page the badge family loads from (`Apl_Hir/Grp_BtlBadge.bin`).
extern const BinIdentifier data_ov003_020dc044;

s32 func_ov003_020a6d6c(TaskPool* pool, Task* task, void* args, s32 stage);

/// The payload of the task: the touch actor followed by the badge sprite.
typedef struct BtlBadge {
    /* 0x000 */ Ov003TouchActor actor;
    /* 0x114 */ CombatSprite    badge;
} BtlBadge;

char             data_ov003_020e48e0[0x10] = "Tsk_BtlBadge";
const TaskHandle data_ov003_020dc5c0       = {data_ov003_020e48e0, func_ov003_020a6d6c, sizeof(BtlBadge)};

void func_ov003_020a6d24(Ov003PinAction* action) {
    Ov003PinAction* args = action;

    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020dc5c0, NULL, 0, NULL, &args);
}

s32 func_ov003_020a6d6c(TaskPool* pool, Task* task, void* args, s32 stage) {
    switch (stage) {
        default:
        case 0: {
            BtlBadge*         self = task->data;
            SpriteAnimationEx anim;

            MI_CpuSet(self, 0, sizeof(*self));
            func_ov003_020a3b9c(&self->actor, *(void**)args);
            CombatSprite_InitAnim(&anim.anim, 2, &data_ov003_020dc044);
            anim.anim.bits_10_11 = 0;
            anim.anim.unk_1C     = 3;
            anim.anim.animIndex  = 1;
            anim.anim.unk_28     = 3;
            anim.anim.unk_20     = 4;
            anim.animTable       = NULL;
            anim.anim.unk_18     = 2;
            anim.anim.unk_26     = 2;
            anim.anim.packIndex  = 1;
            CombatSprite_Load(&self->badge, &anim);
            CombatSprite_SetAnim(&self->badge, 0x15, 0);
            func_ov003_02082738(&self->badge, 0);
            func_ov003_02082730(&self->badge, 0x3F2000);
            return 1;
        }
        case 1: {
            BtlBadge* self = task->data;

            if (data_ov003_020e71b8->unk3D884 != 0 || (data_ov003_020e71b8->unk3D878 & 6) != 0 ||
                (data_ov003_020e71b8->unk3D878 & 0x40) != 0)
            {
                return 1;
            }
            func_ov003_020a43c8(&self->actor);
            func_ov003_020a3d98(&self->actor);
            CombatSprite_Update(&self->badge);
            CombatSprite_Update(&self->actor.sprite);
            return 1;
        }
        case 2: {
            BtlBadge* self = task->data;
            s32       posX;
            s32       posY;

            if (func_ov003_020a3dbc(&self->actor) == 0) {
                return 1;
            }
            posX = (s32)(self->actor.x * 0x10) >> 16;
            posY = (s32)(self->actor.y * 0x10) >> 16;

            CombatSprite_SetPosition(&self->badge, posX, posY);
            CombatSprite_SetPosition(&self->actor.sprite, posX, posY);
            if (func_ov003_020a3edc(&self->actor) != 0) {
                if (self->actor.state == 5) {
                    CombatSprite_Render(&self->badge);
                }
                CombatSprite_Render(&self->actor.sprite);
            } else {
                if (self->actor.state == 5) {
                    CombatSprite_RenderWithPalette(&self->badge, 8);
                }
                CombatSprite_RenderWithPalette(&self->actor.sprite, 8);
            }
            return 1;
        }
        case 3: {
            BtlBadge* self = task->data;

            CombatSprite_Release(&self->badge);
            CombatSprite_Release(&self->actor.sprite);
            func_ov003_020a3dac(&self->actor);
            return 1;
        }
    }
}
