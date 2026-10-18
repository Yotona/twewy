#include "Field/TaskHelpers.h"
// USA TU; original assembly remains linked (incomplete).
// Range: 0x020c268c-0x020c2ccc; 13 functions.
// Research draft: docs/decomp/ov030/Fld_Wall.c
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

s32  func_ov030_020aeca4(s32 arg0);
s32  Field_GetPlayerHeight(void);
void func_ov030_020b7ac8(void);
s16  FieldNoise_GetTotalQuota(void);
void func_ov030_020aec1c(s32 arg0);
void func_ov030_020aec38(s32 arg0);

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

extern TaskHandle      data_ov030_020e7094;
extern TaskStages      data_ov030_020e70a0;
extern s32             data_ov030_020e70b0[];
extern SpriteAnimation data_ov030_020e70d0;

void func_ov030_020c268c(s32 arg0) {
    s32 i;

    for (i = 0; i < 0x4B; i++) {
        gSaveData.exitStates[i] = arg0;
    }
}

void func_ov030_020c26bc(s32 arg0) {
    s32 i;

    for (i = 0; i < 0x38; i++) {
        gSaveData.exitStates[i] = arg0;
    }
}

void func_ov030_020c26ec(void) {
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
    func_ov030_020c268c(0);
    gSaveData.flashExitMode = 0;
    gSaveData.flashSequence = -1;
    gSaveData.unk_3120      = 0;
}

s32 func_ov030_020c27f4(u32 arg0) {
    u32 index = arg0;

    if (index >= 8U) {
        index = 0;
    }
    return data_ov030_020e70b0[index];
}

void func_ov030_020c280c(Ov030WallSprite* arg0) {
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

SpriteFrameInfo* func_ov030_020c297c(Sprite* sprite, s32 arg, s32 mode) {
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

void func_ov030_020c2a2c(Ov030WallSprite* arg0, Sprite* arg1, Ov030WallConfig* arg2) {
    SpriteAnimation anim = data_ov030_020e70d0;

    anim.dataType  = arg2->dataType;
    anim.posX      = (arg0->x - arg0->origin->x) >> 0xC;
    anim.posY      = (arg0->y - arg0->origin->y) >> 0xC;
    anim.animIndex = func_ov030_020c27f4(arg0->dir) + 1;
    anim.owner     = arg0;
    _Sprite_Load(arg1, &anim);
}

s32 func_ov030_020c2adc(TaskPool* pool, Task* task, Ov030WallConfig* arg2) {
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
    func_ov030_020c280c(sprite);
    sprite->sortKey = 0x1000;
    switch (func_ov030_020c27f4(sprite->dir)) {
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
    func_ov030_020c2a2c(sprite, &sprite->sprite, arg2);
    SndMgr_PlaySEWithPan(0xC8, sprite->sprite.posX);
    return 1;
}

s32 func_ov030_020c2bcc(TaskPool* pool, Task* task) {
    Sprite* sprite = &((Ov030WallSprite*)task->data)->sprite;

    Sprite_Update(sprite);
    if (Sprite_HasAnimation(sprite) != 0) {
        return 1;
    }
    return 0;
}

s32 func_ov030_020c2bf4(TaskPool* pool, Task* task) {
    Ov030WallSprite* w = task->data;

    w->sprite.posX = (w->x - w->origin->x) >> 0xC;
    w->sprite.posY = (w->y - w->origin->y) >> 0xC;
    Sprite_RenderFrame(&w->sprite);
    return 1;
}

s32 func_ov030_020c2c38(TaskPool* pool, Task* task) {
    Sprite_Release(&((Ov030WallSprite*)task->data)->sprite);
    return 1;
}

s32 func_ov030_020c2c4c(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    TaskStages stages = data_ov030_020e70a0;

    return stages.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020c2c94(TaskPool* pool, s32 dataType, FieldPos* origin, s32 dir) {
    Ov030WallConfig params;

    params.dataType = dataType;
    params.origin   = origin;
    params.dir      = dir;
    return EasyTask_CreateTask(pool, &data_ov030_020e7094, NULL, 0, NULL, &params);
}
