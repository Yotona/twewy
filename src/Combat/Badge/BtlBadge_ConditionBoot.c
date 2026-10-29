#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
#include "Engine/EasyTask.h"

#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

// The condition-boot badge (`Tsk_BtlBadge_ConditionBoot`): a small badge sprite whose visible
// window follows the battle state of the actor it was spawned from.

/// The battle sprite page the badge family loads from (`Apl_Hir/Grp_BtlBadge.bin`).
extern const BinIdentifier data_ov003_020dc044;

s32 func_ov003_020a6fdc(TaskPool* pool, Task* task, void* args, s32 stage);

/// The payload of the task: the spawned actor, the badge flag and the badge sprite.
typedef struct BtlBadgeConditionBoot {
    /* 0x000 */ Ov003TouchActor* actor;  // the actor the badge follows
    /* 0x004 */ s32              unk_04; // latch once the badge has been triggered
    /* 0x008 */ s32              unk_08; // battle state the badge was spawned for
    /* 0x00C */ CombatSprite     badge;
} BtlBadgeConditionBoot;

/// The task parameter: the actor and the battle state it was spawned for.
typedef struct BtlBadgeConditionBootArgs {
    /* 0x0 */ Ov003TouchActor* actor;
    /* 0x4 */ s32              unk_04;
} BtlBadgeConditionBootArgs;

char             data_ov003_020e48f0[0x1C] = "Tsk_BtlBadge_ConditionBoot";
const TaskHandle data_ov003_020dc5cc       = {data_ov003_020e48f0, func_ov003_020a6fdc, sizeof(BtlBadgeConditionBoot)};

/// Spawns the badge for `actor`; `arg1` is the battle state the badge was armed in.
void func_ov003_020a6f90(Ov003TouchActor* actor, s32 arg1) {
    BtlBadgeConditionBootArgs sp8;

    sp8.unk_04 = arg1;
    sp8.actor  = actor;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020dc5cc, NULL, 0, NULL, &sp8);
}

s32 func_ov003_020a6fdc(TaskPool* pool, Task* task, void* args, s32 stage) {
    switch (stage) {
        default:
        case 0: {
            BtlBadgeConditionBoot*     self = task->data;
            BtlBadgeConditionBootArgs* arg  = args;
            SpriteAnimationEx          anim;

            MI_CpuSet(self, 0, sizeof(*self));
            self->actor  = arg->actor;
            self->unk_08 = arg->unk_04;
            self->unk_04 = 0;
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
            CombatSprite_SetAnim(&self->badge, 0x17, 0);
            func_ov003_02082738(&self->badge, 0);
            func_ov003_02082730(&self->badge, 0x3F3000);
            return 1;
        }
        case 1: {
            BtlBadgeConditionBoot* self = task->data;
            Ov003TouchActor*       slots[6];

            if (data_ov003_020e71b8->unk3D884 != 0 || (data_ov003_020e71b8->unk3D878 & 6) != 0 ||
                (data_ov003_020e71b8->unk3D878 & 0x40) != 0)
            {
                return 1;
            }
            switch (self->actor->state) {
                default:
                case 0:
                case 1:
                case 2:
                case 3:
                    break;
                case 4:
                case 5:
                    return 0;
            }
            if (self->unk_04 == 0) {
                if (data_ov003_020e71b8->unk3D874 == 2) {
                    s32 triggered = self->unk_08 == 2 && func_ov003_020a5ac4(slots, 6, &self->actor->effect, 0) != 0;

                    if (triggered) {
                        func_ov003_02087ed8(0x2D);
                        func_ov003_020a4390(self->actor, 3);
                        return 0;
                    }
                }
            }
            CombatSprite_Update(&self->badge);
            return 1;
        }
        case 2: {
            BtlBadgeConditionBoot* self = task->data;
            Ov003TouchActor*       actor;
            s32                    posX;
            s32                    posY;

            if (func_ov003_02084d80(0) == 1) {
                return 1;
            }
            if ((data_ov003_020e71b8->unk3D878 & 6) != 0 || data_ov003_020e71b8->unk3D884 != 0) {
                return 1;
            }
            actor = self->actor;
            posX  = (s32)(actor->x * 0x10) >> 16;
            posY  = (s32)(actor->y * 0x10) >> 16;
            CombatSprite_SetPosition(&actor->sprite, posX, posY);
            CombatSprite_SetPosition(&self->badge, posX, posY);
            CombatSprite_Render(&actor->sprite);
            CombatSprite_Render(&self->badge);
            return 1;
        }
        case 3: {
            BtlBadgeConditionBoot* self = task->data;

            CombatSprite_Release(&self->badge);
            return 1;
        }
    }
}
