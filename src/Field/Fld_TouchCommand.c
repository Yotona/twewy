// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020bfb80-0x020bffac; 11 functions.
// Research draft: docs/decomp/ov030/Fld_TouchCommand.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "Field/TaskHelpers.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 pad_06;
    /* 0x08 */ s32 unk8;
} Ov030TouchCommandConfig;

typedef struct {
    Sprite         sprite; // 0x00
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u16 unk42;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u16 unk4C;
    /* 0x4E */ u16 pad_4E;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
} Ov030TouchCommandSprite;

typedef TaskStages Ov030TouchCommandFns;

extern s32                  func_ov030_020c5374(TaskPool*, s32, s32);
extern s32                  func_ov030_020c53ac(TaskPool*, s32, s32);
extern void                 func_ov030_020c53e8(Task*, s16, s32);
extern SpriteAnimation      data_ov030_020e6e58;
extern Ov030TouchCommandFns data_ov030_020e6e48;
extern TaskHandle           data_ov030_020e6e3c;

SpriteFrameInfo* func_ov030_020bfb80(Sprite* arg0, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(arg0, arg2);
}

void func_ov030_020bfc1c(Sprite* arg0, Ov030TouchCommandConfig* arg1) {
    SpriteAnimation anim = data_ov030_020e6e58;
    u16             dataType;

    dataType       = (u16)arg1->unk0;
    anim.dataType  = dataType;
    anim.animIndex = 1;
    if (arg1->unk8 == 0) {
        anim.bits_0_1 = 1;
    }
    _Sprite_Load(arg0, &anim);
}

s32 func_ov030_020bfca8(TaskPool* arg0, Task* arg1, Ov030TouchCommandConfig* arg2) {
    Ov030TouchCommandSprite* sprite = arg1->data;

    func_ov030_020bfc1c(&sprite->sprite, arg2);
    sprite->unk40 = arg2->unk4;
    sprite->unk42 = arg2->unk4;
    sprite->unk48 = 0;
    sprite->unk44 = 0;
    sprite->unk4C = 0;
    sprite->unk54 = arg2->unk0;
    sprite->unk50 = arg2->unk8;
    if (sprite->unk50 != 0) {
        sprite->unk58 = func_ov030_020c53ac(arg0, sprite->unk54, sprite->unk40);
    } else {
        sprite->unk58 = func_ov030_020c5374(arg0, sprite->unk54, sprite->unk40);
    }
    func_ov030_020c53e8(EasyTask_GetTaskById(arg0, sprite->unk58), (s16)((sprite->unk44 >> 0xC) + 0x10), 8);
    return 1;
}

s32 func_ov030_020bfd4c(TaskPool* arg0, Task* arg1) {
    Ov030TouchCommandSprite* sprite = arg1->data;

    if (sprite->unk4C != 0) {
        func_02026590(&sprite->unk44, sprite->unk48, sprite->unk4C);
        sprite->unk4C = sprite->unk4C - 1;
        if (sprite->unk4C == 0 && sprite->unk48 != 0) {
            EasyTask_DeleteTask(arg0, sprite->unk58);
            sprite->unk40 = sprite->unk42;
            if (sprite->unk50 != 0) {
                sprite->unk58 = func_ov030_020c53ac(arg0, sprite->unk54, sprite->unk40);
            } else {
                sprite->unk58 = func_ov030_020c5374(arg0, sprite->unk54, sprite->unk40);
            }
            sprite->unk48 = 0;
            sprite->unk4C = 0x14;
        }
        sprite->sprite.posX = (s16)(sprite->unk44 >> 0xC);
        func_ov030_020c53e8(EasyTask_GetTaskById(arg0, sprite->unk58), (s16)((sprite->unk44 >> 0xC) + 0x10), 8);
    }
    Sprite_Update(&sprite->sprite);
    return 1;
}

s32 func_ov030_020bfe28(TaskPool* arg0, Task* arg1) {
    Sprite_RenderFrame(arg1->data);
    return 1;
}

s32 func_ov030_020bfe3c(TaskPool* arg0, Task* arg1) {
    Ov030TouchCommandSprite* sprite = arg1->data;

    Sprite_Release(&sprite->sprite);
    EasyTask_DeleteTask(arg0, sprite->unk58);
    return 1;
}

s32 func_ov030_020bfe64(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    Ov030TouchCommandFns funcs = data_ov030_020e6e48;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

u16 func_ov030_020bfeac(s32 arg0) {
    if (arg0 == 1) {
        return 0x5DD2;
    }
    if (arg0 == 4) {
        return 0x5DD5;
    }
    if (arg0 == 3) {
        return 0x5DD4;
    }
    return 0x5DD3;
}

s32 func_ov030_020bfee4(TaskPool* arg0, s32 arg1, s32 arg2) {
    Ov030TouchCommandConfig params;

    params.unk0 = arg1;
    params.unk8 = 1;
    params.unk4 = func_ov030_020bfeac(arg2);
    return EasyTask_CreateTask(arg0, &data_ov030_020e6e3c, NULL, 0, NULL, &params);
}

s32 func_ov030_020bff30(TaskPool* arg0, s32 arg1, s16 arg2, s32 arg3) {
    Ov030TouchCommandConfig params;

    params.unk0 = arg1;
    params.unk4 = arg2;
    params.unk8 = arg3;
    return EasyTask_CreateTask(arg0, &data_ov030_020e6e3c, NULL, 0, NULL, &params);
}

void func_ov030_020bff68(Task* arg0, s32 arg1) {
    Ov030TouchCommandSprite* sprite = arg0->data;
    u16                      val;

    val = func_ov030_020bfeac(arg1);
    if (sprite->unk42 == val) {
        return;
    }
    sprite->unk42 = val;
    if (sprite->unk48 != 0) {
        return;
    }
    sprite->unk48 = -0x50000;
    sprite->unk4C = 0x14;
}
