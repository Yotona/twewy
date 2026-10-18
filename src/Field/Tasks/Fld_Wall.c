#include "Field/Tasks/Fld_Wall.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldNoiseQuota.h"
#include "Field/Event/EventFlags.h"
#include "Field/Event/EventTables.h"
#include "Field/FieldTypes.h"
#include "Field/Scene/FieldArea.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ u8             pad_40[0x4];
    /* 0x44 */ s32            unk44;
    /* 0x48 */ s32            unk48;
    /* 0x4C */ FieldPos*      origin;
    /* 0x50 */ s32            x; // fx32 map position
    /* 0x54 */ s32            y;
    /* 0x58 */ OamAffineParam affine;
    /* 0x68 */ s32            sortKey;
    /* 0x6C */ s32            dir;
} Ov030WallSprite; // Size: 0x70

typedef struct {
    /* 0x00 */ s32       dataType;
    /* 0x04 */ FieldPos* origin;
    /* 0x08 */ s32       dir;
} Ov030WallConfig;
static SpriteFrameInfo* Fld_Wall_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_Wall_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Wall = {"Tsk_Fld_Wall", Fld_Wall_RunTask, sizeof(Ov030WallSprite)};

static const s32 Fld_Wall_DirFacings[8] = {1, 3, 3, 0, 0, 2, 2, 1};

static const SpriteAnimation Fld_Wall_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 6,
    .bits_10_11        = 1,
    .bits_12_13        = 0,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0000,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_Wall_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
    .unk_18            = 2,
    .packIndex         = 9,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 1,
};

void Fld_Wall_SetAllExitStates(s32 arg0) {
    s32 i;

    for (i = 0; i < 0x4B; i++) {
        gSaveData.exitStates[i] = arg0;
    }
}

void Fld_Wall_SetAllWallStates(s32 arg0) {
    s32 i;

    for (i = 0; i < 0x38; i++) {
        gSaveData.exitStates[i] = arg0;
    }
}

void Field_ResetDayProgress(void) {
    s32 i;

    if (gSaveData.unk_2484 == 0) {
        return;
    }
    gSaveData.unk_2484 = 0;
    gSaveData.unk_2488 = 0;
    func_ov030_020b7ac8();
    i                  = 0;
    gSaveData.unk_26E4 = 0;
    gSaveData.unk_26F0 = 0;
    gSaveData.unk_26EC = 0;
    do {
        gSaveData.progressFlags[i] = 0;
        i++;
    } while (i < 4);
    func_ov030_020aec1c(1);
    func_ov030_020aec38(4);
    func_ov030_020aec38(5);
    func_ov030_020aec38(7);
    i                  = 0;
    gSaveData.unk_24C4 = 0;
    do {
        gSaveData.unk_24C8[i] = -1;
        i++;
    } while (i < 0x10);
    gSaveData.noiseQuotaStep   = 0;
    gSaveData.noiseSymbolCount = FieldNoise_GetTotalQuota();
    gSaveData.unk_2470         = 0;
    Fld_Wall_SetAllExitStates(0);
    gSaveData.flashExitMode = 0;
    gSaveData.flashSequence = -1;
    gSaveData.unk_3120      = 0;
}

static s32 Fld_Wall_GetFacing(u32 arg0) {
    u32 index = arg0;

    if (index >= 8U) {
        index = 0;
    }
    return Fld_Wall_DirFacings[index];
}

static void Fld_Wall_PlaceAhead(Ov030WallSprite* arg0) {
    s32 bound;
    s32 limit;
    s32 dir;
    s32 move;

    func_ov030_020b7b00();
    func_ov030_020b7b80();
    func_ov030_020b7b00();
    func_ov030_020b7b80();
    bound = ((func_ov030_020b7b40() + func_ov030_020b7b60()) - 1) << 0xC;
    limit = (func_ov030_020b7bf8() + 1) << 0xC;
    move  = func_ov030_020aeca4(gSaveData.fieldPos.y) * 0x10;
    dir   = arg0->dir;
    if (dir == 1) {
        arg0->y += move / 2;
        arg0->x += move / 2;
    } else if (dir == 0) {
        arg0->y += move;
    } else if (dir == 7) {
        arg0->y += move / 2;
        arg0->x -= move / 2;
    } else if (dir == 6) {
        arg0->x -= move;
    } else if (dir == 5) {
        arg0->y -= move / 2;
        arg0->x -= move / 2;
    } else if (dir == 4) {
        arg0->y -= move;
    } else if (dir == 3) {
        arg0->y -= move / 2;
        arg0->x += move / 2;
    } else if (dir == 2) {
        arg0->x += move;
    }
    if (arg0->y < limit + 0x18000) {
        arg0->y = limit + 0x18000;
        return;
    }
    if (bound - 0x18000 < arg0->y) {
        arg0->y = bound - 0x18000;
    }
}

static SpriteFrameInfo* Fld_Wall_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Ov030WallSprite* owner = sprite->owner;
    SpriteFrameInfo* info  = NULL;

    switch (mode) {
        case SPRITE_FRAME_UPDATE:
            g_SpriteFrameInfo.updateSteps = 1;
            return &g_SpriteFrameInfo;
        case SPRITE_FRAME_RENDER: {
            SpriteFrameInfo* temp = &g_SpriteFrameInfo;

            Sprite_FillFrameInfo(temp, sprite);
            temp->affine  = &owner->affine;
            temp->sortKey = owner->sortKey;
            info          = temp;
        } break;
    }
    return info;
}

static void Fld_Wall_Load(Ov030WallSprite* arg0, Sprite* sprite, Ov030WallConfig* arg2) {
    SpriteAnimation anim = Fld_Wall_Anim;

    anim.dataType  = arg2->dataType;
    anim.posX      = (arg0->x - arg0->origin->x) >> 0xC;
    anim.posY      = (arg0->y - arg0->origin->y) >> 0xC;
    anim.animIndex = Fld_Wall_GetFacing(arg0->dir) + 1;
    anim.owner     = arg0;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_Wall_Init(TaskPool* pool, Task* task, void* args) {
    Ov030WallConfig* arg2   = args;
    Ov030WallSprite* sprite = task->data;
    s32              tmp;

    sprite->unk44           = func_ov030_020aeca4(gSaveData.fieldPos.y);
    sprite->dir             = arg2->dir;
    sprite->affine.rotation = 0;
    sprite->affine.scaleX   = sprite->unk44;
    sprite->affine.scaleY   = sprite->unk44;
    sprite->affine.unk_0C   = 0;
    sprite->affine.unk_0E   = 0;
    sprite->unk48           = 0;
    sprite->x               = gSaveData.fieldPos.x;
    tmp                     = Field_GetPlayerHeight();
    sprite->y               = gSaveData.fieldPos.y - tmp / 2;
    sprite->origin          = arg2->origin;
    Fld_Wall_PlaceAhead(sprite);
    sprite->sortKey = 0x1000;
    switch (Fld_Wall_GetFacing(sprite->dir)) {
        case 3:
            sprite->unk48 = 1;
            break;
        case 1:
            sprite->unk48   = 2;
            sprite->sortKey = 0x3E8000;
            break;
        case 0:
            sprite->sortKey = 0x1000;
            break;
    }
    Fld_Wall_Load(sprite, &sprite->sprite, arg2);
    SndMgr_PlaySEWithPan(0xC8, sprite->sprite.posX);
    return 1;
}

static s32 Fld_Wall_Update(TaskPool* pool, Task* task, void* args) {
    Sprite* sprite = &((Ov030WallSprite*)task->data)->sprite;

    Sprite_Update(sprite);
    if (Sprite_HasAnimation(sprite) != 0) {
        return 1;
    }
    return 0;
}

static s32 Fld_Wall_Render(TaskPool* pool, Task* task, void* args) {
    Ov030WallSprite* w = task->data;

    w->sprite.posX = (w->x - w->origin->x) >> 0xC;
    w->sprite.posY = (w->y - w->origin->y) >> 0xC;
    Sprite_RenderFrame(&w->sprite);
    return 1;
}

static s32 Fld_Wall_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(&((Ov030WallSprite*)task->data)->sprite);
    return 1;
}

static s32 Fld_Wall_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Wall_Init,
        .update     = Fld_Wall_Update,
        .render     = Fld_Wall_Render,
        .cleanup    = Fld_Wall_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Wall_CreateTask(TaskPool* pool, s32 dataType, FieldPos* origin, s32 dir) {
    Ov030WallConfig params;

    params.dataType = dataType;
    params.origin   = origin;
    params.dir      = dir;
    return EasyTask_CreateTask(pool, &Tsk_Fld_Wall, NULL, 0, NULL, &params);
}
