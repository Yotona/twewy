#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/TusinSet.h"
#include "SndMgr.h"
#include "SndMgrSeIdx.h"

typedef struct {
    /* 0x000 */ Sprite          sprites[5];
    /* 0x140 */ BOOL            visible[5];
    /* 0x154 */ TusinSetObject* tusinSet;
} TusinSet_partner; // Size: 0x158

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_partner_Args;

static SpriteFrameInfo* TusinSet_partner_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_partner = {"Tsk_TusinSet_partner", TusinSet_partner_RunTask, sizeof(TusinSet_partner)};

static const SpriteAnimation TusinSet_partner_Anim = {
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
    .frameInfoCallback = TusinSet_partner_GetFrameInfo,
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

static SpriteFrameInfo* TusinSet_partner_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_partner_Load(TusinSet_partner* partner, Sprite* sprites, TusinSet_partner_Args* args) {
    TusinSetObject* tusinSet = partner->tusinSet;
    SpriteAnimation anim     = TusinSet_partner_Anim;

    anim.dataType = args->dataType;
    for (s16 i = 0; i < 3; i++) {
        anim.animIndex = i + 22;
        anim.posX      = i * 18 + 133;
        anim.posY      = 12;
        _Sprite_Load(&sprites[i], &anim);
    }

    anim.animIndex = 20;
    anim.posX      = tusinSet->partner * 18 + 133;
    anim.posY      = 12;
    _Sprite_Load(&sprites[3], &anim);

    anim.animIndex = 25;
    anim.posX      = 81;
    anim.posY      = 12;
    _Sprite_Load(&sprites[4], &anim);
}

static s32 TusinSet_partner_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_partner*      partner  = task->data;
    TusinSet_partner_Args* initArgs = args;

    partner->tusinSet = initArgs->tusinSet;
    for (u16 i = 0; i < 5; i++) {
        partner->visible[i] = TRUE;
    }
    TusinSet_partner_Load(partner, partner->sprites, initArgs);
    return 1;
}

static s32 TusinSet_partner_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_partner* partner  = task->data;
    TusinSetObject*   tusinSet = partner->tusinSet;
    TouchCoord        touch;

    if (tusinSet->helpOpen != 0) {
        partner->visible[3] = FALSE;
        return 1;
    }

    partner->visible[3] = TRUE;
    if (tusinSet->flags & 0x10) {
        tusinSet->flags &= ~0x10;
    }

    if (TouchInput_WasTouchPressed()) {
        s16 index;

        TouchInput_GetCoord(&touch);
        index = TusinSet_GetPartnerAtPoint(touch.x, touch.y);
        if (index != -1 && tusinSet->partner != index) {
            s32 screenIndex;

            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            partner->sprites[3].posX = index * 18 + 133;
            tusinSet->partner        = index;
            TusinSet_ChangePartner(tusinSet, index);
            if (tusinSet->partner == 0) {
                screenIndex = 0;
            } else if (tusinSet->partner == 1) {
                screenIndex = 4;
            } else {
                screenIndex = 5;
            }
            TusinSet_ReleaseBgResource(&tusinSet->resources[3], DISPLAY_SUB);
            TusinSet_LoadBgResourceIndexed(&tusinSet->resources[3], DISPLAY_SUB, 3, 5, 1, 13, screenIndex, tusinSet->partner);
        }
    }

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&partner->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_partner_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_partner* partner = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (partner->visible[i]) {
            Sprite_RenderFrame(&partner->sprites[i]);
        }
    }
    return 1;
}

static s32 TusinSet_partner_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_partner* partner = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&partner->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_partner_Init,
        .update     = TusinSet_partner_Update,
        .render     = TusinSet_partner_Render,
        .cleanup    = TusinSet_partner_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_partner_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_partner_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_partner, NULL, 0, NULL, &args);
}
