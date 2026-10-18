#include "Field/Tasks/Fld_Badge.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Event/EventTables.h"
#include "Field/FieldTypes.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

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

static SpriteFrameInfo* Fld_Badge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_Badge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Badge = {"Tsk_Fld_Badge", Fld_Badge_RunTask, sizeof(FldBadgeItem)};

static const SpriteAnimation Fld_Badge_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_Badge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
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

static SpriteFrameInfo* Fld_Badge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
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

static void Fld_Badge_Load(Sprite* sprite, FldBadgeArgs* args) {
    SpriteAnimation anim = Fld_Badge_Anim;

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

static s32 Fld_Badge_Init(TaskPool* pool, Task* task, void* args) {
    FldBadgeItem* item = task->data;
    FldBadgeArgs* a    = args;

    item->pressed = 0;
    item->held    = 0;
    item->pos     = a->pos;
    item->visible = 1;
    Fld_Badge_Load(&item->sprite, a);
    return 1;
}

static s32 Fld_Badge_Update(TaskPool* pool, Task* task, void* args) {
    FldBadgeItem* item = task->data;

    if (TouchInput_WasTouchPressed() != 0) {
        if (Fld_Badge_IsTouched(task) != 0) {
            item->pressed = 1;
        } else {
            item->pressed = 0;
        }
    }
    if ((TouchInput_IsTouchActive() != 0) || (TouchInput_WasTouchPressed() != 0)) {
        if ((item->pressed != 0) && (Fld_Badge_IsTouched(task) != 0)) {
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

static s32 Fld_Badge_Render(TaskPool* pool, Task* task, void* args) {
    FldBadgeItem* item = task->data;

    if ((gSaveData.fieldArea == 0x25) && (item->sprite.bits_0_1 == 0)) {
        return 1;
    }
    if (item->visible != 0) {
        Sprite_Render(&item->sprite);
    }
    return 1;
}

static s32 Fld_Badge_Destroy(TaskPool* pool, Task* task, void* args) {
    FldBadgeItem* item = task->data;

    Sprite_Destroy(&item->sprite);
    return 1;
}

static s32 Fld_Badge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Badge_Init,
        .update     = Fld_Badge_Update,
        .render     = Fld_Badge_Render,
        .cleanup    = Fld_Badge_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Badge_CreateTask(TaskPool* pool, s32 arg1, s16 arg2, s16 arg3, s32 arg4, u16 arg5) {
    FldBadgeArgs args;

    args.dataType = arg1;
    args.pos.x    = arg2;
    args.pos.y    = arg3;
    args.unk_08   = arg4;
    args.unk_0C   = arg5;
    return EasyTask_CreateTask(pool, &Tsk_Fld_Badge, NULL, 0, NULL, &args);
}

s32 Fld_Badge_IsHeld(Task* task) {
    if (task == NULL) {
        return 0;
    }
    return ((FldBadgeItem*)task->data)->held;
}

// Nonmatching
s32 Fld_Badge_IsTouched(Task* task) {
    TouchCoord    coords[2];
    FldBadgeItem* item = task->data;
    s32           cx;
    s32           cy;
    s32           x;

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

s32 Fld_Badge_SetAnimation(Task* task, s16 anim) {
    FldBadgeItem* item = task->data;

    return Sprite_ChangeAnimation(&item->sprite, item->sprite.animData, anim, item->sprite.cellTable);
}

void Fld_Badge_SetVisible(Task* task, s32 visible) {
    FldBadgeItem* item = task->data;

    item->visible = visible;
}

void Fld_Badge_SetPriority(Task* task, u16 mode) {
    FldBadgeItem* item = task->data;

    item->sprite.unk_0A.unk_10 = mode;
}
