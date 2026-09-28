#include "Engine/Core/OamMgr.h"
#include "Interface/Menu/MenuBadge.h"
#include <nitro/fx/fx_division.h>

typedef struct {
    /* 0x000 */ Sprite           sprites[5];
    /* 0x140 */ BOOL             visible[5];
    /* 0x154 */ MenuBadgeObject* menuBadge;
    /* 0x158 */ u16              rotation[5];
    /* 0x162 */ u16              _pad_162;
    /* 0x164 */ s32              scaleX[5];
    /* 0x178 */ s32              scaleY[5];
} MenuBadge_bpGaugeU; // Size: 0x18C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_bpGaugeU_Args;

static SpriteFrameInfo* MenuBadge_bpGaugeU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuBadge_bpGaugeU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bpGaugeU = {"Tsk_MenuBadge_bpGaugeU", MenuBadge_bpGaugeU_RunTask,
                                                  sizeof(MenuBadge_bpGaugeU)};

static const SpriteAnimation MenuBadge_bpGaugeU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = MenuBadge_bpGaugeU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuBadge_BinIdentifiers[5],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

void func_ov043_0209a23c(MenuBadge_bpGaugeU* gauge, s32 index, s32 value, u32 max) {
    if (value == 0) {
        gauge->scaleX[index]  = 41;
        gauge->visible[index] = FALSE;
    } else {
        s32 scale = FX_Divide(value << 0xC, (max >> 1) << 0xC);

        if (scale < 41) {
            gauge->scaleX[index]  = 41;
            gauge->visible[index] = FALSE;
        } else {
            gauge->scaleX[index]  = scale;
            gauge->visible[index] = TRUE;
        }
    }
}

void func_ov043_0209a2a4(MenuBadge_bpGaugeU* gauge) {
    MenuBadgeObject* menuBadge = gauge->menuBadge;

    if (menuBadge->cursorBadge.level == menuBadge->cursorBadge.maxLevel) {
        gauge->visible[1] = FALSE;
        gauge->visible[2] = FALSE;
        gauge->visible[3] = FALSE;
        gauge->visible[4] = TRUE;
        gauge->scaleX[4]  = 0x1000;
        return;
    }

    s32 a   = menuBadge->cursorBadge.battlePP;
    s32 b   = menuBadge->cursorBadge.shutdownPP;
    s32 c   = menuBadge->cursorBadge.minglePP;
    u32 max = menuBadge->cursorBadge.maxPP;

    s32 total = (c + (a + b)) << 0xC;
    if (total > 0) {
        s32 cWeighted = (c << 0xC) * 0x14;
        s32 bWeighted = (b << 0xC) * 9;
        s32 aWeighted = a << 0xC;
        s32 ratio     = FX_Divide(total, cWeighted + (bWeighted + (a << 0xC)));

        a = (s32)(((s64)aWeighted * ratio + 0x800) >> 0xC) >> 0xC;
        b = (s32)(((s64)bWeighted * ratio + 0x800) >> 0xC) >> 0xC;
        c = (s32)(((s64)cWeighted * ratio + 0x800) >> 0xC) >> 0xC;
    }

    func_ov043_0209a23c(gauge, 1, a, max);
    func_ov043_0209a23c(gauge, 2, b + a, max);
    func_ov043_0209a23c(gauge, 3, a + (c + b), max);
    gauge->visible[4] = FALSE;
}

static SpriteFrameInfo* MenuBadge_bpGaugeU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void MenuBadge_bpGaugeU_Load(MenuBadge_bpGaugeU* gauge, Sprite* sprites, MenuBadge_bpGaugeU_Args* args) {
    SpriteAnimation anim = MenuBadge_bpGaugeU_Anim;

    anim.dataType  = (u16)args->dataType;
    anim.posX      = 0x8E;
    anim.posY      = 0x4D;
    anim.animIndex = 0x24;
    anim.unk_02.raw |= 2;
    _Sprite_Load(&sprites[0], &anim);

    anim.posX      = 0x8F;
    anim.posY      = 0x49;
    anim.animIndex = 0x1C;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 0x1D;
    _Sprite_Load(&sprites[2], &anim);

    anim.animIndex = 0x1B;
    _Sprite_Load(&sprites[3], &anim);

    anim.animIndex = 0x23;
    _Sprite_Load(&sprites[4], &anim);

    for (s32 i = 0; i < 5; i++) {
        gauge->rotation[i] = 0;
        gauge->scaleX[i]   = 0x1000;
        gauge->scaleY[i]   = 0x1000;
        gauge->visible[i]  = TRUE;
    }

    gauge->visible[0] = TRUE;
    gauge->visible[4] = FALSE;
    func_ov043_0209a2a4(gauge);
}

static s32 MenuBadge_bpGaugeU_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bpGaugeU*      gauge     = task->data;
    MenuBadge_bpGaugeU_Args* gaugeArgs = args;

    gauge->menuBadge = gaugeArgs->menuBadge;
    MenuBadge_bpGaugeU_Load(gauge, gauge->sprites, gaugeArgs);
    return 1;
}

static s32 MenuBadge_bpGaugeU_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bpGaugeU* gauge = task->data;

    func_ov043_0209a2a4(gauge);

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&gauge->sprites[i]);
    }
    return 1;
}

// Nonmatching: regswaps
static s32 MenuBadge_bpGaugeU_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bpGaugeU* gauge = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite* sprite = &gauge->sprites[i];
        u32     affine = (u16)OamMgr_AllocAffineGroup(&g_OamMgr[1], gauge->rotation[i], gauge->scaleX[i], gauge->scaleY[i], 0);

        sprite->unk_0A.raw = (sprite->unk_0A.raw & ~1) | 1;
        sprite->unk_0A.raw = (sprite->unk_0A.raw & ~0x3E0) | ((affine << 0x1B) >> 0x16);

        if (gauge->visible[i] != 0) {
            Sprite_RenderFrame(sprite);
        }
    }
    return 1;
}

static s32 MenuBadge_bpGaugeU_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bpGaugeU* gauge = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&gauge->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_bpGaugeU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bpGaugeU_Init,
        .update     = MenuBadge_bpGaugeU_Update,
        .render     = MenuBadge_bpGaugeU_Render,
        .cleanup    = MenuBadge_bpGaugeU_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bpGaugeU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_bpGaugeU_Args args;

    args.dataType  = dataType;
    args.menuBadge = badge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bpGaugeU, NULL, 0, NULL, &args);
}
