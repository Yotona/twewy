#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite             sprite;
    /* 0x40 */ s32                unk_40;
    /* 0x44 */ UnkStruct_TopMenu* unk_44;
    /* 0x48 */ u16                unk_48;
} MenuTop_drawPtrAI; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32   dataType;
    /* 0x4 */ void* unk_4;
    /* 0x8 */ u16   unk_8;
} MenuTop_drawPtrAI_Args;

extern s32 func_ov043_02084620(Sprite* sprite, s16 frameIndex);

static SpriteFrameInfo* MenuTop_drawPtrAI_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_drawPtrAI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_drawPtrAI = {"Tsk_MenuTop_drawPtrAI", MenuTop_drawPtrAI_RunTask,
                                                 sizeof(MenuTop_drawPtrAI)};

static const SpriteAnimation data_ov043_020c7f30 = {
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
    .unk_08     = MenuTop_drawPtrAI_GetFrameInfo,
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

static s16 func_ov043_02088b80(void* arg0) {
    MenuTop_drawPtrAI* taskData = arg0;

    s16 data_ov043_020c7f0c[4] = {0x38, 0x3B, 0x3A, 0x39};

    return data_ov043_020c7f0c[taskData->unk_44->unk_14];
}

static SpriteFrameInfo* MenuTop_drawPtrAI_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

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
            break;
        }
    }

    return info;
}

static void MenuTop_drawPtrAI_Load(void* arg0, void* arg1, MenuTop_drawPtrAI_Args* args) {
    MenuTop_drawPtrAI* taskData = arg0;
    Sprite*            sprite   = arg1;
    SpriteAnimation    anim     = data_ov043_020c7f30;

    s32 val = func_ov043_02088b80(taskData);

    anim.dataType    = args->dataType;
    taskData->unk_40 = 1;
    anim.unk_2A      = val;
    anim.unk_04      = 218;
    anim.unk_06      = 164;

    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_drawPtrAI_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawPtrAI*      taskData = task->data;
    MenuTop_drawPtrAI_Args* initArgs = args;

    taskData->unk_44 = initArgs->unk_4;
    taskData->unk_48 = initArgs->unk_8;
    MenuTop_drawPtrAI_Load(taskData, &taskData->sprite, initArgs);
    return 1;
}

static s32 MenuTop_drawPtrAI_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawPtrAI* taskData = task->data;

    func_ov043_02084620(&taskData->sprite, func_ov043_02088b80(taskData));
    Sprite_Update(&taskData->sprite);
    return 1;
}

static s32 MenuTop_drawPtrAI_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawPtrAI* taskData = task->data;

    if (taskData->unk_40 != 0) {
        Sprite_RenderFrame(&taskData->sprite);
    }
    return 1;
}

static s32 MenuTop_drawPtrAI_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawPtrAI* taskData = task->data;

    Sprite_Release(&taskData->sprite);
    return 1;
}

s32 MenuTop_drawPtrAI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_drawPtrAI_Init,
        .update     = MenuTop_drawPtrAI_Update,
        .render     = MenuTop_drawPtrAI_Render,
        .cleanup    = MenuTop_drawPtrAI_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

void MenuTop_drawPtrAI_CreateTask(TaskPool* pool, s32 arg1, UnkStruct_TopMenu* arg2) {
    MenuTop_drawPtrAI_Args args;

    args.dataType = arg1;
    args.unk_4    = arg2;
    args.unk_8    = arg2->unk_14;

    EasyTask_CreateTask(pool, &Tsk_MenuTop_drawPtrAI, NULL, 0, NULL, &args);
}
