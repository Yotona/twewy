#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Field/FieldTasks.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

extern BinIdentifier data_ov030_020d9944;

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ s32    unk_44;
    /* 0x48 */ u16    unk_48;
} Fld_WhichArrow; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s16 unk_4;
    /* 0x6 */ s16 unk_6;
    /* 0x8 */ s32 unk_8;
} Fld_WhichArrow_Args;

SpriteFrameInfo* Fld_WhichArrow_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
s32              Fld_WhichArrow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle      Tsk_Fld_WhichArrow  = {"Tsk_Fld_WhichArrow", Fld_WhichArrow_RunTask, 0x4C};
static const SpriteAnimation data_ov030_020ec938 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0x80,
    .unk_06     = 0,
    .unk_08     = Fld_WhichArrow_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov030_020d9944,
    .unk_18     = 2,
    .packIndex  = 12,
    .unk_1C     = 2,
    .unk_1E     = 0,
    .unk_20     = 1,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 3,
    .unk_28     = 4,
    .unk_2A     = 1,
};

SpriteFrameInfo* Fld_WhichArrow_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

void Fld_WhichArrow_Load(Sprite* sprite, Fld_WhichArrow_Args* arrowArgs, s16 arg2) {
    SpriteAnimation anim = data_ov030_020ec938;
    anim.dataType        = arrowArgs->dataType;
    anim.unk_06          = arg2;
    anim.unk_2A          = arrowArgs->unk_6;
    if (arrowArgs->unk_6 == 3) {
        anim.bits_12_13 = 2;
    }
    if (arrowArgs->unk_6 <= 3) {
        anim.bits_0_1 = 0;
    }
    _Sprite_Load(sprite, &anim);
}

s32 Fld_WhichArrow_Init(TaskPool* pool, Task* task, void* args) {
    Fld_WhichArrow*      arrow     = task->data;
    Fld_WhichArrow_Args* arrowArgs = args;

    s32 var_r6;

    if (arrowArgs->unk_6 == 4 || arrowArgs->unk_6 == 7 || arrowArgs->unk_6 == 10) {
        arrow->unk_48 = 0x14;
        var_r6        = 0xB000;
    } else if (arrowArgs->unk_6 == 5 || arrowArgs->unk_6 == 8 || arrowArgs->unk_6 == 11) {
        var_r6        = -0xB000;
        arrow->unk_48 = 0x14;
    } else {
        var_r6        = 0;
        arrow->unk_48 = 0;
        if (arrowArgs->unk_6 == 3) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
        }
    }
    var_r6 += I2F(arrowArgs->unk_4);

    arrow->unk_44 = I2F(arrowArgs->unk_4);
    arrow->unk_40 = var_r6;
    Fld_WhichArrow_Load(&arrow->sprite, arrowArgs, F2I(arrow->unk_40));
    return 1;
}

s32 Fld_WhichArrow_Update(TaskPool* pool, Task* task, void* args) {
    Fld_WhichArrow* arrow = task->data;

    Sprite_Update(&arrow->sprite);
    if (arrow->unk_48 != 0) {
        func_02026590(&arrow->unk_40, arrow->unk_44, arrow->unk_48);
        arrow->unk_48--;
        arrow->sprite.posY = F2I(arrow->unk_40);
    }
    return 1;
}

s32 Fld_WhichArrow_Render(TaskPool* pool, Task* task, void* args) {
    Fld_WhichArrow* arrow = task->data;

    Sprite_RenderFrame(&arrow->sprite);
    return 1;
}

s32 Fld_WhichArrow_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_WhichArrow* arrow = task->data;

    Sprite_Release(&arrow->sprite);
    return 1;
}

s32 Fld_WhichArrow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_WhichArrow_Init,
        .update     = Fld_WhichArrow_Update,
        .render     = Fld_WhichArrow_Render,
        .cleanup    = Fld_WhichArrow_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_WhichArrow_CreateTask(TaskPool* pool, s32 dataType, s16 arg2, s16 arg3, s32 arg4) {
    Fld_WhichArrow_Args args;
    args.dataType = dataType;
    args.unk_4    = arg2;
    args.unk_6    = arg3;
    args.unk_8    = arg4;
    return EasyTask_CreateTask(pool, &Tsk_Fld_WhichArrow, 0, 0, 0, &args);
}
