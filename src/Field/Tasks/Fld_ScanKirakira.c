#include "Field/Tasks/Fld_ScanKirakira.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/Event/EventTables.h"
#include "Field/Tasks/Fld_ScanFukidasi.h"
#include "Save.h"
#include "SpriteMgr.h"
#include <nitro/math.h>

// The scan-kirakira task data: a Sprite plus a countdown and the per-animation
// parameters handed to Fld_ScanFukidasi_AddSpot.
typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s16    unk40;
    /* 0x42 */ s16    unk42;
    /* 0x44 */ s32    unk44;
    /* 0x48 */ s32    unk48;
    /* 0x4C */ s32    unk4C;
    /* 0x50 */ s32    unk50;
    /* 0x54 */ Task*  human;
    /* 0x58 */ s32    unk58;
} ScanKirakiraSprite;

// Spawn parameters (also passed through to Fld_ScanKirakira_Load).
typedef struct {
    /* 0x00 */ u16   unk0;
    /* 0x02 */ u16   unk2;
    /* 0x04 */ s32   unk4;
    /* 0x08 */ s32   unk8;
    /* 0x0C */ s32   unkC;
    /* 0x10 */ s32   unk10;
    /* 0x14 */ s32   unk14;
    /* 0x18 */ Task* human;
} ScanKirakiraArgs; // Size: 0x1C

static SpriteFrameInfo* Fld_ScanKirakira_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_ScanKirakira_RunTask(TaskPool* pool, Task* task, void* arg2, s32 arg3);

static const TaskHandle Tsk_Fld_ScanKirakira = {"Tsk_Fld_ScanKirakira", Fld_ScanKirakira_RunTask, sizeof(ScanKirakiraSprite)};

static const SpriteAnimation Fld_ScanKirakira_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0800,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_ScanKirakira_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldScan,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 1,
};

static SpriteFrameInfo* Fld_ScanKirakira_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
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

static void Fld_ScanKirakira_Load(Sprite* sprite, ScanKirakiraArgs* args) {
    SpriteAnimation anim = Fld_ScanKirakira_Anim;
    s16             posX;

    anim.dataType  = (u16)args->unk8;
    posX           = args->unk0;
    anim.posY      = args->unk2;
    anim.posX      = posX;
    anim.animIndex = (s16)args->unk4;
    if (args->unk4 == 5) {
        anim.animIndex = 1;
        anim.unk_20    = 9;
    }
    if (args->unkC >= 0x10000) {
        anim.unk_20 = 5;
    }
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_ScanKirakira_Init(TaskPool* pool, Task* task, void* args) {
    ScanKirakiraArgs*   params = args;
    ScanKirakiraSprite* data   = task->data;

    data->unk44 = params->unk8;
    data->unk48 = params->unkC;
    data->unk4C = params->unk10;
    data->unk50 = params->unk14;
    data->human = params->human;
    Fld_ScanKirakira_Load(&data->sprite, params);
    data->unk40 = 0x14;
    data->unk58 = 0;
    return 1;
}

static s32 Fld_ScanKirakira_IsOnScreen(s32 arg0, s32 arg1) {
    if (gSaveData.fieldArea == 1) {
        return 1;
    }
    if (arg0 > 0x100) {
        return 0;
    }
    if (arg0 < 0) {
        return 0;
    }
    if (arg1 < 0) {
        return 0;
    }
    if (arg1 <= 0xC0) {
        return 1;
    }
    return 0;
}

static s32 Fld_ScanKirakira_Update(TaskPool* pool, Task* task, void* args) {
    ScanKirakiraSprite* data = task->data;
    s32                 temp;

    if (data->unk40 != 0) {
        data->unk40 = (s16)(data->unk40 - 1);
    }
    if (data->unk40 == 0 && data->unk48 != -2 && Fld_ScanKirakira_IsOnScreen(data->sprite.posX, data->sprite.posY) != 0) {
        temp = data->unk58;
        if (temp == 0) {
            data->unk58 = temp + 1;
            Sprite_SetAnimation(&data->sprite, data->sprite.animData, 2, data->sprite.cellTable);
            data->unk40 = 0x12;
        } else {
            Fld_ScanFukidasi_AddSpot(pool, data->unk44, data->sprite.posX, data->sprite.posY, data->unk48, data->unk4C,
                                     data->unk50, data->human);
            return 0;
        }
    }
    if (FieldScan_IsOpen() == 0) {
        return 0;
    }
    if (FieldScan_IsEnteringBattle() != 0) {
        return 0;
    }
    Sprite_Update(&data->sprite);
    return 1;
}

static s32 Fld_ScanKirakira_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_ScanKirakira_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_ScanKirakira_RunTask(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    const TaskStages stages = {
        .initialize = Fld_ScanKirakira_Init,
        .update     = Fld_ScanKirakira_Update,
        .render     = Fld_ScanKirakira_Render,
        .cleanup    = Fld_ScanKirakira_Destroy,
    };

    return stages.iter[arg3](pool, task, arg2);
}

s32 Fld_ScanKirakira_CreateTask(TaskPool* pool, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7,
                                Task* task) {
    ScanKirakiraArgs params;

    params.unk8  = arg1;
    params.unk0  = arg2;
    params.unk2  = arg3;
    params.unk4  = arg4;
    params.unk14 = arg7;
    params.human = task;
    params.unkC  = arg5;
    params.unk10 = arg6;
    return EasyTask_CreateTask(pool, &Tsk_Fld_ScanKirakira, NULL, 0, NULL, &params);
}
