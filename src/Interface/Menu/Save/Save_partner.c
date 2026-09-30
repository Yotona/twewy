#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Save.h"
#include "SndMgr.h"
#include "SndMgrSeIdx.h"

typedef struct {
    /* 0x000 */ Sprite          sprites[5];
    /* 0x140 */ BOOL            visible[5];
    /* 0x154 */ SaveMenuObject* save;
} Save_partner; // Size: 0x158

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_partner_Args;

static SpriteFrameInfo* Save_partner_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_partner = {"Tsk_Save_partner", Save_partner_RunTask, sizeof(Save_partner)};

static const SpriteAnimation Save_partner_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Save_partner_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Save_BinIdentifiers[3],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 13,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* Save_partner_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Save_partner_Load(Save_partner* partner, Sprite* sprites, Save_partner_Args* args) {
    SaveMenuObject* save = partner->save;
    SpriteAnimation anim = Save_partner_Anim;

    anim.dataType = args->dataType;
    for (s16 i = 0; i < 3; i++) {
        anim.animIndex = i + 4;
        anim.posX      = i * 18 + 133;
        anim.posY      = 12;
        _Sprite_Load(&sprites[i], &anim);
    }

    anim.animIndex = 3;
    anim.posX      = save->partner * 18 + 133;
    anim.posY      = 12;
    _Sprite_Load(&sprites[3], &anim);

    anim.animIndex = 1;
    anim.posX      = 81;
    anim.posY      = 12;
    _Sprite_Load(&sprites[4], &anim);
}

static s32 Save_partner_Init(TaskPool* pool, Task* task, void* args) {
    Save_partner*      partner  = task->data;
    Save_partner_Args* initArgs = args;

    partner->save = initArgs->save;
    for (u16 i = 0; i < 5; i++) {
        partner->visible[i] = TRUE;
    }
    Save_partner_Load(partner, partner->sprites, initArgs);
    return 1;
}

static s32 Save_partner_Update(TaskPool* pool, Task* task, void* args) {
    Save_partner*   partner = task->data;
    SaveMenuObject* save    = partner->save;
    TouchCoord      touch;

    if (save->helpOpen != 0) {
        partner->visible[3] = FALSE;
        return 1;
    }

    partner->visible[3] = TRUE;
    if (save->flags & 0x100) {
#ifdef REGION_USA
        for (s32 i = 0; i < 5; i++) {
            Sprite_Update(&partner->sprites[i]);
        }
#endif
        return 1;
    }

    if (TouchInput_WasTouchPressed()) {
        s16 index;

        TouchInput_GetCoord(&touch);
        index = Save_GetPartnerAtPoint(touch.x, touch.y);
        if (index != -1 && save->partner != index) {
            s32 screenIndex;

            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            partner->sprites[3].posX = index * 18 + 133;
            save->partner            = index;
            Save_ChangePartner(save, index);
            if (save->partner == 0) {
                screenIndex = 0;
            } else if (save->partner == 1) {
                screenIndex = 4;
            } else {
                screenIndex = 5;
            }
            Save_ReleaseBgResource(&save->resources[3], DISPLAY_SUB);
            Save_LoadBgResourceIndexed(&save->resources[3], DISPLAY_SUB, 3, 5, 1, 13, screenIndex, save->partner);
        }
    }

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&partner->sprites[i]);
    }
    return 1;
}

static s32 Save_partner_Render(TaskPool* pool, Task* task, void* args) {
    Save_partner* partner = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (partner->visible[i]) {
            Sprite_RenderFrame(&partner->sprites[i]);
        }
    }
    return 1;
}

static s32 Save_partner_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_partner* partner = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&partner->sprites[i]);
    }
    return 1;
}

static s32 Save_partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_partner_Init,
        .update     = Save_partner_Update,
        .render     = Save_partner_Render,
        .cleanup    = Save_partner_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_partner_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_partner_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_partner, NULL, 0, NULL, &args);
}
