// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020cd578-0x020cd790; 9 functions.
// Research draft: docs/decomp/ov030/Fld_ShinigamiON.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite; // 0x40
    /* 0x40 */ s16    unk40;  // current animation index
    /* 0x42 */ s16    pad42;
} Ov030ONSprite;              // 0x44

typedef TaskStages Ov030ONFns;

extern Ov030ONFns      data_ov030_020ed15c;
extern TaskHandle      data_ov030_020ed150;
extern SpriteAnimation data_ov030_020ed16c;

SpriteFrameInfo* func_ov030_020cd578(Sprite* arg0, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(arg0, arg2);
}

// Nonmatching: SpriteAnimation build; retail materialises the dataType value
// operand before the posY/posX stores (loading *arg1 first, then bic/orr
// against the anim word), ours sinks the load to just before its own
// read-modify-write (USA 71.97%; JP unvalidated).
void func_ov030_020cd614(Sprite* arg0, s32* arg1) {
    SpriteAnimation anim = data_ov030_020ed16c;

    anim.posY     = 0x60;
    anim.posX     = 0x80;
    anim.dataType = *arg1;
    SndMgr_PlaySEWithPan(0xC7, 0x80);
    _Sprite_Load(arg0, &anim);
}

void func_ov030_020cd694(TaskPool* arg0, Task* arg1, s32* arg2) {
    Ov030ONSprite* sprite = arg1->data;

    sprite->unk40 = 4;
    func_ov030_020cd614(&sprite->sprite, arg2);
}

s32 func_ov030_020cd6b0(TaskPool* arg0, Task* arg1, void* arg2) {
    Sprite_Update(arg1->data);
    return 1;
}

s32 func_ov030_020cd6c4(TaskPool* arg0, Task* arg1, void* arg2) {
    Sprite_RenderFrame(arg1->data);
    return 1;
}

s32 func_ov030_020cd6d8(TaskPool* arg0, Task* arg1, void* arg2) {
    Sprite_Release(arg1->data);
    return 1;
}

s32 func_ov030_020cd6ec(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    Ov030ONFns funcs = data_ov030_020ed15c;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020cd734(TaskPool* arg0, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(arg0, &data_ov030_020ed150, NULL, 0, NULL, &param);
}

s32 func_ov030_020cd764(Task* arg0, s16 arg1) {
    Ov030ONSprite* sprite = arg0->data;

    Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, arg1, sprite->sprite.cellTable);
    sprite->unk40 = arg1;
    return 1;
}
