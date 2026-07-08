#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/MenuBadge.h"
#include "SndMgr.h"
#include <nitro/fx/fx_division.h>

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ s32              unk_C0;
    /* 0xC4 */ MenuBadgeObject* unk_C4;
    /* 0xC8 */ u16              unk_C8;
} MenuBadge_sbar; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
} MenuBadge_sbar_Args;

static SpriteFrameInfo* func_ov043_02094854(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              func_ov043_02094d78(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle data_ov043_020c89dc = {"Tsk_MenuBadge_sbar", func_ov043_02094d78, sizeof(MenuBadge_sbar)};

static const SpriteAnimation data_ov043_020c89f8 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = func_ov043_02094854,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
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

static SpriteFrameInfo* func_ov043_02094854(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info = temp;
        } break;
    }

    return info;
}

static void func_ov043_020948f0(Sprite* sprites, MenuBadge_sbar_Args* args) {
    SpriteAnimation anim = data_ov043_020c89f8;

    anim.dataType = args->dataType;

    anim.unk_2A = 0x20;
    anim.unk_04 = 0xF8;
    anim.unk_06 = 0x65;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x1E;
    anim.unk_04 = 0xF8;
    anim.unk_06 = 0x4A;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = 0x1F;
    anim.unk_04 = 0xF8;
    anim.unk_06 = 0xAA;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 func_ov043_020949bc(TaskPool* pool, Task* task, void* args) {
    MenuBadge_sbar_Args* initArgs = args;
    MenuBadgeObject*     owner    = initArgs->unk_4;

    MenuBadge_sbar* sbar = task->data;

    sbar->unk_C4 = owner;
    sbar->unk_C8 = owner->unk_AEFA;

    func_ov043_020948f0(sbar->sprites, initArgs);
    return 1;
}

static s32 func_ov043_020949e8(TaskPool* pool, Task* task, void* args) {
    MenuBadge_sbar*  sbar  = task->data;
    MenuBadgeObject* owner = sbar->unk_C4;
    TouchCoord       coord;

    if (owner->unk_AF20 != 0) {
        return 1;
    }
    if (owner->unk_AEE8 & 1) {
        return 1;
    }

    if (owner->unk_AEE8 & 8) {
        TouchInput_GetCoord(&coord);
        owner->unk_AEFA = func_ov043_020947bc((u16)(coord.y + 0xC), owner->unk_AEEB);
        owner->unk_AEE8 &= ~8;
        owner->unk_AEE8 |= 0x10;

        sbar->sprites[0].posY = func_ov043_02094818(owner->unk_AEFA, owner->unk_AEEB);
    }

    if (owner->unk_AEE8 & 0x10) {
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

        owner->unk_AEFA = func_ov043_020947bc(sbar->sprites[0].posY, owner->unk_AEEB);

        if (TouchInput_WasTouchReleased() != 0) {
            owner->unk_AEE8 &= ~0x10;
        }

        if (owner->unk_AEF6 >= 6U) {
            s32 diff = owner->unk_AEF8 - owner->unk_AEFA;

            if (diff < 0) {
                owner->unk_AEF6 = (owner->unk_AEF8 % 8) + 6;
            } else if (diff < 0x18) {
                owner->unk_AEF6 = diff + 6;
            } else {
                owner->unk_AEF6 = (diff % 8) + 0x16;
            }
        }
    } else {
        if (TouchInput_WasTouchPressed() != 0) {
            TouchInput_GetCoord(&coord);
            if (func_ov043_02091760(coord.x, coord.y) == 1) {
                if (func_ov043_020917c0(coord.x, coord.y, sbar->sprites[0].posX, sbar->sprites[0].posY) == 1) {
                    owner->unk_AEE8 |= 0x10;
                } else {
                    owner->unk_AEE8 |= 8;
                }
            } else {
                s32 arrow = func_ov043_02091820(coord.x, coord.y);

                if (arrow == 0) {
                    u16 scroll = owner->unk_AEFA;

                    if (scroll >= 8U) {
                        owner->unk_AEFA = scroll - 8;

                        if (owner->unk_AEF6 >= 6U && owner->unk_AEF6 < 0x16U) {
                            owner->unk_AEF6 = owner->unk_AEF6 + 8;
                        }
                    }
                } else if (arrow == 1) {
                    s32 maxSteps;
                    u16 scroll;

                    if (owner->unk_AEEB == 0) {
                        maxSteps = 0x1D;
                    } else {
                        maxSteps = 0x23;
                    }

                    scroll = owner->unk_AEFA;
                    if (scroll <= (maxSteps * 8) - 8) {
                        owner->unk_AEFA = scroll + 8;

                        if (owner->unk_AEF6 >= 0xEU && owner->unk_AEF6 < 0x1EU) {
                            owner->unk_AEF6 = owner->unk_AEF6 - 8;
                        }
                    }
                }
            }
        }

        sbar->sprites[0].posY = func_ov043_02094818(owner->unk_AEFA, owner->unk_AEEB);
    }

    if (sbar->unk_C8 != owner->unk_AEFA) {
        SndMgr_StartPlayingSE(0x119);
    }
    sbar->unk_C8 = owner->unk_AEFA;

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

s32 func_ov043_02094dc0(TaskPool* pool, s32 dataType, MenuBadgeObject* owner) {
    MenuBadge_sbar_Args args;

    args.dataType = dataType;
    args.unk_4    = owner;

    return EasyTask_CreateTask(pool, &data_ov043_020c89dc, NULL, 0, NULL, &args);
}
