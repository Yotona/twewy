#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

// The level-up gauge badge (`Tsk_BtlBadge_Gauge`): the badge sprite, its gauge bar and the flash
// that plays when the actor is hit, all following the touch actor's remaining lifetime.

extern void func_ov003_02087ed8(s32);

/// The battle sprite page the badge family loads from (`Apl_Hir/Grp_BtlBadge.bin`).
extern const BinIdentifier data_ov003_020dc044;

s32 func_ov003_020a7688(TaskPool* pool, Task* task, void* args, s32 stage);

/// The payload of the task: the touch actor followed by the badge sprites.
typedef struct BtlBadgeGauge {
    /* 0x000 */ Ov003TouchActor actor;
    /* 0x114 */ CombatSprite    badge;
    /* 0x174 */ CombatSprite    bar;
    /* 0x1D4 */ CombatSprite    flash;
    /* 0x234 */ CombatSprite    glow;
    /* 0x294 */ s32             glowing; // the bar is showing its filled animation
    /* 0x298 */ s32             unk_298;
} BtlBadgeGauge;                         // Size: 0x29C

char             data_ov003_020e4924[0x14] = "Tsk_BtlBadge_Gauge";
const TaskHandle data_ov003_020dc5e4       = {data_ov003_020e4924, func_ov003_020a7688, sizeof(BtlBadgeGauge)};

void func_ov003_020a7640(void* arg0) {
    s32 sp8 = (s32)arg0;

    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020dc5e4, NULL, 0, NULL, &sp8);
}

s32 func_ov003_020a7688(TaskPool* pool, Task* task, void* args, s32 stage) {
    switch (stage) {
        case 0: {
            BtlBadgeGauge*    self  = task->data;
            Ov003TouchActor*  actor = &self->actor;
            SpriteAnimationEx anim;

            MI_CpuSet(self, 0, sizeof(*self));
            func_ov003_020a3b9c(actor, *(void**)args);
            CombatSprite_InitAnim(&anim.anim, 2, &data_ov003_020dc044);
            anim.anim.bits_10_11 = 0;
            anim.anim.unk_1E     = 0;
            anim.anim.packIndex  = 1;
            anim.anim.unk_18     = 2;
            anim.animTable       = NULL;
            anim.anim.unk_20     = 4;
            anim.anim.unk_28     = 3;
            anim.anim.unk_26     = 1;
            func_ov003_02082998(&self->badge, &anim);
            func_ov003_020825b8(&self->badge, 7, 0);
            func_ov003_02082738(&self->badge, 0);
            func_ov003_02082730(&self->badge, 0x3E9000);
            func_ov003_02082998(&self->bar, &anim);
            func_ov003_020825b8(&self->bar, 9, 0);
            func_ov003_02082738(&self->bar, 0);
            func_ov003_02082730(&self->bar, 0x3EA000);
            func_ov003_02082998(&self->flash, &anim);
            func_ov003_020825b8(&self->flash, 0x15, 0);
            func_ov003_02082738(&self->flash, 0);
            func_ov003_02082730(&self->flash, 0x3F2000);
            return 1;
        }
        case 1: {
            BtlBadgeGauge*   self  = task->data;
            Ov003TouchActor* actor = &self->actor;
            s32              state;

            if (data_ov003_020e71b8->unk3D884 != 0 || (data_ov003_020e71b8->unk3D878 & 6) != 0 ||
                (data_ov003_020e71b8->unk3D878 & 0x40) != 0)
            {
                return 1;
            }
            state = func_ov003_020a43c8(actor);
            if (state == 3 || state == 7) {
                if (actor->unk_044 == actor->unk_046) {
                    self->glowing = 1;
                    func_ov003_020825b8(&self->bar, 9, 0);
                } else {
                    self->glowing = 0;
                    func_ov003_020825b8(&self->bar, 8, 0);
                }
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
                    func_ov003_02082998(&self->glow, &anim);
                    func_ov003_020825b8(&self->glow, 0x16, 1);
                    func_ov003_02082738(&self->glow, 0);
                    func_ov003_02082730(&self->glow, 0x3F2000);
                }
            }
            func_ov003_020a3d98(actor);
            switch (self->actor.state) {
                case 0:
                case 1:
                case 2:
                    break;
                case 3:
                    if (self->glowing != 0 && actor->unk_044 < actor->unk_046) {
                        self->glowing = 0;
                        func_ov003_020825b8(&self->bar, 8, 0);
                    }
                    if (actor->unk_044 <= 0) {
                        func_ov003_020a4390(actor, 2);
                        func_ov003_02087ed8(0x2E);
                    }
                    break;
                case 4:
                case 5:
                    break;
            }
            if (Sprite_HasAnimation(&self->glow.sprite) != 0) {
                if (SpriteMgr_IsAnimationFinished(&self->glow.sprite) != 0) {
                    func_ov003_02082cc4(&self->glow);
                } else {
                    func_ov003_02082b0c(&self->glow);
                }
            }
            func_ov003_02082b0c(&self->flash);
            func_ov003_02082b0c(&self->bar);
            func_ov003_02082b0c(&self->badge);
            func_ov003_02082b0c(&self->actor.sprite);
            return 1;
        }
        case 2: {
            BtlBadgeGauge*   self  = task->data;
            Ov003TouchActor* actor = &self->actor;
            s32              state;
            s32              posX;
            s32              posY;

            if (func_ov003_020a3dbc(actor) == 0) {
                return 1;
            }
            if (actor->unk_044 != actor->unk_046) {
                func_ov003_0208260c(&self->bar, 0, FX_Divide(actor->unk_044 << 12, actor->unk_046 << 12), 0x1000, 0);
            } else {
                func_ov003_0208260c(&self->bar, 0, 0x1000, 0x1000, 0);
            }
            posX = (s32)(actor->x * 0x10) >> 16;
            posY = (s32)(actor->y * 0x10) >> 16;
            func_ov003_02082724(&self->flash, posX, posY);
            func_ov003_02082724(&self->bar, posX, posY);
            func_ov003_02082724(&self->badge, posX, posY);
            func_ov003_02082724(&self->actor.sprite, posX, posY);
            state = actor->state;
            if (func_ov003_020a3edc(actor) != 0) {
                if (state == 5) {
                    func_ov003_02082b64(&self->flash);
                } else {
                    func_ov003_02082b64(&self->bar);
                    func_ov003_02082b64(&self->badge);
                }
                func_ov003_02082b64(&self->actor.sprite);
            } else {
                if (state == 5) {
                    func_ov003_02082848(&self->flash, 8);
                } else {
                    func_ov003_02082848(&self->bar, 8);
                    func_ov003_02082848(&self->badge, 8);
                }
                func_ov003_02082848(&self->actor.sprite, 8);
            }
            if (Sprite_HasAnimation(&self->glow.sprite) != 0) {
                func_ov003_02082724(&self->glow, posX, posY);
                if (func_ov003_020a3edc(actor) != 0) {
                    func_ov003_02082b64(&self->glow);
                } else {
                    func_ov003_02082848(&self->glow, 8);
                }
            }
            return 1;
        }
        case 3: {
            BtlBadgeGauge*   self  = task->data;
            Ov003TouchActor* actor = &self->actor;

            func_ov003_02082cc4(&self->flash);
            func_ov003_02082cc4(&self->bar);
            func_ov003_02082cc4(&self->badge);
            func_ov003_02082cc4(&self->actor.sprite);
            func_ov003_020a3dac(actor);
            return 1;
        }
    }
}
