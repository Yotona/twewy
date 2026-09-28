#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/MenuBadge.h"
#include "SndMgr.h"
#include <nitro/fx/fx_division.h>

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ s32              unk_C0;
    /* 0xC4 */ MenuBadgeObject* menuBadge;
    /* 0xC8 */ u16              prevListTop;
} MenuBadge_sbar; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_sbar_Args;

static SpriteFrameInfo* func_ov043_02094854(Sprite* sprite, s32 arg, s32 mode);
static s32              func_ov043_02094d78(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_sbar = {"Tsk_MenuBadge_sbar", func_ov043_02094d78, sizeof(MenuBadge_sbar)};

static const SpriteAnimation data_ov043_020c89f8 = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = func_ov043_02094854,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuBadge_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 0xD,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

// Nonmatching
static u16 func_ov043_020947bc(u16 posY, u8 mode) {
    u32 y = posY;
    if (y < 101) {
        y = 101;
    }

    u16 maxSteps;
    if (mode == 0) {
        maxSteps = 29;
    } else {
        maxSteps = 35;
    }

    u32 step = (u32)(FX_Divide((y - 101) << 0xC, 0x3E000 / (maxSteps + 1)) * 0x10) >> 0x10;

    if (maxSteps < step) {
        step = maxSteps;
    }
    return step * 8;
}

static u16 func_ov043_02094818(u16 scroll, u8 mode) {
    s32 maxSteps;

    if (mode == 0) {
        maxSteps = 29;
    } else {
        maxSteps = 35;
    }

    u16 y = (s32)(((u32)(scroll << 0xD) >> 0x10) * 62) / maxSteps + 101;
    if (y > 163) {
        y = 163;
    }
    return y;
}

static SpriteFrameInfo* func_ov043_02094854(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void func_ov043_020948f0(Sprite* sprites, MenuBadge_sbar_Args* args) {
    SpriteAnimation anim = data_ov043_020c89f8;

    anim.dataType = args->dataType;

    anim.animIndex = 0x20;
    anim.posX      = 0xF8;
    anim.posY      = 0x65;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 0x1E;
    anim.posX      = 0xF8;
    anim.posY      = 0x4A;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 0x1F;
    anim.posX      = 0xF8;
    anim.posY      = 0xAA;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 func_ov043_020949bc(TaskPool* pool, Task* task, void* args) {
    MenuBadge_sbar_Args* initArgs = args;
    MenuBadgeObject*     owner    = initArgs->menuBadge;

    MenuBadge_sbar* sbar = task->data;

    sbar->menuBadge   = owner;
    sbar->prevListTop = owner->listTop;

    func_ov043_020948f0(sbar->sprites, initArgs);
    return 1;
}

static s32 func_ov043_020949e8(TaskPool* pool, Task* task, void* args) {
    MenuBadge_sbar*  sbar  = task->data;
    MenuBadgeObject* owner = sbar->menuBadge;
    TouchCoord       coord;

    if (owner->windowMessage != 0) {
        return 1;
    }
    if (owner->flags & MENUBADGE_FLAG_DRAGGING) {
        return 1;
    }

    if (owner->flags & MENUBADGE_FLAG_SCROLL_JUMP) {
        TouchInput_GetCoord(&coord);
        owner->listTop = func_ov043_020947bc((u16)(coord.y + 0xC), owner->listMode);
        owner->flags &= ~MENUBADGE_FLAG_SCROLL_JUMP;
        owner->flags |= MENUBADGE_FLAG_SCROLL_DRAG;

        sbar->sprites[0].posY = func_ov043_02094818(owner->listTop, owner->listMode);
    }

    if (owner->flags & MENUBADGE_FLAG_SCROLL_DRAG) {
        TouchInput_GetCoord(&coord);

        s16 posY    = sbar->sprites[0].posY;
        s16 targetY = coord.y + 0xC;
        s16 step    = (targetY - posY) >> 2;

        if (step != 0) {
            sbar->sprites[0].posY = posY + step;
        } else {
            sbar->sprites[0].posY = (u16)targetY;
        }

        posY = sbar->sprites[0].posY;
        if (posY < 101) {
            sbar->sprites[0].posY = 101;
        } else if (posY > 163) {
            sbar->sprites[0].posY = 163;
        }

        owner->listTop = func_ov043_020947bc(sbar->sprites[0].posY, owner->listMode);

        if (TouchInput_WasTouchReleased() != 0) {
            owner->flags &= ~MENUBADGE_FLAG_SCROLL_DRAG;
        }

        if (owner->cursorSlot >= 6U) {
            s32 diff = owner->cursorListIndex - owner->listTop;

            if (diff < 0) {
                owner->cursorSlot = (owner->cursorListIndex % 8) + 6;
            } else if (diff < 0x18) {
                owner->cursorSlot = diff + 6;
            } else {
                owner->cursorSlot = (diff % 8) + 0x16;
            }
        }
    } else {
        if (TouchInput_WasTouchPressed() != 0) {
            TouchInput_GetCoord(&coord);
            if (MenuBadge_IsPointOnScrollBar(coord.x, coord.y) == 1) {
                if (MenuBadge_IsPointOnScrollKnob(coord.x, coord.y, sbar->sprites[0].posX, sbar->sprites[0].posY) == 1) {
                    owner->flags |= MENUBADGE_FLAG_SCROLL_DRAG;
                } else {
                    owner->flags |= MENUBADGE_FLAG_SCROLL_JUMP;
                }
            } else {
                s32 arrow = MenuBadge_GetScrollArrowAtPoint(coord.x, coord.y);

                if (arrow == 0) {
                    u16 scroll = owner->listTop;

                    if (scroll >= 8U) {
                        owner->listTop = scroll - 8;

                        if (owner->cursorSlot >= 6U && owner->cursorSlot < 0x16U) {
                            owner->cursorSlot = owner->cursorSlot + 8;
                        }
                    }
                } else if (arrow == 1) {
                    s32 maxSteps;
                    u16 scroll;

                    if (owner->listMode == 0) {
                        maxSteps = 0x1D;
                    } else {
                        maxSteps = 0x23;
                    }

                    scroll = owner->listTop;
                    if (scroll <= (maxSteps * 8) - 8) {
                        owner->listTop = scroll + 8;

                        if (owner->cursorSlot >= 0xEU && owner->cursorSlot < 0x1EU) {
                            owner->cursorSlot = owner->cursorSlot - 8;
                        }
                    }
                }
            }
        }

        sbar->sprites[0].posY = func_ov043_02094818(owner->listTop, owner->listMode);
    }

    if (sbar->prevListTop != owner->listTop) {
        SndMgr_StartPlayingSE(0x119);
    }
    sbar->prevListTop = owner->listTop;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&sbar->sprites[i]);
    }
    return 1;
}

static s32 func_ov043_02094d20(TaskPool* pool, Task* task, void* args) {
    MenuBadge_sbar* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&sbar->sprites[i]);
    }
    return 1;
}

static s32 func_ov043_02094d4c(TaskPool* pool, Task* task, void* args) {
    MenuBadge_sbar* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&sbar->sprites[i]);
    }
    return 1;
}

static s32 func_ov043_02094d78(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = func_ov043_020949bc,
        .update     = func_ov043_020949e8,
        .render     = func_ov043_02094d20,
        .cleanup    = func_ov043_02094d4c,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_sbar_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* owner) {
    MenuBadge_sbar_Args args;

    args.dataType  = dataType;
    args.menuBadge = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_sbar, NULL, 0, NULL, &args);
}
