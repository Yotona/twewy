#include "Field/TaskHelpers.h"
// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020c0268-0x020c0534; 9 functions.
// Research draft: docs/decomp/ov030/Fld_TouchCursor.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "SpriteMgr.h"

typedef struct {
    Sprite         sprite; // 0x00
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
} Ov030TouchCursorSprite;

typedef TaskStages Ov030TouchCursorFns;

extern SpriteAnimation     data_ov030_020e6f08;
extern Ov030TouchCursorFns data_ov030_020e6ef8;
extern TaskHandle          data_ov030_020e6eec;

SpriteFrameInfo* func_ov030_020c0268(Sprite* arg0, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(arg0, arg2);
}

void func_ov030_020c0304(Sprite* arg0, FldSpriteAnimationParams* arg1) {
    SpriteAnimation anim = data_ov030_020e6f08;

    anim.dataType  = arg1->dataType;
    anim.animIndex = arg1->animIndex;
    _Sprite_Load(arg0, &anim);
}

s32 func_ov030_020c0378(TaskPool* arg0, Task* arg1, FldSpriteAnimationParams* arg2) {
    Ov030TouchCursorSprite* sprite = arg1->data;

    func_ov030_020c0304(&sprite->sprite, arg2);
    sprite->unk40 = 0;
    sprite->unk44 = 0;
    sprite->unk46 = arg2->animIndex;
    return 1;
}

s32 func_ov030_020c03ac(TaskPool* arg0, Task* arg1) {
    Ov030TouchCursorSprite* sprite = arg1->data;
    TouchCoord              coord;
    s16                     temp;

    if (TouchInput_WasTouchPressed() != 0) {
        Sprite_SetAnimation(&sprite->sprite, sprite->sprite.animData, sprite->unk46, sprite->sprite.cellTable);
        sprite->unk44 = 8;
        if (sprite->unk46 == 1) {
            sprite->unk44 = 0xC;
        }
    }
    if (TouchInput_IsTouchActive() != 0) {
        sprite->unk40 = 1;
        TouchInput_GetCoord(&coord);
        sprite->sprite.posX = coord.x;
        sprite->sprite.posY = coord.y;
    } else if (sprite->unk44 != 0) {
        sprite->unk40 = 1;
    } else {
        sprite->unk40 = 0;
    }
    temp = sprite->unk44;
    if (temp != 0) {
        sprite->unk44 = temp - 1;
    }
    Sprite_Update(&sprite->sprite);
    return 1;
}

s32 func_ov030_020c045c(TaskPool* arg0, Task* arg1) {
    Ov030TouchCursorSprite* sprite = arg1->data;

    if (sprite->unk40 != 0) {
        Sprite_RenderFrame(&sprite->sprite);
    }
    return 1;
}

s32 func_ov030_020c047c(TaskPool* arg0, Task* arg1) {
    Sprite_Release(arg1->data);
    return 1;
}

s32 func_ov030_020c0490(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    Ov030TouchCursorFns funcs = data_ov030_020e6ef8;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020c04d8(TaskPool* arg0, s32 arg1, s16 arg2) {
    FldSpriteAnimationParams params;

    params.dataType  = arg1;
    params.animIndex = arg2;
    return EasyTask_CreateTask(arg0, &data_ov030_020e6eec, NULL, 0, NULL, &params);
}

void func_ov030_020c050c(Task* arg0, s16 arg1) {
    Ov030TouchCursorSprite* sprite = arg0->data;

    Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, arg1, sprite->sprite.cellTable);
    sprite->unk46 = arg1;
}
