#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"
#include "SndMgr.h"

typedef struct {
    /* 0x000 */ Sprite             sprites[5];
    /* 0x140 */ s32                visibleFlags[5];
    /* 0x154 */ UnkStruct_TopMenu* unk_154;
    /* 0x158 */ u16                previousLevel;
} MenuTop_numLV; // Size: 0x15C

typedef struct {
    /* 0x0 */ s32   dataType;
    /* 0x4 */ void* owner;
    /* 0x8 */ u16   level;
    /* 0xA */ u16   bonus;
} MenuTop_numLV_Args;

extern s32 func_ov043_02084620(Sprite* sprite, s16 frameIndex);

static SpriteFrameInfo* MenuTop_numLV_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_numLV_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const s16 data_ov043_020c7d7c[5] = {
    0x32, 0x38, 0x40, 0x48, 0x4E,
};

static const TaskHandle Tsk_MenuTop_numLV = {"Tsk_MenuTop_numLV", MenuTop_numLV_RunTask, sizeof(MenuTop_numLV)};

static const SpriteAnimation data_ov043_020c7da4 = {
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
    .unk_08     = MenuTop_numLV_GetFrameInfo,
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

// Nonmatching
void func_ov043_020875cc(MenuTop_numLV* taskData) {
    u16 level = taskData->unk_154->unk_06;
    s16 tensFrame;
    s16 onesFrame;
    s16 onesXOffset;

    if (level >= 100) {
        tensFrame                 = 1;
        taskData->visibleFlags[0] = 0;
        onesFrame                 = 0x15;
        onesXOffset               = -3;
    } else {
        u32 signedBit = level >> 0x1F;
        s16 tens      = (s16)(signedBit + (level / 10));
        s16 ones      = (s16)(level - (10 * tens));

        tensFrame = tens + 10;
        onesFrame = ones + 10;

        if (level >= 10) {
            taskData->visibleFlags[0] = 1;
            taskData->visibleFlags[1] = 1;
            onesXOffset               = 0;
        } else {
            taskData->visibleFlags[0] = 0;
            taskData->visibleFlags[1] = 1;
            onesXOffset               = -3;
        }
    }

    func_ov043_02084620(&taskData->sprites[0], tensFrame);
    func_ov043_02084620(&taskData->sprites[1], onesFrame);
    taskData->sprites[0].posX = 0x32;
    taskData->sprites[1].posX = (s16)(onesXOffset + 0x38);
}

static SpriteFrameInfo* MenuTop_numLV_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

// Nonmatching
void MenuTop_numLV_Load(void* taskDataPtr, void* spritesPtr, void* argsPtr) {
    MenuTop_numLV*      taskData       = taskDataPtr;
    Sprite*             sprites        = spritesPtr;
    MenuTop_numLV_Args* args           = argsPtr;
    SpriteAnimation     anim           = data_ov043_020c7da4;
    s16                 digitFrames[5] = {0};
    s16                 xOffsets[5]    = {0};

    anim.dataType = args->dataType;

    for (s32 i = 0; i < 5; i++) {
        taskData->visibleFlags[i] = 1;
    }

    if (args->level >= 100) {
        digitFrames[0]            = 1;
        digitFrames[1]            = 0x15;
        taskData->visibleFlags[0] = 0;
        xOffsets[0]               = 1;
        xOffsets[1]               = -3;
    } else {
        u32 signedBit = args->level >> 0x1F;
        s16 tens      = (s16)(signedBit + (args->level / 10));
        s16 ones      = (s16)(args->level - (10 * tens));

        digitFrames[0] = (s16)(tens + 0xA);
        digitFrames[1] = (s16)(ones + 0xA);

        if (args->level >= 10) {
            xOffsets[1] = 0;
        } else {
            taskData->visibleFlags[0] = 0;
            xOffsets[1]               = -3;
        }
    }

    digitFrames[2] = 0x14;

    if (args->bonus >= 100) {
        digitFrames[3]            = 1;
        digitFrames[4]            = 0x15;
        taskData->visibleFlags[3] = 0;
        xOffsets[3]               = 1;
        xOffsets[4]               = -3;
    } else {
        u32 signedBit = args->bonus >> 0x1F;
        s16 tens      = (s16)(signedBit + (args->bonus / 10));
        s16 ones      = (s16)(args->bonus - (10 * tens));

        digitFrames[3] = (s16)(tens + 0xA);
        digitFrames[4] = (s16)(ones + 0xA);

        if (args->bonus >= 10) {
            xOffsets[4] = 0;
        } else {
            taskData->visibleFlags[3] = 0;
            xOffsets[4]               = -3;
        }
    }

    for (u16 i = 0; i < 5; i++) {
        anim.unk_2A = digitFrames[i];
        anim.unk_04 = (s16)(data_ov043_020c7d7c[i] + xOffsets[i]);
        anim.unk_06 = 0x8B;
        _Sprite_Load(&sprites[i], &anim);
    }

    taskData->previousLevel = args->level;
}

static s32 MenuTop_numLV_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_numLV* taskData = task->data;

    MenuTop_numLV_Load(taskData, taskData, args);
    taskData->unk_154 = ((MenuTop_numLV_Args*)args)->owner;
    return 1;
}

static s32 MenuTop_numLV_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_numLV*     taskData = task->data;
    UnkStruct_TopMenu* temp     = taskData->unk_154;

    func_ov043_020875cc(taskData);
    if (taskData->previousLevel != temp->unk_06) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
        taskData->previousLevel = temp->unk_06;
    }

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }

    return 1;
}

static s32 MenuTop_numLV_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_numLV* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (taskData->visibleFlags[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }

    return 1;
}

static s32 MenuTop_numLV_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_numLV* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }

    return 1;
}

s32 MenuTop_numLV_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_numLV_Init,
        .update     = MenuTop_numLV_Update,
        .render     = MenuTop_numLV_Render,
        .cleanup    = MenuTop_numLV_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void MenuTop_numLV_CreateTask(TaskPool* pool, s32 dataType, UnkStruct_TopMenu* arg2) {
    MenuTop_numLV_Args args;

    args.dataType = dataType;
    args.owner    = arg2;
    args.level    = arg2->unk_06;
    args.bonus    = arg2->unk_08;

    EasyTask_CreateTask(pool, &Tsk_MenuTop_numLV, NULL, 0, NULL, &args);
}
