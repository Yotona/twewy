#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Field/FieldTasks.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ s32    unk_44;
    /* 0x48 */ u16    unk_48;
    /* 0x4A */ u16    unk_4A;
    /* 0x4C */ s32    unk_4C;
    /* 0x50 */ s32    unk_50;
    /* 0x54 */ u32    unk_54;
    /* 0x58 */ s8     unk_58;
} Fld_TalkEventStringPlate; // Size: 0x5C

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x10 */ s32 unk_10;
} Fld_TalkEventStringPlate_Args;

SpriteFrameInfo* Fld_TalkEventStringPlate_GetFrameInfo(Sprite* sprite, s32 arg1, s32 arg2);
s32              Fld_TalkEventStringPlate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TalkEventStringPlate = {"Tsk_Fld_TalkEventStringPlate", Fld_TalkEventStringPlate_RunTask,
                                                        0x5C};

extern const BinIdentifier data_ov030_020d9944;

const SpriteAnimation data_ov030_020e71d0 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 1,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0,
    .unk_06     = 0,
    .unk_08     = Fld_TalkEventStringPlate_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov030_020d9944,
    .unk_18     = 2,
    .packIndex  = 8,
    .unk_1C     = 2,
    .unk_1E     = 0,
    .unk_20     = 1,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 3,
    .unk_28     = 4,
    .unk_2A     = 1,
};

SpriteFrameInfo* Fld_TalkEventStringPlate_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    // Not yet implemented
}

void Fld_TalkEventStringPlate_Load(Fld_TalkEventStringPlate* plate, Fld_TalkEventStringPlate_Args* args, s32 arg2, s16 arg3,
                                   void* arg4) {
    SpriteAnimation anim = data_ov030_020e71d0;
    anim.dataType        = args->unk_00;
    anim.unk_04          = arg2;
    anim.unk_06          = arg3;

    if (args->unk_08 != 4) {
        anim.unk_2A += (s16)args->unk_08;
    } else {
        anim.unk_2A    = 1;
        anim.packIndex = 13;
        anim.bits_0_1  = 0;
    }
    anim.unk_10 = arg4;
    _Sprite_Load(&plate->sprite, &anim);
}

s32 Fld_TalkEventStringPlate_Init(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventStringPlate*      plate     = task->data;
    Fld_TalkEventStringPlate_Args* plateArgs = args;

    plate->unk_44 = I2F(plateArgs->unk_04);
    plate->unk_50 = plateArgs->unk_10;
    if (plateArgs->unk_08 == 4) {
        plate->unk_40 = I2F(plateArgs->unk_04);
        plate->unk_48 = 0;
    } else if (plateArgs->unk_10 != 0) {
        plate->unk_40 = I2F((plateArgs->unk_04 < 0x80) ? plateArgs->unk_04 - 168 : plateArgs->unk_04 + 168);
        plate->unk_48 = 20;
    } else if (plateArgs->unk_08 != 4) {
        plate->unk_40 = I2F(plateArgs->unk_04);
        plate->unk_48 = 20;
        plate->unk_4C = 0;
    }
    Fld_TalkEventStringPlate_Load(plate, plateArgs, (plate->unk_40 * 0x10) >> 0x10, plateArgs->unk_06, plate);
    plate->unk_54 = func_ov030_020c4a78(pool, plate, plateArgs->unk_0C, &plate->unk_58, 1);
    return 1;
}

s32 Fld_TalkEventStringPlate_Update(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventStringPlate* plate = task->data;

    if (plate->unk_50 != 0) {
        if (plate->unk_48 != 0) {
            func_02026590(&plate->unk_40, plate->unk_44, plate->unk_48);
            plate->unk_48--;
            plate->sprite.posX = F2I(plate->unk_40);
        }
    } else if (plate->unk_48 != 0) {
        func_02026590(&plate->unk_4C, 0x1000, plate->unk_48);
        plate->unk_48--;
    }
    Sprite_Update(&plate->sprite);
    return 1;
}

s32 Fld_TalkEventStringPlate_Render(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventStringPlate* plate = task->data;

    Sprite_RenderFrame(&plate->sprite);
    plate->unk_58 = 1;
    return 1;
}

s32 Fld_TalkEventStringPlate_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventStringPlate* plate = task->data;

    Sprite_Release(&plate->sprite);
    if (EasyTask_ValidateTaskId(pool, &plate->unk_54) != 0) {
        EasyTask_DeleteTask(pool, plate->unk_54);
    }
    return 1;
}

s32 Fld_TalkEventStringPlate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TalkEventStringPlate_Init,
        .update     = Fld_TalkEventStringPlate_Update,
        .render     = Fld_TalkEventStringPlate_Render,
        .cleanup    = Fld_TalkEventStringPlate_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_TalkEventStringPlate_CreateTask(TaskPool* pool, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s16 arg5, s32 arg6) {
    Fld_TalkEventStringPlate_Args args;
    args.unk_00 = arg1;
    args.unk_04 = arg2;
    args.unk_06 = arg3;
    args.unk_08 = arg4;
    args.unk_0C = arg5;
    args.unk_10 = arg6;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TalkEventStringPlate, NULL, 0, NULL, &args);
}
