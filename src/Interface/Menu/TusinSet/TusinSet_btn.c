#include "Engine/Core/System.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Interface/Menu/TusinSet.h"
#include "SndMgr.h"
#include "SndMgrSeIdx.h"

typedef struct {
    /* 0x00 */ Sprite          sprite;
    /* 0x40 */ BOOL            visible;
    /* 0x44 */ TusinSetObject* tusinSet;
    /* 0x48 */ s16             unk_48;
    /* 0x4A */ char            unk_4A[0x4C - 0x4A];
} TusinSet_btn; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_btn_Args;

static SpriteFrameInfo* TusinSet_btn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_btn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_btn = {"Tsk_TusinSet_btn", TusinSet_btn_RunTask, sizeof(TusinSet_btn)};

static const SpriteAnimation TusinSet_btn_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xc00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = TusinSet_btn_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 7,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* TusinSet_btn_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_btn_Load(TusinSet_btn* btn, TusinSet_btn_Args* args) {
    SpriteAnimation anim = TusinSet_btn_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 28;
    anim.posX      = 232;
    anim.posY      = 175;
    _Sprite_Load(&btn->sprite, &anim);
}

static s32 TusinSet_btn_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_btn*      btn      = task->data;
    TusinSet_btn_Args* initArgs = args;

    btn->visible  = TRUE;
    btn->tusinSet = initArgs->tusinSet;
    btn->unk_48   = 0;
    TusinSet_btn_Load(btn, initArgs);
    return 1;
}

static s32 TusinSet_btn_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_btn*   btn      = task->data;
    TusinSetObject* tusinSet = btn->tusinSet;
    TouchCoord      touch;

    if (tusinSet->helpOpen != 0) {
        return 1;
    }
    if (tusinSet->flags & 0x1000) {
        return 1;
    }
    if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&touch);
        if (TusinSet_IsPointOnBtn(touch.x, touch.y) == 1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
            SystemStatusFlags;
            SystemStatusFlags.unk_06 = FALSE;
            TusinSet_SetSpriteFrame(&btn->sprite, 29);
            tusinSet->flags |= 0x1000;
            tusinSet->nextProcess = 1;
            DebugOvlDisp_Pop();
        }
    }
    Sprite_Update(&btn->sprite);
    return 1;
}

static s32 TusinSet_btn_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_btn* btn = task->data;

    Sprite_RenderFrame(&btn->sprite);
    return 1;
}

static s32 TusinSet_btn_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_btn* btn = task->data;

    Sprite_Release(&btn->sprite);
    return 1;
}

static s32 TusinSet_btn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_btn_Init,
        .update     = TusinSet_btn_Update,
        .render     = TusinSet_btn_Render,
        .cleanup    = TusinSet_btn_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_btn_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_btn_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_btn, NULL, 0, NULL, &args);
}
