#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
#include "Engine/EasyTask.h"

#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

// The time-boot badge (`Tsk_BtlBadge_TimeBoot`): the "boot time" countdown badge. Its payload
// keeps a pointer to the touch actor that owns it, and the three sprites it draws.

extern void func_ov003_02087ed8(s32);

/// The battle sprite page the badge family loads from (`Apl_Hir/Grp_BtlBadge.bin`).
extern const BinIdentifier data_ov003_020dc044;

s32 func_ov003_020a8350(TaskPool* pool, Task* task, void* args, s32 stage);

/// The payload of the task.
typedef struct BtlBadgeTimeBoot {
    /* 0x000 */ Ov003TouchActor* actor;   // the touch actor the badge counts down for
    /* 0x004 */ s32              unk_004;
    /* 0x008 */ s16              unk_008; // half of the actor's lifetime when the badge started
    /* 0x00A */ s16              unk_00A;
    /* 0x00C */ CombatSprite     badge;
    /* 0x06C */ CombatSprite     bar;
    /* 0x0CC */ CombatSprite     fill;  // the part of the bar that drains
    /* 0x12C */ s32              armed; // the "about to fire" animation has been switched to
} BtlBadgeTimeBoot;                     // Size: 0x130

/// The task parameter of the badge: the owning actor, its new lifetime and the lifetime the bar counts down from.
typedef struct BtlBadgeTimeBootArgs {
    /* 0x0 */ Ov003TouchActor* actor;
    /* 0x4 */ s16              duration;
    /* 0x6 */ s16              total;
} BtlBadgeTimeBootArgs; // Size: 0x8

char             data_ov003_020e4968[0x18] = "Tsk_BtlBadge_TimeBoot";
const TaskHandle data_ov003_020dc608       = {data_ov003_020e4968, func_ov003_020a8350, sizeof(BtlBadgeTimeBoot)};

void func_ov003_020a8300(Ov003TouchActor* actor, s32 duration, s32 arg2) {
    BtlBadgeTimeBootArgs sp8;

    sp8.actor    = actor;
    sp8.duration = duration;
    sp8.total    = arg2;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020dc608, NULL, 0, NULL, &sp8);
}

s32 func_ov003_020a8350(TaskPool* pool, Task* task, void* args, s32 stage) {
    switch (stage) {
        case 0: {
            BtlBadgeTimeBoot*     self = task->data;
            BtlBadgeTimeBootArgs* arg  = args;
            SpriteAnimationEx     anim;

            MI_CpuSet(self, 0, sizeof(*self));
            self->actor          = arg->actor;
            self->actor->unk_048 = arg->duration;
            self->unk_008        = arg->total;
            self->armed          = 0;
            self->unk_004        = 0;
            CombatSprite_InitAnim(&anim.anim, 2, &data_ov003_020dc044);
            anim.anim.bits_10_11 = 0;
            anim.anim.unk_1C     = 3;
            anim.anim.unk_28     = 1;
            anim.anim.unk_20     = 4;
            anim.animTable       = NULL;
            anim.anim.unk_18     = 2;
            anim.anim.unk_26     = 1;
            anim.anim.packIndex  = 1;
            anim.anim.animIndex  = 1;
            func_ov003_02082998(&self->badge, &anim);
            func_ov003_020825b8(&self->badge, 0, 0);
            func_ov003_02082738(&self->badge, 0);
            func_ov003_02082730(&self->badge, 0x3F5000);
            func_ov003_02082998(&self->bar, &anim);
            func_ov003_020825b8(&self->bar, 2, 0);
            func_ov003_02082738(&self->bar, 0);
            func_ov003_02082730(&self->bar, 0x3F4000);
            func_ov003_02082998(&self->fill, &anim);
            func_ov003_020825b8(&self->fill, 1, 0);
            func_ov003_02082738(&self->fill, 0);
            func_ov003_02082730(&self->fill, 0x3F3000);
            return 1;
        }
        case 1: {
            BtlBadgeTimeBoot* self = task->data;

            if (data_ov003_020e71b8->unk3D884 != 0 || (data_ov003_020e71b8->unk3D878 & 6) != 0 ||
                (data_ov003_020e71b8->unk3D878 & 0x40) != 0)
            {
                return 1;
            }
            // The original table-check leaves room for the five live states in front of the two
            // faded-out ones; spell them out so the jump table is emitted.
            switch (self->actor->state) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                case 5:
                    return 0;
            }
            if (self->unk_004 == 0) {
                if (data_ov003_020e71b8->unk3D874 == 2) {
                    self->actor->unk_048 = self->actor->unk_048 - 1;
                }
                if (self->armed == 0 && self->actor->unk_048 < self->unk_008 / 2) {
                    self->armed = 1;
                    func_ov003_020825b8(&self->bar, 3, 0);
                }
                if (self->actor->unk_048 < 0) {
                    func_ov003_02087ed8(0x2D);
                    func_ov003_020a4390(self->actor, 3);
                    return 0;
                }
            }
            func_ov003_02082b0c(&self->badge);
            func_ov003_02082b0c(&self->bar);
            func_ov003_02082b0c(&self->fill);
            return 1;
        }
        case 2: {
            BtlBadgeTimeBoot* self = task->data;
            s32               ratio;
            s32               posX;
            s32               posY;

            if (func_ov003_02084d80(0) == 1) {
                return 1;
            }
            if ((data_ov003_020e71b8->unk3D878 & 6) != 0 || data_ov003_020e71b8->unk3D884 != 0) {
                return 1;
            }

            ratio = FX_Divide(self->actor->unk_048 << 12, self->unk_008 << 12);
            CombatSprite_SetAffineTransform(&self->fill, (u16)FX_Mul(ratio, 0x10000), 0x1000, 0x1000, 0);
            posX = (s32)(self->actor->x * 0x10) >> 16;
            posY = (s32)(self->actor->y * 0x10) >> 16;
            func_ov003_02082724(&self->badge, posX, posY);
            func_ov003_02082724(&self->bar, posX, posY);
            func_ov003_02082724(&self->fill, posX, posY);
            func_ov003_02082b64(&self->badge);
            func_ov003_02082b64(&self->bar);
            func_ov003_02082b64(&self->fill);
            return 1;
        }
        case 3: {
            BtlBadgeTimeBoot* self = task->data;

            func_ov003_02082cc4(&self->badge);
            func_ov003_02082cc4(&self->bar);
            func_ov003_02082cc4(&self->fill);
            return 1;
        }
    }
}
