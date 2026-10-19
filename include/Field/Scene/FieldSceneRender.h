#ifndef FIELD_SCENE_FIELDSCENERENDER_H
#define FIELD_SCENE_FIELDSCENERENDER_H

#include "Engine/EasyTask.h"
#include <nitro/fx.h>

// Engine-side render state; bit 4 of unk_FC is cleared before each ortho reload.
typedef struct {
    /* 0x00 */ char unk_00[0xFC];
    /* 0xFC */ s32  unk_FC;
} Data02076A44; // Size: 0x100
extern Data02076A44 data_02076a44;
extern Mtx44        data_02076a4c;

struct Ov030B5C58Root;

s16         func_ov030_020b5cd8(Task* task);
s32         func_ov030_020b5ce4(Task* task);
void        func_ov030_020b5d08(Task* task, s32 arg1);
void        func_ov030_020b5d14(Task* task, s32 arg1, s16 arg2, s32 arg3);
s32         func_ov030_020b5d4c(Task* task);
s32         func_ov030_020b5d58(Task* task);
const char* func_ov030_020b5fe8(void);
s32         func_ov030_020b600c(void);
void        func_ov030_020b69b4(struct Ov030B5C58Root* arg0);

#endif // FIELD_SCENE_FIELDSCENERENDER_H
