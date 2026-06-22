#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprites[2];
    /* 0x80 */ s32    visibleFlags[2];
    /* 0x88 */ void*  owner;
} MenuTop_rankNumU; // Size: 0x8C

typedef struct {
    /* 0x00 */ s32   dataType;
    /* 0x04 */ void* owner;
    /* 0x08 */ u16   positionIndex;
    /* 0x0A */ u16   value;
} MenuTop_rankNumU_Args;

static SpriteFrameInfo* MenuTop_rankNumU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_rankNumU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_rankNumU = {"Tsk_MenuTop_rankNumU", MenuTop_rankNumU_RunTask, sizeof(MenuTop_rankNumU)};

static const SpriteAnimation MenuTop_rankNumU_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x1280,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuTop_rankNumU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c79a8,
    .unk_18     = 0,
    .packIndex  = 1,
    .unk_1C     = 0,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static const Point data_ov043_020c7d40[] = {
    {13, 105},
    {13, 130},
    {13, 155},
    {13, 180},
};

// Nonmatching
static SpriteFrameInfo* MenuTop_rankNumU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* frameInfo = NULL;

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
                temp->unk_04 = *(u16*)(sprite->frameDataTable + (((sprite->unk16 * 4) + 1)));
                temp->unk_08 = (s32)(sprite->frameDataTable + (*(u16*)(sprite->frameDataTable + (sprite->unk16 * 1))));
            }

            frameInfo = temp;
        } break;
    }

    return frameInfo;
}

// Nonmatching
static void MenuTop_rankNumU_Load(MenuTop_rankNumU* taskData, Sprite* sprites, MenuTop_rankNumU_Args* args) {
    SpriteAnimation anim = MenuTop_rankNumU_Anim;

    s32 tens = args->value / 10;
    s32 ones = args->value - (tens * 10);
    s32 xOffset;

    anim.dataType = args->dataType;

    if (args->value >= 10) {
        taskData->visibleFlags[0] = 1;
        taskData->visibleFlags[1] = 1;
        xOffset                   = 0;
    } else {
        taskData->visibleFlags[0] = 0;
        taskData->visibleFlags[1] = 1;
        xOffset                   = -3;
    }

    anim.unk_2A = tens + 3;
    anim.unk_04 = data_ov043_020c7d40[args->positionIndex].x + xOffset;
    anim.unk_06 = data_ov043_020c7d40[args->positionIndex].y;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = ones + 3;
    anim.unk_04 = data_ov043_020c7d40[args->positionIndex].x + xOffset + 7;
    anim.unk_06 = data_ov043_020c7d40[args->positionIndex].y;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 MenuTop_rankNumU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_rankNumU*      taskData = task->data;
    MenuTop_rankNumU_Args* initArgs = args;

    taskData->owner = initArgs->owner;
    MenuTop_rankNumU_Load(taskData, taskData->sprites, initArgs);
    return 1;
}

static s32 MenuTop_rankNumU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_rankNumU* taskData = task->data;

    for (s16 i = 0; i < 2; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_rankNumU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_rankNumU* taskData = task->data;

    for (s16 i = 0; i < 2; i++) {
        if (taskData->visibleFlags[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }

    return 1;
}

static s32 MenuTop_rankNumU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_rankNumU* taskData = task->data;

    for (s16 i = 0; i < 2; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }
    return 1;
}

s32 MenuTop_rankNumU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_rankNumU_Init,
        .update     = MenuTop_rankNumU_Update,
        .render     = MenuTop_rankNumU_Render,
        .cleanup    = MenuTop_rankNumU_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

void MenuTop_rankNumU_CreateTask(TaskPool* pool, s32 dataType, s16 positionIndex, s16 value, s32 owner) {
    MenuTop_rankNumU_Args args;

    args.dataType      = dataType;
    args.owner         = (void*)owner;
    args.positionIndex = positionIndex;
    args.value         = value;

    EasyTask_CreateTask(pool, &Tsk_MenuTop_rankNumU, NULL, 0, NULL, &args);
}