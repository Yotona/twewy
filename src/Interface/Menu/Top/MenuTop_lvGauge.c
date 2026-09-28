#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Top.h"
#include <nitro/fx.h>

typedef struct {
    /* 0x000 */ Sprite         sprites[5];
    /* 0x140 */ BOOL           visible[5];
    /* 0x154 */ MenuTopObject* topMenu;
    /* 0x158 */ u16            rotation;
    /* 0x15A */ u16            _pad_15A;
    /* 0x15C */ s32            scaleX;
    /* 0x160 */ s32            scaleY;
} MenuTop_lvGauge; // Size: 0x164

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
} MenuTop_lvGauge_Args;

static SpriteFrameInfo* MenuTop_lvGauge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_lvGauge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_lvGauge = {"Tsk_MenuTop_lvGauge", MenuTop_lvGauge_RunTask, sizeof(MenuTop_lvGauge)};

static const SpriteAnimation MenuTop_lvGauge_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuTop_lvGauge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[3],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 4,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static void MenuTop_lvGauge_SetKnobFromLevel(MenuTopObject* topMenu) {
    s16 level = topMenu->currentLevel;
    s16 max   = topMenu->maxLevel;
    s16 x;

    if (level == max) {
        x = 238;
    } else {
        x = ((s32)((level - 1) * FX_Divide(0xC8000, (max - 1) << 12)) >> 12) + 0x26;
    }

    topMenu->levelKnobX = x;
    topMenu->levelKnobY = 0x98;
}

// Nonmatching
static void MenuTop_lvGauge_SetLevelFromKnob(MenuTopObject* topMenu) {
    s16 max = topMenu->maxLevel;
    s16 x   = topMenu->levelKnobX;

    if (max == 1) {
        topMenu->currentLevel = 1;
        topMenu->levelKnobX   = 238;

        if (TouchInput_WasTouchReleased() == 0) {
            return;
        }

        topMenu->flags &= ~1;
        topMenu->flags &= ~4;
        return;
    }

    u32 step  = FX_Divide(0xC8000, (max - 1) << 12);
    u32 base  = x - 0x26;
    u32 index = FX_Divide(base << 12, step) >> 12;

    if (((base << 12) - (step * index)) > ((s32)(step + (step >> 31)) >> 1)) {
        topMenu->currentLevel = index + 2;
    } else {
        topMenu->currentLevel = index + 1;
    }

    if (topMenu->currentLevel > topMenu->maxLevel) {
        topMenu->currentLevel = topMenu->maxLevel;
    } else if (topMenu->currentLevel < 1) {
        topMenu->currentLevel = 1;
    }

    if (TouchInput_WasTouchReleased()) {
        MenuTop_lvGauge_SetKnobFromLevel(topMenu);
        topMenu->flags &= ~1;
        topMenu->flags &= ~4;
    }
}

static s32 MenuTop_lvGauge_GetBarScale(MenuTop_lvGauge* gauge, s32 knobX) {
    fx32 x = I2F(knobX);
    fx32 scale;

    if (x <= I2F(38)) {
        gauge->visible[1] = FALSE;
        // BUG: no return value on this path; the caller gets whatever is left in r0
    } else {
        scale             = FX_Divide(x - I2F(38), I2F(100));
        gauge->visible[1] = TRUE;
        return scale;
    }
}

static SpriteFrameInfo* MenuTop_lvGauge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_lvGauge_Load(MenuTop_lvGauge* gauge, Sprite* sprites, MenuTop_lvGauge_Args* args) {
    MenuTopObject*  topMenu = gauge->topMenu;
    SpriteAnimation anim    = MenuTop_lvGauge_Anim;

    anim.dataType = args->dataType;

    MenuTop_lvGauge_SetKnobFromLevel(topMenu);

    anim.unk_02.raw &= ~2;
    anim.animIndex = 0x23;
    anim.posX      = topMenu->levelKnobX;
    anim.posY      = topMenu->levelKnobY;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_02.raw |= 2;
    anim.animIndex = 0x24;
    anim.posX      = 0x26;
    anim.posY      = 0x98;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_02.raw &= ~2;
    anim.animIndex = 0x25;
    anim.posX      = 0x26;
    anim.posY      = 0x98;
    _Sprite_Load(&sprites[2], &anim);

    anim.unk_02.raw &= ~2;
    anim.animIndex = 0x21;
    anim.posX      = 0x21;
    anim.posY      = 0x98;
    _Sprite_Load(&sprites[3], &anim);

    anim.unk_02.raw &= ~2;
    anim.animIndex = 0x22;
    anim.posX      = 0xF3;
    anim.posY      = 0x98;
    _Sprite_Load(&sprites[4], &anim);

    for (s32 i = 0; i < 5; i++) {
        gauge->visible[i] = TRUE;
    }

    gauge->rotation = 0;
    gauge->scaleX   = 0x1000;
    gauge->scaleY   = 0x1000;
}

static s32 MenuTop_lvGauge_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_lvGauge*      gauge    = task->data;
    MenuTop_lvGauge_Args* initArgs = args;

    gauge->topMenu = initArgs->topMenu;
    MenuTop_lvGauge_Load(gauge, gauge->sprites, initArgs);
    return 1;
}

static s32 MenuTop_lvGauge_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_lvGauge* gauge   = task->data;
    MenuTopObject*   topMenu = gauge->topMenu;
    TouchCoord       coord;

    if (topMenu->upperPage != 0) {
        return 1;
    }

    if ((topMenu->flags & 8) != 0) {
        return 1;
    }

    if ((topMenu->flags & 1) != 0 && (topMenu->flags & 4) != 0) {
        TouchInput_GetCoord(&coord);
        topMenu->levelKnobX = (s16)coord.x;

        if (topMenu->levelKnobX > 0xEE) {
            topMenu->levelKnobX = 0xEE;
        } else if (topMenu->levelKnobX < 0x26) {
            topMenu->levelKnobX = 0x26;
        }

        MenuTop_lvGauge_SetLevelFromKnob(topMenu);
    } else {
        if (topMenu->maxLevel <= 1) {
            gauge->visible[0] = FALSE;
        } else {
            gauge->visible[0] = TRUE;
        }

        if (TouchInput_IsTouchActive() != 0) {
            TouchInput_GetCoord(&coord);

            if (MenuTop_IsPointOnLevelGauge((s16)coord.x, (s16)coord.y) == 1) {
                topMenu->flags |= 5;
            } else if (TouchInput_WasTouchPressed() != 0) {
                if (MenuTop_IsPointOnLevelDown((s16)coord.x, (s16)coord.y) != 0) {
                    topMenu->currentLevel--;
                    if (topMenu->currentLevel < 1) {
                        topMenu->currentLevel = 1;
                    }

                    MenuTop_lvGauge_SetKnobFromLevel(topMenu);
                } else if (MenuTop_IsPointOnLevelUp((s16)coord.x, (s16)coord.y) != 0) {
                    topMenu->currentLevel++;
                    if (topMenu->currentLevel > topMenu->maxLevel) {
                        topMenu->currentLevel = topMenu->maxLevel;
                    }

                    MenuTop_lvGauge_SetKnobFromLevel(topMenu);
                }
            }
        }
    }

    gauge->sprites[0].posX = topMenu->levelKnobX;
    gauge->scaleX          = MenuTop_lvGauge_GetBarScale(gauge, gauge->sprites[0].posX);

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&gauge->sprites[i]);
    }

    return 1;
}

// Nonmatching
static s32 MenuTop_lvGauge_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_lvGauge* gauge = task->data;

    u16 affine = OamMgr_AllocAffineGroup(&g_OamMgr[DISPLAY_SUB], gauge->rotation, gauge->scaleX, gauge->scaleY, 0);

    gauge->sprites[0].unk_0A.raw = (gauge->sprites[0].unk_0A.raw & ~1) | 1;
    gauge->sprites[0].unk_0A.raw = (gauge->sprites[0].unk_0A.raw & ~0x3E0) | ((affine & 0x1F) << 5);

    for (s32 i = 0; i < 5; i++) {
        if (gauge->visible[i] != 0) {
            Sprite_RenderFrame(&gauge->sprites[i]);
        }
    }

    return 1;
}

static s32 MenuTop_lvGauge_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_lvGauge* gauge = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&gauge->sprites[i]);
    }

    return 1;
}

static s32 MenuTop_lvGauge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_lvGauge_Init,
        .update     = MenuTop_lvGauge_Update,
        .render     = MenuTop_lvGauge_Render,
        .cleanup    = MenuTop_lvGauge_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_lvGauge_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_lvGauge_Args args;

    args.dataType = dataType;
    args.topMenu  = topMenu;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_lvGauge, NULL, 0, NULL, &args);
}