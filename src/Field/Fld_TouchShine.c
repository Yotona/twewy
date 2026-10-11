// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020c0534-0x020c0780; 10 functions.
// Research draft: docs/decomp/ov030/Fld_TouchShine.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ s32 dataType;
} Ov030TouchShineConfig;

typedef TaskStages Ov030TouchShineFns;

extern SpriteAnimation    data_ov030_020e6f50;
extern Ov030TouchShineFns data_ov030_020e6f40;
extern TaskHandle         data_ov030_020e6f34;
extern s16                data_ov030_02109d74;

SpriteFrameInfo* func_ov030_020c0534(Sprite* arg0, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(arg0, arg2);
}

void func_ov030_020c05d0(Sprite* arg0, Ov030TouchShineConfig* arg1) {
    SpriteAnimation anim = data_ov030_020e6f50;
    TouchCoord      coord;

    anim.dataType = arg1->dataType;
    TouchInput_GetCoord(&coord);
    anim.posX = coord.x;
    anim.posY = coord.y;
    SndMgr_PlaySEWithPan(0xC7, anim.posX);
    _Sprite_Load(arg0, &anim);
}

void func_ov030_020c065c(TaskPool* arg0, Task* arg1, Ov030TouchShineConfig* arg2) {
    Sprite* sprite = arg1->data;

    data_ov030_02109d74 += 1;
    func_ov030_020c05d0(sprite, arg2);
}

s32 func_ov030_020c0684(TaskPool* arg0, Task* arg1) {
    Sprite* sprite = arg1->data;

    Sprite_Update(sprite);
    return Sprite_HasAnimation(sprite) != 0;
}

s32 func_ov030_020c06ac(TaskPool* arg0, Task* arg1) {
    Sprite_RenderFrame(arg1->data);
    return 1;
}

s32 func_ov030_020c06c0(TaskPool* arg0, Task* arg1) {
    Sprite_Release(arg1->data);
    data_ov030_02109d74 -= 1;
    return 1;
}

s32 func_ov030_020c06e8(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    Ov030TouchShineFns funcs = data_ov030_020e6f40;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020c0730(TaskPool* arg0, s32 arg1) {
    s32 param;

    param = arg1;
    return EasyTask_CreateTask(arg0, &data_ov030_020e6f34, NULL, 0, NULL, &param);
}

void func_ov030_020c0760(s16 arg0) {
    data_ov030_02109d74 = arg0;
}

s16 func_ov030_020c0770(void) {
    return data_ov030_02109d74;
}
