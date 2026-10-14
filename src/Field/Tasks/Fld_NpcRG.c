// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020bee24-0x020bf084; 8 functions.
// Research draft: docs/decomp/ov030/Fld_NpcRG.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

// The task data is a Sprite at +0x00 extended with a sentinel at +0x40.
typedef struct {
    /* 0x00 */ Sprite sprite; // 0x40
    /* 0x40 */ s32    npc;
} NpcRGSprite;

typedef struct {
    /* 0x00 */ s32 dataType;
    /* 0x04 */ s32 npc; // NpcRGEntry index, -1 = none
} NpcRGArgs;

// 0xC-stride animation parameter records.
typedef struct {
    /* 0x00 */ s16 charIndex;
    /* 0x02 */ s16 animDataIndex;
    /* 0x04 */ s16 cellIndex;
    /* 0x06 */ s16 paletteIndex;
    /* 0x08 */ s16 animIndex;
    /* 0x0A */ s16 packIndex;
} NpcRGEntry; // Size: 0xC

typedef TaskStages NpcRGFns;

extern SpriteAnimation data_ov030_020e6398;
extern NpcRGEntry      data_ov030_020e63c4[];
extern NpcRGFns        data_ov030_020e6388;
extern TaskHandle      data_ov030_020e637c;

SpriteFrameInfo* func_ov030_020bee24(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode)
}

void func_ov030_020beec0(Sprite* sprite, NpcRGArgs* args) {
    SpriteAnimation anim = data_ov030_020e6398;
    NpcRGEntry*     entry;
    s32             idx;

    anim.dataType = args->dataType;
    idx           = args->npc;
    if (idx < 0 || (u32)idx >= 0xCFU) {
        idx = 0;
    }
    entry          = &data_ov030_020e63c4[idx];
    anim.unk_1C    = entry->charIndex;
    anim.unk_20    = entry->paletteIndex;
    anim.unk_26    = entry->cellIndex;
    anim.unk_28    = entry->animDataIndex;
    anim.animIndex = entry->animIndex;
    anim.packIndex = entry->packIndex;
    _Sprite_Load(sprite, &anim);
    Sprite_Update(sprite);
}

s32 func_ov030_020bef8c(TaskPool* pool, Task* task, NpcRGArgs* args) {
    NpcRGSprite* sprite = task->data;

    sprite->npc = args->npc;
    if (sprite->npc != -1) {
        func_ov030_020beec0(&sprite->sprite, args);
    }
    return 1;
}

s32 func_ov030_020befb8(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov030_020befc0(TaskPool* pool, Task* task) {
    NpcRGSprite* sprite = task->data;

    if (sprite->npc != -1) {
        Sprite_RenderFrame(&sprite->sprite);
    }
    return 1;
}

s32 func_ov030_020befe4(TaskPool* pool, Task* task) {
    NpcRGSprite* sprite = task->data;

    if (sprite->npc != -1) {
        Sprite_Release(&sprite->sprite);
    }
    return 1;
}

s32 func_ov030_020bf008(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    NpcRGFns funcs = data_ov030_020e6388;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020bf050(TaskPool* pool, s32 dataType, s32 npc) {
    NpcRGArgs params;

    params.dataType = dataType;
    params.npc      = npc;
    return EasyTask_CreateTask(pool, &data_ov030_020e637c, NULL, 0, NULL, &params);
}
