#include "Field/TaskHelpers.h"
// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020bffac-0x020c0268; 10 functions.
// Research draft: docs/decomp/ov030/Fld_TouchSeal.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
} Ov030TouchSealEntry;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
} Ov030TouchSealConfig;

typedef struct {
    Sprite         sprite; // 0x00
    /* 0x40 */ s32 unk40;
} Ov030TouchSealSprite;

typedef TaskStages Ov030TouchSealFns;

extern Ov030TouchSealEntry data_ov030_020e6ea0[];
extern SpriteAnimation     data_ov030_020e6ec0;
extern Ov030TouchSealFns   data_ov030_020e6e90;
extern TaskHandle          data_ov030_020e6e84;

SpriteFrameInfo* func_ov030_020bffac(Sprite* arg0, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(arg0, arg2);
}

// Nonmatching: register allocation + load scheduling in the SpriteAnimation build
// (retail loads the config dataType before the table entry and keeps &anim in r1
// while using r2 for the index and lr for the base; ours hoists the anim-word load
// of the dataType read-modify-write and sinks the dataType load to the insert)
// (USA 70.61%; JP unvalidated).
void func_ov030_020c0048(Sprite* arg0, Ov030TouchSealConfig* arg1) {
    SpriteAnimation anim = data_ov030_020e6ec0;

    anim.posX      = arg1->unk8;
    anim.packIndex = data_ov030_020e6ea0[arg1->unk4].unk0;
    anim.dataType  = arg1->unk0;
    anim.posY      = arg1->unkA;
    _Sprite_Load(arg0, &anim);
}

s32 func_ov030_020c00e0(TaskPool* arg0, Task* arg1, Ov030TouchSealConfig* arg2) {
    Ov030TouchSealSprite* sprite = arg1->data;

    sprite->unk40 = arg2->unk4;
    func_ov030_020c0048(&sprite->sprite, arg2);
    return 1;
}

s32 func_ov030_020c0100(TaskPool* arg0, Task* arg1) {
    Sprite_Update(arg1->data);
    return 1;
}

s32 func_ov030_020c0114(TaskPool* arg0, Task* arg1) {
    Sprite_RenderFrame(arg1->data);
    return 1;
}

s32 func_ov030_020c0128(TaskPool* arg0, Task* arg1) {
    Sprite_Release(arg1->data);
    return 1;
}

s32 func_ov030_020c013c(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    Ov030TouchSealFns funcs = data_ov030_020e6e90;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020c0184(TaskPool* arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    Ov030TouchSealConfig params;

    params.unk0 = arg1;
    params.unk4 = arg2;
    params.unk8 = arg3;
    params.unkA = arg4;
    return EasyTask_CreateTask(arg0, &data_ov030_020e6e84, NULL, 0, NULL, &params);
}

void func_ov030_020c01c4(Task* arg0, s16 arg1) {
    Ov030TouchSealSprite* sprite = arg0->data;

    Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, arg1, sprite->sprite.cellTable);
    Sprite_Update(&sprite->sprite);
}

s32 func_ov030_020c01ec(Task* arg0) {
    Ov030TouchSealSprite* sprite = arg0->data;
    Ov030TouchSealEntry*  entry  = &data_ov030_020e6ea0[sprite->unk40];
    s16                   x;
    s16                   y;
    TouchCoord            coord;

    x = sprite->sprite.posX;
    y = sprite->sprite.posY;
    TouchInput_GetCoord(&coord);
    if (x - entry->unk4 < coord.x && coord.x < x + entry->unk4) {
        if (y - entry->unk2 < coord.y && coord.y < y + entry->unk2) {
            return 1;
        }
    }
    return 0;
}
