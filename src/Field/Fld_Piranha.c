#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ u8     unk_40[0x24];
} Fld_Piranha; // Size: 0x64

extern s32 func_ov030_020c5bd8(TaskPool* pool, Task* task, void* args);
extern s32 func_ov030_020c5c8c(TaskPool* pool, Task* task, void* args);
extern s32 func_ov030_020c5e1c(TaskPool* pool, Task* task, void* args);

s32              Fld_Piranha_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
SpriteFrameInfo* Fld_Piranha_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

char                  data_ov030_02108474[28] = "Apl_Kit/GRP_FldPiranha.bin";
const BinIdentifier   data_ov030_020ea438     = {30, data_ov030_02108474};
const TaskHandle      Tsk_Fld_Piranha         = {"Tsk_Fld_Piranha", Fld_Piranha_RunTask, sizeof(Fld_Piranha)};
const SpriteAnimation Fld_Piranha_Anim        = {
           .bits_0_1          = 2,
           .dataType          = 0,
           .bit_6             = 0,
           .bits_7_9          = 5,
           .bits_10_11        = 0,
           .bits_12_13        = 1,
           .bits_14_15        = 0,
           .unk_02.raw        = 0x0,
           .posX              = 0x0,
           .posY              = 0x0,
           .frameInfoCallback = Fld_Piranha_GetFrameInfo,
           .callbackArg       = 0,
           .owner             = NULL,
           .binIden           = &data_ov030_020ea438,
           .unk_18            = 0,
           .packIndex         = 0,
           .unk_1C            = 1,
           .unk_1E            = 0,
           .unk_20            = 4,
           .unk_22            = 1,
           .unk_24            = 0,
           .unk_26            = 2,
           .unk_28            = 3,
           .animIndex         = 1,
};

s32 func_ov030_020c59b0(s32 angle) {
    if (angle < 0x1000) {
        return 1;
    }
    if (angle < 0x4000) {
        return 7;
    }
    if (angle < 0x7000) {
        return 8;
    }
    if (angle < 0x9000) {
        return 2;
    }
    if (angle < 0xC000) {
        return 5;
    }
    if (angle < 0xF000) {
        return 4;
    }
    return 1;
}

SpriteFrameInfo* Fld_Piranha_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = -0x8C000);
}

s32 func_ov030_020c5c6c(Sprite* sprite) {
    s32 animIndex = sprite->animIndex;

    if (animIndex <= 3) {
        return 3;
    }
    if (animIndex <= 6) {
        return 6;
    }
    return 9;
}

s32 Fld_Piranha_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return TRUE;
}

s32 Fld_Piranha_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = func_ov030_020c5bd8,
        .update     = func_ov030_020c5c8c,
        .render     = func_ov030_020c5e1c,
        .cleanup    = Fld_Piranha_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
