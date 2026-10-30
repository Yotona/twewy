#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"

#include <nitro/mi/cpumem.h>

// The ranking badge (`Tsk_BtlBadge_RankingEffect`). Unlike the other badge effects it is not a
// touch actor: it is a 0x64-byte sprite block that shows the sprite of the ranking entry the
// spawn record selects, and it runs until the battle moves on.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The task parameter: the actor to follow and the sprite of its ranking entry.
typedef struct BtlBadgeRankingEffectArgs {
    /* 0x0 */ Ov003TouchActor* actor;
    /* 0x4 */ s16              spriteIndex;
} BtlBadgeRankingEffectArgs; // Size: 0x8

/// The payload of the task.
typedef struct BtlBadgeRankingEffect {
    /* 0x00 */ Ov003TouchActor* source; // the actor the sprite follows
    /* 0x04 */ CombatSprite     sprite;
} BtlBadgeRankingEffect;                // Size: 0x64

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

/// The battle sprite page the badge family loads from (`Apl_Hir/Grp_BtlBadge.bin`).
extern const BinIdentifier data_ov003_020dc044;

extern s32 func_ov003_020a6c7c(s32 kind);

char data_ov003_020e4938[0x1C] = "Tsk_BtlBadge_RankingEffect";

/// Defined below; the task handle needs its address.
s32 func_ov003_020a7c9c(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle data_ov003_020dc5f0 = {data_ov003_020e4938, func_ov003_020a7c9c, sizeof(BtlBadgeRankingEffect)};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Spawns the ranking badge for the ranking entry of the actor's pin, if the battle has one.
void func_ov003_020a7ba8(Ov003TouchActor* actor) {
    BtlBadgeRankingEffectArgs spawn;

    spawn.actor = actor;
    switch (func_ov003_020a6c14()) {
        case 0:
            return;
        case 2:
            return;
        case 1:
            if ((s32)actor->params.unk_05 >= 0xD) {
                return;
            }
            switch (func_ov003_020a6c7c(actor->params.unk_05)) {
                case 0:
                    spawn.spriteIndex = 0;
                    break;
                case 1:
                    spawn.spriteIndex = 1;
                    break;
                case 2:
                    spawn.spriteIndex = 2;
                    break;
                case 12:
                    spawn.spriteIndex = 3;
                    break;
                default:
                    return;
            }
            break;
    }
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020dc5f0, NULL, 0, NULL, &spawn);
}

/// The ranking badge stages: build, update, position and render, release.
// Nonmatching: the target keeps `task->data` in `r4` from the dispatch on and reuses it for the
// stage-1/stage-3 bodies; mwcc re-materialises it inside those cases.
s32 func_ov003_020a7c9c(TaskPool* pool, Task* task, void* args, s32 stage) {
    switch (stage) {
        case 0: {
            BtlBadgeRankingEffect*     self = task->data;
            BtlBadgeRankingEffectArgs* arg  = args;
            SpriteAnimationEx          anim;

            MI_CpuSet(self, 0, sizeof(*self));
            self->source = arg->actor;
            CombatSprite_InitAnim(&anim.anim, 2, &data_ov003_020dc044);
            anim.anim.bits_10_11 = 0;
            anim.anim.unk_1C     = 3;
            anim.anim.animIndex  = 2;
            anim.anim.unk_28     = 3;
            anim.anim.unk_20     = 4;
            anim.animTable       = NULL;
            anim.anim.unk_18     = 2;
            anim.anim.unk_26     = 2;
            anim.anim.packIndex  = 1;
            CombatSprite_Load(&self->sprite, &anim);
            CombatSprite_SetAnim(&self->sprite, arg->spriteIndex, 0);
            func_ov003_02082738(&self->sprite, 0);
            func_ov003_02082730(&self->sprite, 0x3F6000);
            return 1;
        }
        case 1: {
            BtlBadgeRankingEffect* self = task->data;

            if (data_ov003_020e71b8->unk3D874 == 2) {
                return 0;
            }
            CombatSprite_Update(&self->sprite);
            return 1;
        }
        case 2: {
            BtlBadgeRankingEffect* self = task->data;
            s32                    x;
            s32                    y;

            if (func_ov003_02084d80(0) == 1) {
                return 1;
            }
            if ((data_ov003_020e71b8->unk3D878 & 6) != 0 || data_ov003_020e71b8->unk3D884 != 0) {
                return 1;
            }
            x = (s32)((u32)self->source->x << 4) >> 16;
            y = (s32)((u32)self->source->y << 4) >> 16;
            CombatSprite_SetPosition(&self->sprite, x, y);
            if (func_ov003_020a3edc(self->source) != 0) {
                CombatSprite_Render(&self->sprite);
            } else {
                CombatSprite_RenderWithPalette(&self->sprite, 8);
            }
            return 1;
        }
        case 3: {
            BtlBadgeRankingEffect* self = task->data;

            CombatSprite_Release(&self->sprite);
            return 1;
        }
    }
}
