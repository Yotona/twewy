#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Field/FieldTasks.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include "types.h"

extern BinIdentifier data_ov030_020d9944;

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ s32           unk_40;
    /* 0x44 */ s32           unk_44;
    /* 0x48 */ u16           unk_48;
    /* 0x4A */ u8            unk_4A;
    /* 0x4C */ UnkOv31Struct unk_4C;
    /* 0xC8 */ void*         unk_C8;
} Fld_GetMess; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ u16 unk_4;
    /* 0x6 */ u16 unk_6;
} Fld_GetMess_Args;

SpriteFrameInfo* Fld_GetMess_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
s32              Fld_GetMess_RunTask(TaskPool*, Task*, void*, s32);

static const TaskHandle Tsk_Fld_GetMess = {"Tsk_Fld_GetMess", Fld_GetMess_RunTask, sizeof(Fld_GetMess)};

static const SpriteAnimation data_ov030_020ec8f0 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 1,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0xFF80,
    .unk_06     = 0xA0,
    .unk_08     = Fld_GetMess_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov030_020d9944,
    .unk_18     = 2,
    .packIndex  = 11,
    .unk_1C     = 2,
    .unk_1E     = 0,
    .unk_20     = 1,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 3,
    .unk_28     = 4,
    .unk_2A     = 3,
};

SpriteFrameInfo* Fld_GetMess_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            info = temp;
        } break;
    }

    return info;
}

void Fld_GetMess_Load(Sprite* sprite, Fld_GetMess_Args* args) {
    SpriteAnimation anim = data_ov030_020ec8f0;

    anim.dataType = args->unk_0;
    _Sprite_Load(sprite, &anim);
}

s32 Fld_GetMess_Init(TaskPool* pool, Task* task, void* args) {
    Fld_GetMess*      mess     = task->data;
    Fld_GetMess_Args* messArgs = args;

    func_ov031_0210aaac(&mess->unk_4C, 1, 1);
    func_ov031_0210ab54(&mess->unk_4C, 1, 0);
    Fld_GetMess_Load(&mess->sprite, messArgs);
    Sprite_Update(&mess->sprite);

    mess->unk_4A = 2;
    mess->unk_40 = 0x180000;
    mess->unk_44 = 0x80000;
    mess->unk_48 = 0x14;

    if (messArgs->unk_6 == 0xFFFF) {
        mess->unk_C8 = func_ov031_0210b698(&mess->unk_4C, messArgs->unk_4);
        func_ov031_0210ab3c(&mess->unk_4C, 1, func_ov031_0210c320(&mess->unk_4C, mess->unk_C8));
        func_ov031_0210ab28(&mess->unk_4C, 0xD, 9);
    } else if (messArgs->unk_6 == 0xFFFE) {
        mess->unk_C8 = func_ov031_0210b698(&mess->unk_4C, messArgs->unk_4);
        func_ov030_020c4b8c(&mess->unk_4C, mess->unk_C8, 0, mess);
    } else {
        mess->unk_C8 = func_ov030_020c4ae8(&mess->unk_4C, messArgs->unk_4, messArgs->unk_6);
        func_ov030_020c4b8c(&mess->unk_4C, mess->unk_C8, 0, mess);
    }
    return 1;
}

s32 Fld_GetMess_Update(TaskPool* pool, Task* task, void* args) {
    Fld_GetMess* mess = task->data;

    if (mess->unk_48 != 0) {
        func_020265d4(&mess->unk_40, mess->unk_44, mess->unk_48);
        mess->unk_48--;
        mess->sprite.posX = F2I(mess->unk_40);
    }
    return 1;
}

s32 Fld_GetMess_Render(TaskPool* pool, Task* task, void* args) {
    Fld_GetMess* mess = task->data;

    Sprite_RenderFrame(&mess->sprite);
    if (mess->unk_4A != 0) {
        mess->unk_4A--;
        if ((mess->unk_4A & 0xFF) == 0) {
            func_ov031_0210beb0(&mess->unk_4C, mess->unk_C8, &mess->sprite, 1);
            Mem_Free(&gDebugHeap, mess->unk_C8);
            func_ov031_0210aabc(&mess->unk_4C);
            mess->unk_C8 = NULL;
        }
    }
    return 1;
}

s32 Fld_GetMess_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_GetMess* mess = task->data;

    Sprite_Destroy(&mess->sprite);
    if (mess->unk_C8 != NULL) {
        Mem_Free(&gDebugHeap, mess->unk_C8);
        func_ov031_0210aabc(&mess->unk_4C);
    }
    return 1;
}

s32 Fld_GetMess_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_GetMess_Init,
        .update     = Fld_GetMess_Update,
        .render     = Fld_GetMess_Render,
        .cleanup    = Fld_GetMess_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_GetMess_CreateTask(TaskPool* arg0, s32 arg1, s16 arg2, s16 arg3) {
    Fld_GetMess_Args args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    args.unk_6 = arg3;
    return EasyTask_CreateTask(arg0, &Tsk_Fld_GetMess, NULL, 0, NULL, &args);
}
