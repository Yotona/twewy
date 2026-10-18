#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/FldBadge.h"
#include "Field/Ov030Data.h"
#include "Field/TaskHelpers.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include "Util/SysFont.h"

// USA 0x020b3d00-0x020b45b4: message-window / badge placement helpers. The
// 020b3d88 entry point spawns a message window task through Task_Window after
// normalizing the window kind and picking an X position from the table at
// data_ov030_020e0864. The task stages (020b4304/4340/4414/445c) drive a
// Sprite embedded at offset 0 of a 0x50-byte task data block.

typedef struct {
    /* 0x00 */ s32          dataType;
    /* 0x04 */ FldScreenPos pos;
    /* 0x08 */ s32          unk_08;
    /* 0x0C */ u16          unk_0C;
} FldBadgeArgs; // Size: 0x10

typedef struct {
    /* 0x00 */ Sprite       sprite;
    /* 0x40 */ s32          pressed;
    /* 0x44 */ s32          held;
    /* 0x48 */ FldScreenPos pos;
    /* 0x4C */ s32          visible;
} FldBadgeItem; // Size: 0x50

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
} FldBadgeEntity; // Size: 0x68

extern const char       data_ov030_02106524[];
extern const TaskHandle Task_Window;

extern s32  Field_HasBustShotsOnBothSides(void);
extern s32  func_ov030_020aad28(void);
extern BOOL FieldEvent_IsDebugSkipHeld(void);

extern u8 func_ov030_020ca778(SysFont* font, s32 mesNo, s32 arg2, const u8* winKind);

s16              func_ov030_020b3d00(u32 arg0, s32 arg1);
s16              func_ov030_020b3d40(u32 arg0, s32 arg1);
s16              func_ov030_020b3d70(u32 arg0, s32 arg1);
void             func_ov030_020b400c(Task* task, s32 mode);
void             func_ov030_020b4038(Task* task);
void             func_ov030_020b4060(Task* task, s16 arg1);
void             func_ov030_020b40ac(Task* task, s32 arg1, s32 arg2);
s32              func_ov030_020b40e8(Task* task);
void             func_ov030_020b411c(Task* task);
void             func_ov030_020b4144(Task* task);
void             func_ov030_020b416c(Task* task);
SpriteFrameInfo* func_ov030_020b41a4(Sprite* sprite, s32 arg, s32 mode);
void             func_ov030_020b4240(Sprite* sprite, FldBadgeArgs* args);
s32              func_ov030_020b4304(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020b4340(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020b4414(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020b445c(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020b4470(TaskPool* pool, Task* task, void* args, s32 stage);
s32              func_ov030_020b44b8(TaskPool* pool, s32 arg1, s16 arg2, s16 arg3, s32 arg4, u16 arg5);
s32              func_ov030_020b4500(Task* task);
s32              func_ov030_020b4514(Task* task);
s32              func_ov030_020b4570(Task* task, s16 anim);
void             func_ov030_020b458c(Task* task, s32 visible);
void             func_ov030_020b4598(Task* task, u16 mode);

// Per-(kind, row) X positions consulted by func_ov030_020b3d00.
const s16 data_ov030_020e0864[4][3] = {
    {0x88, 0x78, 0x64},
    {0x90, 0x70, 0x5C},
    {0x98, 0x68, 0x54},
    {0x9C, 0x64, 0x50},
};

const TaskHandle data_ov030_020e087c = {data_ov030_02106524, func_ov030_020b4470, sizeof(FldBadgeItem)};

const TaskStages data_ov030_020e0888 = {
    {func_ov030_020b4304, func_ov030_020b4340, func_ov030_020b4414, func_ov030_020b445c}
};

const SpriteAnimation data_ov030_020e0898 = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 1,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = func_ov030_020b41a4,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 7,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

s16 func_ov030_020b3d00(u32 arg0, s32 arg1) {
    if (Field_HasBustShotsOnBothSides() != 0) {
        return 0x80;
    }
    if (arg0 > 3) {
        arg0 = 3;
    }
    return data_ov030_020e0864[arg0][arg1];
}

s16 func_ov030_020b3d40(u32 arg0, s32 arg1) {
    s32 row;

    if (arg1 < 0x80) {
        row = 0;
    } else if (func_ov030_020aad28() != 0) {
        row = 2;
    } else {
        row = 1;
    }
    return func_ov030_020b3d00(arg0, row);
}

s16 func_ov030_020b3d70(u32 arg0, s32 arg1) {
    s16 result;

    if (arg1 < 0x80) {
        result = arg1 + (arg0 * 8);
    } else {
        result = arg1 - (arg0 * 8);
    }
    return result;
}

// Spawns a message window task through Task_Window. winKind 0x5D is the
// "flipped" layout sentinel; kinds >= 0x1F wrap by subtracting 0x1F. When the
// Y position argument is below 0x80, even window kinds are remapped onto their
// odd sibling.
// Nonmatching: register allocation only (target keeps winKind in r10 and pool in r11, so its prologue saves two
// more callee-saved registers than ours and shifts every stack-arg offset by 8; ours packs winKind into scratch
// r12, pool in r8 and loads arg4 late into r5) (USA 90.04%; JP unvalidated).
s32 func_ov030_020b3d88(TaskPool* pool, s32 datSlot, s32 arg2, s16 arg3, s32 arg4, s16 arg5, s32 mesNo, u32 winKind, s16 arg8,
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
        param.unk_08.sx = func_ov030_020b3d70(param.unk_19, arg2);
    } else if (arg4 == -1022) {
        param.unk_08.sx = func_ov030_020b3d40(param.unk_19, arg2);
    } else if (arg4 == -1024) {
        param.unk_08.sx = func_ov030_020b3d00(param.unk_19, 0);
    } else if (arg4 == -1023) {
        param.unk_08.sx = func_ov030_020b3d00(param.unk_19, 1);
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

s32 func_ov030_020b3fbc(TaskPool* pool, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, u8 arg7, s16 arg8, u8 arg9,
                        u16 arg10) {
    return func_ov030_020b3d88(pool, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, 1, arg10);
}

// Sets the entity sprite's rendering-mode bits (Unk_Bitfield.unk_10).
void func_ov030_020b400c(Task* task, s32 mode) {
    FldBadgeEntity* entity;

    if (task == NULL) {
        return;
    }
    entity                       = task->data;
    entity->sprite.unk_0A.unk_10 = mode;
}

void func_ov030_020b4038(Task* task) {
    FldBadgeEntity* entity;

    if (task == NULL) {
        return;
    }
    entity = task->data;
    if (entity->timerA > 1) {
        entity->timerA = 1;
    }
    entity->timerB = 0;
}

void func_ov030_020b4060(Task* task, s16 arg1) {
    FldBadgeEntity* entity;
    s16             step;
    s32             y;

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

void func_ov030_020b40ac(Task* task, s32 arg1, s32 arg2) {
    FldBadgeEntity* entity;
    s32             y;

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

s32 func_ov030_020b40e8(Task* task) {
    FldBadgeEntity* entity;

    if (task == NULL) {
        return 0;
    }
    entity = task->data;
    if (entity->timerA != 0) {
        return 1;
    }
    if (entity->timerB > 0) {
        return 1;
    }
    return 0;
}

void func_ov030_020b411c(Task* task) {
    FldBadgeEntity* entity;

    if (task == NULL) {
        return;
    }
    entity         = task->data;
    entity->state  = 2;
    entity->timerB = 8;
    entity->unk_64 = 0;
}

void func_ov030_020b4144(Task* task) {
    FldBadgeEntity* entity;

    if (task == NULL) {
        return;
    }
    entity         = task->data;
    entity->state  = 5;
    entity->timerB = 6;
    entity->unk_64 = 0;
}

void func_ov030_020b416c(Task* task) {
    FldBadgeEntity* entity;

    if (task == NULL) {
        return;
    }
    entity        = task->data;
    entity->state = 6;
    if (func_ov030_020b40e8(task) != 0) {
        return;
    }
    entity->timerB = 6;
    entity->unk_64 = 0;
}

SpriteFrameInfo* func_ov030_020b41a4(Sprite* sprite, s32 arg, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case SPRITE_FRAME_UPDATE:
            g_SpriteFrameInfo.updateSteps = 1;
            return &g_SpriteFrameInfo;
        case SPRITE_FRAME_RENDER: {
            SpriteFrameInfo* frameInfo = &g_SpriteFrameInfo;

            Sprite_FillFrameInfo(frameInfo, sprite);
            info = frameInfo;
        }
        default:
            return info;
    }
}

// Copies the data_ov030_020e0898 template and patches it from the spawn args
// before loading the sprite.
void func_ov030_020b4240(Sprite* sprite, FldBadgeArgs* args) {
    SpriteAnimation anim = data_ov030_020e0898;

    anim.dataType = args->dataType;
    anim.posX     = args->pos.sx - 0x10;
    anim.posY     = args->pos.sy - 0x10;
    anim.bits_0_1 = args->unk_0C;
    if (args->unk_08 == 1) {
        anim.unk_1C = 4;
        anim.unk_20 = 8;
        anim.unk_26 = 5;
        anim.unk_28 = 6;
    }
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020b4304(TaskPool* pool, Task* task, void* args) {
    FldBadgeItem* item = task->data;
    FldBadgeArgs* a    = args;

    item->pressed = 0;
    item->held    = 0;
    item->pos     = a->pos;
    item->visible = 1;
    func_ov030_020b4240(&item->sprite, a);
    return 1;
}

s32 func_ov030_020b4340(TaskPool* pool, Task* task, void* args) {
    FldBadgeItem* item = task->data;

    if (TouchInput_WasTouchPressed() != 0) {
        if (func_ov030_020b4514(task) != 0) {
            item->pressed = 1;
        } else {
            item->pressed = 0;
        }
    }
    if ((TouchInput_IsTouchActive() != 0) || (TouchInput_WasTouchPressed() != 0)) {
        if ((item->pressed != 0) && (func_ov030_020b4514(task) != 0)) {
            item->held = 1;
        } else {
            item->pressed = 0;
            item->held    = 0;
        }
    } else {
        item->pressed = 0;
        item->held    = 0;
    }
    Sprite_UpdateAndCheck(&item->sprite);
    if ((item->sprite.animIndex == 2) && (item->sprite.frameTimer == 1)) {
        if ((item->sprite.currentFrame - item->sprite.loopFrame) == 3) {
            SndMgr_PlaySEWithPan(0x123, item->sprite.posX);
        }
    }
    return 1;
}

s32 func_ov030_020b4414(TaskPool* pool, Task* task, void* args) {
    FldBadgeItem* item = task->data;

    if ((gSaveData.fieldArea == 0x25) && (item->sprite.bits_0_1 == 0)) {
        return 1;
    }
    if (item->visible != 0) {
        Sprite_Render(&item->sprite);
    }
    return 1;
}

s32 func_ov030_020b445c(TaskPool* pool, Task* task, void* args) {
    FldBadgeItem* item = task->data;

    Sprite_Destroy(&item->sprite);
    return 1;
}

s32 func_ov030_020b4470(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = data_ov030_020e0888;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020b44b8(TaskPool* pool, s32 arg1, s16 arg2, s16 arg3, s32 arg4, u16 arg5) {
    FldBadgeArgs args;

    args.dataType = arg1;
    args.pos.x    = arg2;
    args.pos.y    = arg3;
    args.unk_08   = arg4;
    args.unk_0C   = arg5;
    return EasyTask_CreateTask(pool, &data_ov030_020e087c, NULL, 0, NULL, &args);
}

s32 func_ov030_020b4500(Task* task) {
    if (task == NULL) {
        return 0;
    }
    return ((FldBadgeItem*)task->data)->held;
}

// Nonmatching: dead-store scheduling only (register allocation, load order and the if-converted compare chain are all
// exact; mwcc sinks the two coords[1] stores into the first hole after addlt instead of after ldrltsh, so 2 rows
// differ) (USA 82.61%; JP unvalidated).
s32 func_ov030_020b4514(Task* task) {
    TouchCoord    coords[2];
    FldBadgeItem* item = task->data;
    s32           cx;
    s32           cy;
    s32           x;

    // The coord pair is staged in locals so the coords[0] loads are hoisted and
    // x lands in r1 the way the target allocates it.
    TouchInput_GetCoord(&coords[0]);
    cx          = coords[0].x;
    x           = item->pos.sx;
    cy          = coords[0].y;
    coords[1].x = cx;
    coords[1].y = cy;

    if (((x - 0x10) < cx) && (cx < (x + 0x10)) && ((item->pos.sy - 0x10) < cy) && (cy < (item->pos.sy + 0x10))) {
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_020b4570(Task* task, s16 anim) {
    FldBadgeItem* item = task->data;

    return Sprite_ChangeAnimation(&item->sprite, item->sprite.animData, anim, item->sprite.cellTable);
}

void func_ov030_020b458c(Task* task, s32 visible) {
    FldBadgeItem* item = task->data;

    item->visible = visible;
}

void func_ov030_020b4598(Task* task, u16 mode) {
    FldBadgeItem* item = task->data;

    item->sprite.unk_0A.unk_10 = mode;
}
