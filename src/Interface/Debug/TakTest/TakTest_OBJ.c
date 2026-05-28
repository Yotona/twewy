#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
} TakTest_OBJ; // Size: 0x40

typedef struct {
    /* 0x0*/ s32 dataType;
} TakTest_OBJ_Args;

SpriteFrameInfo* TakTest_OBJ_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
s32              TakTest_OBJ_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

char data_ov043_020cb5d4[] = "Apl_Kit/GRP_FldDownScreen.bin";

const BinIdentifier        data_ov043_020c78cc = {43, data_ov043_020cb5d4}; // TODO: No known usages, breaking .rodata matching
static const BinIdentifier data_ov043_020c78d4 = {43, data_ov043_020cb5d4};

const SpriteAnimation data_ov043_020c78dc = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0xC0,
    .unk_06     = 0,
    .unk_08     = TakTest_OBJ_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c78d4,
    .unk_18     = 2,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

const TaskHandle Tsk_TakTest_OBJ = {"Tsk_TakTest_OBJ", TakTest_OBJ_RunTask, sizeof(TakTest_OBJ)};

SpriteFrameInfo* TakTest_OBJ_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

void TakTest_OBJ_Load(TakTest_OBJ* obj, TakTest_OBJ_Args* args) {
    SpriteAnimation anim = data_ov043_020c78dc;
    anim.dataType        = args->dataType;
    _Sprite_Load(&obj->sprite, &anim);
}

s32 TakTest_OBJ_Init(TaskPool* pool, Task* task, void* args) {
    TakTest_OBJ*      obj     = task->data;
    TakTest_OBJ_Args* objArgs = args;

    TakTest_OBJ_Load(obj, objArgs);
    return 1;
}

s32 TakTest_OBJ_Update(TaskPool* pool, Task* task, void* args) {
    TakTest_OBJ* obj = task->data;

    Sprite_Update(&obj->sprite);
    return 1;
}

s32 TakTest_OBJ_Render(TaskPool* pool, Task* task, void* args) {
    TakTest_OBJ* obj = task->data;

    Sprite_RenderFrame(&obj->sprite);
    return 1;
}

s32 TakTest_OBJ_Destroy(TaskPool* pool, Task* task, void* args) {
    TakTest_OBJ* obj = task->data;

    Sprite_Release(&obj->sprite);
    return 1;
}

s32 TakTest_OBJ_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = TakTest_OBJ_Init,
        .update     = TakTest_OBJ_Update,
        .render     = TakTest_OBJ_Render,
        .cleanup    = TakTest_OBJ_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TakTest_OBJ_CreateTask(TaskPool* pool, s32 arg1) {
    s32 sp8 = arg1;
    EasyTask_CreateTask(pool, &Tsk_TakTest_OBJ, 0, 0, 0, &sp8);
}
