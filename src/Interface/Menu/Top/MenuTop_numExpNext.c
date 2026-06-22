#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x000 */ Sprite             sprites[5];
    /* 0x140 */ s32                visibleFlags[5];
    /* 0x154 */ UnkStruct_TopMenu* unk_154;
} MenuTop_numExpNext; // Size: 0x158

typedef struct {
    /* 0x0 */ s32                dataType;
    /* 0x4 */ UnkStruct_TopMenu* unk_4;
    /* 0x8 */ u32                expNext;
} MenuTop_numExpNext_Args;

static SpriteFrameInfo* MenuTop_numExpNext_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuTop_numExpNext_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_numExpNext = {"Tsk_MenuTop_numExpNext", MenuTop_numExpNext_RunTask,
                                                  sizeof(MenuTop_numExpNext)};

static const SpriteAnimation data_ov043_020c7dec = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuTop_numExpNext_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c7988,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 4,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuTop_numExpNext_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

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

void MenuTop_numExpNext_Load(void* taskDataPtr, void* spritesPtr, void* argsPtr) {
    MenuTop_numExpNext*      taskData = taskDataPtr;
    Sprite*                  sprites  = spritesPtr;
    MenuTop_numExpNext_Args* args     = argsPtr;
    SpriteAnimation          anim     = data_ov043_020c7dec;
    u32                      value;
    u32                      digits[5];

    anim.dataType = args->dataType;

    value     = args->expNext;
    digits[0] = value / 10000;
    value %= 10000;
    digits[1] = value / 1000;
    value %= 1000;
    digits[2] = value / 100;
    value %= 100;
    digits[3] = value / 10;
    digits[4] = value % 10;

    for (s32 i = 0; i < 5; i++) {
        taskData->visibleFlags[i] = 1;
    }

    for (s32 i = 0; i < 4 && digits[i] == 0; i++) {
        taskData->visibleFlags[i] = 0;
    }

    for (s32 i = 0; i < 5; i++) {
        anim.unk_2A = (s16)(digits[i] + 0xA);
        anim.unk_04 = (s16)((i * 6) + 0x87);
        anim.unk_06 = 0x8B;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuTop_numExpNext_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_numExpNext*      taskData = task->data;
    MenuTop_numExpNext_Args* initArgs = args;

    MenuTop_numExpNext_Load(taskData, taskData, initArgs);
    taskData->unk_154 = initArgs->unk_4;
    return 1;
}

static s32 MenuTop_numExpNext_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_numExpNext* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_numExpNext_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_numExpNext* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (taskData->visibleFlags[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_numExpNext_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_numExpNext* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }
    return 1;
}

s32 MenuTop_numExpNext_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_numExpNext_Init,
        .update     = MenuTop_numExpNext_Update,
        .render     = MenuTop_numExpNext_Render,
        .cleanup    = MenuTop_numExpNext_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_numExpNext_CreateTask(TaskPool* pool, s32 dataType, UnkStruct_TopMenu* arg2) {
    MenuTop_numExpNext_Args args;

    args.dataType = dataType;
    args.unk_4    = arg2;
    args.expNext  = arg2->unk_0C;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_numExpNext, NULL, 0, NULL, &args);
}
