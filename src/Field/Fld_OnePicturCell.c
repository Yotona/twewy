// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020bd364-0x020bd5cc; 8 functions.
// Research draft: docs/decomp/ov030/Fld_OnePicturCell.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

// The task data is a Sprite at +0x00 extended with a selector at +0x40.
typedef struct {
    /* 0x00 */ Sprite sprite; // 0x40
    /* 0x40 */ s32    unk40;
} OnePicturSprite;

typedef struct {
    /* 0x00 */ s32 dataType;
    /* 0x04 */ s32 picture; // OnePicturEntry index
} OnePicturArgs;

// 0xC-stride animation parameter records.
typedef struct {
    /* 0x00 */ s16 binIndex;
    /* 0x02 */ s16 packIndex;
    /* 0x04 */ s16 paletteIndex;
    /* 0x06 */ s16 charIndex;
    /* 0x08 */ s16 cellIndex;
    /* 0x0A */ s16 animDataIndex;
} OnePicturEntry; // Size: 0xC

typedef TaskStages OnePicturFns;

extern SpriteAnimation     data_ov030_020e6208;
extern OnePicturEntry      data_ov030_020e61f0[];
extern const BinIdentifier data_ov030_020d98bc[];
extern OnePicturFns        data_ov030_020e61e0;
extern TaskHandle          data_ov030_020e61d4;

SpriteFrameInfo* func_ov030_020bd364(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode)
}

// Nonmatching: mwcc rotates the six live register roles (ours: binIden=r5,
// anim bits=r12, packIndex=r4, dataType<<0x1c=r3; retail: r12/r5/r3/r4), so our
// packIndex load and its store get sunk after the dataType bic/orr merge
// instead of sitting between the BinIdentifier address add and that merge
// (USA 87.57%; JP unvalidated).
void func_ov030_020bd400(Sprite* sprite, OnePicturArgs* args) {
    SpriteAnimation anim      = data_ov030_020e6208;
    s32             idx       = args->picture;
    OnePicturEntry* entry     = &data_ov030_020e61f0[idx];
    s16             packIndex = entry->packIndex;
    s16             binIden   = entry->binIndex;
    u16             dataType  = (u16)args->dataType;

    anim.packIndex = packIndex;
    anim.binIden   = &data_ov030_020d98bc[binIden];
    anim.dataType  = dataType;
    anim.unk_1C    = entry->charIndex;
    anim.unk_20    = entry->paletteIndex;
    anim.unk_26    = entry->cellIndex;
    anim.unk_28    = entry->animDataIndex;
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020bd4bc(TaskPool* pool, Task* task, OnePicturArgs* args) {
    OnePicturSprite* sprite = task->data;

    sprite->unk40 = args->picture;
    func_ov030_020bd400(&sprite->sprite, args);
    return 1;
}

s32 func_ov030_020bd4dc(TaskPool* pool, Task* task) {
    Sprite* sprite   = task->data;
    s32     finished = 0;

    if (SpriteMgr_IsFrameFinished(sprite) != 0) {
        finished = 1;
    }
    Sprite_UpdateAndCheck(sprite);
    if (finished != 0 && SpriteMgr_IsAnimationFinished(sprite) == 0) {
        SndMgr_StartPlayingSE((SndMgrSeIdx)0xD3);
    }
    return 1;
}

s32 func_ov030_020bd528(TaskPool* pool, Task* task) {
    Sprite_Render(task->data);
    return 1;
}

s32 func_ov030_020bd53c(TaskPool* pool, Task* task) {
    Sprite_Destroy(task->data);
    return 1;
}

s32 func_ov030_020bd550(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    OnePicturFns funcs = data_ov030_020e61e0;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020bd598(TaskPool* pool, s32 dataType, s32 picture) {
    OnePicturArgs params;

    params.dataType = dataType;
    params.picture  = picture;
    return EasyTask_CreateTask(pool, &data_ov030_020e61d4, NULL, 0, NULL, &params);
}
