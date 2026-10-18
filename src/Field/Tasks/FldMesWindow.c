#include "Field/Tasks/FldMesWindow.h"
#include "Engine/EasyTask.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldEncounter.h"
#include "Field/Tasks/Task_Window.h"
#include "Util/SysFont.h"
#include "nitro/types.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ char   unk_40[0xC];
    /* 0x4C */ s32    posX; // 12-bit fixed point
    /* 0x50 */ s32    posY; // 12-bit fixed point
    /* 0x54 */ char   unk_54[0x8];
    /* 0x5C */ s16    timerA;
    /* 0x5E */ s16    timerB;
    /* 0x60 */ u8     state;
    /* 0x61 */ char   unk_61[0x3];
    /* 0x64 */ s32    unk_64;
} FldMesWindowState; // Size: 0x68

extern const TaskHandle Task_Window;

static const s16 FldMesWindow_PositionX[4][3] = {
    {0x88, 0x78, 0x64},
    {0x90, 0x70, 0x5C},
    {0x98, 0x68, 0x54},
    {0x9C, 0x64, 0x50},
};

static s16 FldMesWindow_GetX(u32 arg0, s32 arg1) {
    if (Field_HasBustShotsOnBothSides() != 0) {
        return 0x80;
    }
    if (arg0 > 3) {
        arg0 = 3;
    }
    return FldMesWindow_PositionX[arg0][arg1];
}

static s16 FldMesWindow_GetXForAnchor(u32 arg0, s32 arg1) {
    s32 row;

    if (arg1 < 0x80) {
        row = 0;
    } else if (func_ov030_020aad28() != 0) {
        row = 2;
    } else {
        row = 1;
    }
    return FldMesWindow_GetX(arg0, row);
}

static s16 FldMesWindow_OffsetX(u32 arg0, s32 arg1) {
    s16 result;

    if (arg1 < 0x80) {
        result = arg1 + (arg0 * 8);
    } else {
        result = arg1 - (arg0 * 8);
    }
    return result;
}

// Nonmatching
s32 FldMesWindow_Create(TaskPool* pool, s32 datSlot, s32 arg2, s16 arg3, s32 arg4, s16 arg5, s32 mesNo, u32 winKind, s16 arg8,
                        u8 arg9, u16 arg10, u16 arg11) {
    FldMesTaskParam param;
    u32             id      = winKind;
    s32             flipped = 0;

    param.datSlot = datSlot;
    param.mesNo   = mesNo;
    param.unk_24  = arg10;
    param.unk_1B  = arg9;
    param.unk_20  = 0;
    param.unk_1C  = 0;
    param.unk_14  = arg11;
    if (winKind == 0x5D) {
        id      = 0xE;
        flipped = 1;
    }

    if (id >= 0x1F) {
        param.unk_1C = 1;
        id           = (u8)(id - 0x1F);
    }
    param.winKind = id;
    if (id >= 0x16) {
        if (arg2 < 0x80) {
            if ((u8)id == 0x1A) {
                param.winKind = 0x19;
            } else if ((u8)id == 0x1C) {
                param.winKind = 0x1B;
            } else if ((u8)id == 0x1E) {
                param.winKind = 0x1D;
            }
        }
        param.unk_19 = 1;
    } else {
        if (arg2 < 0x80) {
            if ((u8)id == 8) {
                param.winKind = 7;
            } else if ((u8)id == 10) {
                param.winKind = 9;
            } else if ((u8)id == 12) {
                param.winKind = 0xB;
            } else if ((u8)id == 14) {
                param.winKind = 0xD;
            } else if ((u8)id == 16) {
                param.winKind = 0xF;
            } else if ((u8)id == 18) {
                param.winKind = 0x11;
            } else if ((u8)id == 20) {
                param.winKind = 0x13;
            }
        }
        param.unk_19 = func_ov030_020ca778((SysFont*)param.unk_20, param.mesNo, 0, &param.winKind);
    }
    param.unk_08.sx = arg4;
    param.unk_08.sy = arg5;
    if (flipped != 0) {
        param.unk_08.sx = FldMesWindow_OffsetX(param.unk_19, arg2);
    } else if (arg4 == -1022) {
        param.unk_08.sx = FldMesWindow_GetXForAnchor(param.unk_19, arg2);
    } else if (arg4 == -1024) {
        param.unk_08.sx = FldMesWindow_GetX(param.unk_19, 0);
    } else if (arg4 == -1023) {
        param.unk_08.sx = FldMesWindow_GetX(param.unk_19, 1);
    } else {
        param.unk_08.sx = Field_ResolvePresetCoord(arg4);
    }
    param.unk_04.sx = Field_ResolvePresetCoord(arg2);
    param.unk_16    = arg8;
    param.unk_0C    = 0xA;
    param.unk_04.sy = arg3;
    param.unk_1A    = 0;
    return EasyTask_CreateTask(pool, &Task_Window, NULL, 0, NULL, &param);
}

s32 FldMesWindow_CreateMessage(TaskPool* pool, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, u8 arg7, s16 arg8,
                               u8 arg9, u16 arg10) {
    return FldMesWindow_Create(pool, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, 1, arg10);
}

// Sets the entity sprite's rendering-mode bits (Unk_Bitfield.unk_10).
void FldMesWindow_SetPriority(Task* task, s32 mode) {
    FldMesWindowState* entity;

    if (task == NULL) {
        return;
    }
    entity                       = task->data;
    entity->sprite.unk_0A.unk_10 = mode;
}

void FldMesWindow_SkipAnim(Task* task) {
    FldMesWindowState* entity;

    if (task == NULL) {
        return;
    }
    entity = task->data;
    if (entity->timerA > 1) {
        entity->timerA = 1;
    }
    entity->timerB = 0;
}

void FldMesWindow_ScrollUp(Task* task, s16 arg1) {
    FldMesWindowState* entity;
    s16                step;
    s32                y;

    step = arg1;
    if (task == NULL) {
        return;
    }
    if (FieldEvent_IsDebugSkipHeld() != 0) {
        step *= 3;
    }
    entity       = task->data;
    y            = entity->posY - (step << 0xC);
    entity->posY = y;
    if (y < 0) {
        entity->posY = -0x80000;
    }
    entity->timerA = 8;
}

void FldMesWindow_Move(Task* task, s32 arg1, s32 arg2) {
    FldMesWindowState* entity;
    s32                y;

    if (task == NULL) {
        return;
    }
    entity       = task->data;
    entity->posX = entity->posX + (arg1 << 0xC);
    y            = entity->posY + (arg2 << 0xC);
    entity->posY = y;
    if (y < 0) {
        entity->posY = -0x80000;
    }
    entity->timerA = 8;
}

BOOL FldMesWindow_IsAnimating(Task* task) {
    if (task == NULL) {
        return FALSE;
    }
    FldMesWindowState* entity = task->data;
    if (entity->timerA != 0) {
        return TRUE;
    }
    if (entity->timerB > 0) {
        return TRUE;
    }
    return FALSE;
}

void FldMesWindow_Close(Task* task) {
    if (task != NULL) {
        FldMesWindowState* entity = task->data;
        entity->state             = 2;
        entity->timerB            = 8;
        entity->unk_64            = 0;
    }
}

void FldMesWindow_Highlight(Task* task) {
    if (task != NULL) {
        FldMesWindowState* entity = task->data;
        entity->state             = 5;
        entity->timerB            = 6;
        entity->unk_64            = 0;
    }
}

void FldMesWindow_Press(Task* task) {
    if (task != NULL) {
        FldMesWindowState* entity = task->data;

        entity->state = 6;
        if (FldMesWindow_IsAnimating(task) == FALSE) {
            entity->timerB = 6;
            entity->unk_64 = 0;
        }
    }
}
