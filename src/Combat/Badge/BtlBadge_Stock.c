#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

#include <nitro/mi/cpumem.h>

// The stock badge (`Tsk_BtlBadge_Stock`): the badge sprite, its count bar and the flash that
// plays when a stock is used up.

/// The battle sprite page the badge family loads from (`Apl_Hir/Grp_BtlBadge.bin`).
extern const BinIdentifier data_ov003_020dc044;

// Nonmatching: the target keeps the task data in a low callee-saved register from the prologue on,
// so its stage 0 animation descriptor needs one more callee-saved register and the constant stores
// come out in a different order; the stage 1/2/3 logic matches.
s32 func_ov003_020a7eb0(TaskPool* pool, Task* task, void* args, s32 stage);

/// The payload of the task: the touch actor followed by the badge sprites.
typedef struct BtlBadgeStock {
    /* 0x000 */ Ov003TouchActor actor;
    /* 0x114 */ CombatSprite    badge;
    /* 0x174 */ CombatSprite    bar;
    /* 0x1D4 */ CombatSprite    flash;
    /* 0x234 */ s16             shownCount; // value the bar currently displays
    /* 0x236 */ u8              unk_236[2];
} BtlBadgeStock;                            // Size: 0x238

char             data_ov003_020e4954[0x14] = "Tsk_BtlBadge_Stock";
const TaskHandle data_ov003_020dc5fc       = {data_ov003_020e4954, func_ov003_020a7eb0, sizeof(BtlBadgeStock)};

void func_ov003_020a7e68(Ov003PinAction* action) {
    Ov003PinAction* args = action;

    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020dc5fc, NULL, 0, NULL, &args);
}

/// Shows `count` remaining uses on the bar: its animation id is offset by the count.
void func_ov003_020a82d0(BtlBadgeStock* self, s16 count) {
    CombatSprite_SetAnim(&self->badge, (u16)(count + 0xA), 0);
    self->shownCount = count;
}

s32 func_ov003_020a7eb0(TaskPool* pool, Task* task, void* args, s32 stage) {
    switch (stage) {
        case 0: {
            BtlBadgeStock*    self  = task->data;
            Ov003TouchActor*  actor = &self->actor;
            SpriteAnimationEx anim;

            MI_CpuSet(self, 0, 0x238);
            func_ov003_020a3b9c(actor, *(void**)args);
            CombatSprite_InitAnim(&anim.anim, 2, &data_ov003_020dc044);
            anim.anim.bits_10_11 = 0;
            anim.anim.unk_18     = 2;
            anim.animTable       = NULL;
            anim.anim.unk_20     = 4;
            anim.anim.unk_26     = 2;
            anim.anim.unk_28     = 3;
            anim.anim.unk_1C     = 3;
            anim.anim.animIndex  = 0xB;
            anim.anim.packIndex  = 1;
            CombatSprite_Load(&self->badge, &anim);
            func_ov003_02082738(&self->badge, 0);
            func_ov003_02082730(&self->badge, 0x3E9000);
            CombatSprite_Load(&self->bar, &anim);
            CombatSprite_SetAnim(&self->bar, 0x15, 0);
            func_ov003_02082738(&self->bar, 0);
            func_ov003_02082730(&self->bar, 0x3F2000);
            func_ov003_020a82d0(self, actor->unk_044);
            return 1;
        }
        case 1: {
            BtlBadgeStock*   self  = task->data;
            Ov003TouchActor* actor = &self->actor;
            s32              state;

            if (data_ov003_020e71b8->unk3D884 != 0 || (data_ov003_020e71b8->unk3D878 & 6) != 0 ||
                (data_ov003_020e71b8->unk3D878 & 0x40) != 0)
            {
                return 1;
            }
            state = func_ov003_020a43c8(actor);
            if (state == 3 || state == 7) {
                func_ov003_020a82d0(self, actor->unk_044);
                if (data_ov003_020e71b8->unk3D874 == 2) {
                    SpriteAnimationEx anim;

                    CombatSprite_InitAnim(&anim.anim, 2, &data_ov003_020dc044);
                    anim.anim.bits_10_11 = 0;
                    anim.anim.unk_1C     = 3;
                    anim.anim.animIndex  = 1;
                    anim.anim.unk_28     = 1;
                    anim.anim.unk_20     = 4;
                    anim.animTable       = NULL;
                    anim.anim.unk_18     = 2;
                    anim.anim.unk_26     = 3;
                    anim.anim.packIndex  = 1;
                    CombatSprite_Load(&self->flash, &anim);
                    CombatSprite_SetAnim(&self->flash, 0x16, 1);
                    func_ov003_02082738(&self->flash, 0);
                    func_ov003_02082730(&self->flash, 0x3F2000);
                }
            }
            func_ov003_020a3d98(actor);
            // The target's jump table has five live slots in front of the countdown case; spell
            // them out so mwcc emits a table instead of a single compare.
            switch (self->actor.state) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    if (self->shownCount != actor->unk_044) {
                        func_ov003_020a82d0(self, actor->unk_044);
                    }
                    if (actor->unk_044 <= 0) {
                        func_ov003_020a4390(actor, 2);
                        func_ov003_02087ed8(SEIDX_SE_PSY_EMPTY);
                    }
                    break;
                case 4:
                case 5:
                    break;
            }
            if (Sprite_HasAnimation(&self->flash.sprite) != 0) {
                if (SpriteMgr_IsAnimationFinished(&self->flash.sprite) != 0) {
                    CombatSprite_Release(&self->flash);
                } else {
                    CombatSprite_Update(&self->flash);
                }
            }
            CombatSprite_Update(&self->bar);
            CombatSprite_Update(&self->badge);
            CombatSprite_Update(&self->actor.sprite);
            return 1;
        }
        case 2: {
            BtlBadgeStock*   self  = task->data;
            Ov003TouchActor* actor = &self->actor;
            s32              posY;
            s32              posX;

            if (func_ov003_020a3dbc(actor) == 0) {
                return 1;
            }
            posX = (s32)(actor->x * 0x10) >> 16;
            posY = (s32)(actor->y * 0x10) >> 16;
            CombatSprite_SetPosition(&self->bar, posX, posY);
            CombatSprite_SetPosition(&self->badge, posX, posY);
            CombatSprite_SetPosition(&self->actor.sprite, posX, posY);
            if (func_ov003_020a3edc(actor) != 0) {
                if (actor->state == 5) {
                    CombatSprite_Render(&self->bar);
                } else {
                    CombatSprite_Render(&self->badge);
                }
                CombatSprite_Render(&self->actor.sprite);
            } else {
                if (actor->state == 5) {
                    CombatSprite_RenderWithPalette(&self->bar, 8);
                } else {
                    CombatSprite_RenderWithPalette(&self->badge, 8);
                }
                CombatSprite_RenderWithPalette(&self->actor.sprite, 8);
            }
            if (Sprite_HasAnimation(&self->flash.sprite) != 0) {
                CombatSprite_SetPosition(&self->flash, posX, posY);
                if (func_ov003_020a3edc(actor) != 0) {
                    CombatSprite_Render(&self->flash);
                } else {
                    CombatSprite_RenderWithPalette(&self->flash, 8);
                }
            }
            return 1;
        }
        case 3: {
            BtlBadgeStock*   self  = task->data;
            Ov003TouchActor* actor = &self->actor;

            CombatSprite_Release(&self->bar);
            CombatSprite_Release(&self->badge);
            CombatSprite_Release(&self->actor.sprite);
            func_ov003_020a3dac(actor);
            return 1;
        }
    }
}
