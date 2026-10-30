#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/OamMgr.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlTchSlash {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s16          x;
    /* 0x62 */ s16          y;
    /* 0x64 */ s32          scaleX;
    /* 0x68 */ s32          scaleY;
    /* 0x6C */ u16          angle;
    /* 0x6E */ s8           alpha;
    /* 0x6F */ u8           pad_6F;
    /* 0x70 */ s32          fade;
    /* 0x74 */ s32          active;
} BtlTchSlash; // Size: 0x78

extern const BinIdentifier data_ov003_020d6fac;

s32 func_ov003_0208f134(TaskPool* pool, Task* task, void* args) {
    BtlTchSlash*  effect = task->data;
    BtlTouchWork* input  = &data_ov003_020e71b8->touchWork;

    MI_CpuSet(effect, 0, sizeof(*effect));
    effect->active = 1;
    effect->x      = input->startX;
    effect->y      = input->startY;
    effect->scaleX = 0x1000;
    effect->scaleY = input->distance / 64;
    effect->angle  = input->angle + 0xC000;
    effect->alpha  = 31;
    effect->fade   = 0x1000;
    CombatSprite_LoadDirect(2, &effect->sprite, &data_ov003_020d6fac, 1, 3, 2, 4, 9);
    CombatSprite_SetAnim(&effect->sprite, 9, 1);
    CombatSprite_SetPosition(&effect->sprite, effect->x, effect->y);
    func_ov003_02082738(&effect->sprite, 0);
    func_ov003_02082730(&effect->sprite, 0x3F2000);
    if (effect->scaleY < 0x800) {
        func_ov003_02087f00(SEIDX_SE_PENEFFECTS, effect->x);
    } else if (effect->scaleY < 0x1000) {
        func_ov003_02087f00(SEIDX_SE_PENEFFECTM, effect->x);
    } else {
        func_ov003_02087f00(SEIDX_SE_PENEFFECTL, effect->x);
    }
    return 1;
}

s32 func_ov003_0208f260(TaskPool* pool, Task* task, void* args) {
    BtlTchSlash* effect = task->data;

    effect->alpha = (effect->fade * 31) >> 12;
    effect->fade -= 0x7B;
    if (effect->alpha < 0) {
        effect->alpha = 0;
    }
    if (SpriteMgr_IsAnimationFinished(&effect->sprite.sprite)) {
        effect->active = 0;
    }
    CombatSprite_Update(&effect->sprite);
    return effect->active;
}

// Nonmatching: affine-index and bitfield temporaries use different registers.
s32 func_ov003_0208f2b8(TaskPool* pool, Task* task, void* args) {
    BtlTchSlash* effect = task->data;
    u16 affine = OamMgr_AllocAffineGroup(&g_OamMgr[DISPLAY_EXTENDED], effect->angle, effect->scaleX, effect->scaleY, 0);

    effect->sprite.sprite.unk_0A.unk_00 = 1;
    effect->sprite.sprite.unk_0A.unk_05 = affine;
    CombatSprite_RenderWithPalette(&effect->sprite, effect->alpha);
    return 1;
}

s32 func_ov003_0208f328(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

extern const TaskStages data_ov003_020d7934;
s32                     func_ov003_0208f33c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7934;
    return stages.iter[stage](pool, task, args);
}

char             data_ov003_020e3af4[20] = "Tsk_BtlTch_Slash";
const TaskHandle data_ov003_020d7928     = {data_ov003_020e3af4, func_ov003_0208f33c, sizeof(BtlTchSlash)};
const TaskStages data_ov003_020d7934     = {
    func_ov003_0208f134,
    func_ov003_0208f260,
    func_ov003_0208f2b8,
    func_ov003_0208f328,
};
