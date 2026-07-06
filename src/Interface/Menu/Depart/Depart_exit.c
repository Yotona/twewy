#include "Interface/Menu/Depart.h"

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ s32           unk_40;
    /* 0x44 */ DepartObject* depart;
} DepartExit; // Size: 0x48

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ DepartObject* depart;
} DepartExitArgs;

static SpriteFrameInfo* func_ov043_020be368(Sprite* sprite, s32 arg1, s32 arg2);
static s32              DepartExit_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle data_ov043_020cadec = {"Tsk_Depart_exit", DepartExit_RunTask, sizeof(DepartExit)};

static const SpriteAnimation data_ov043_020cae08 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0xE7,
    .unk_06     = 0x9,
    .unk_08     = func_ov043_020be368,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020cad88,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* func_ov043_020be368(Sprite* sprite, s32 arg1, s32 mode) {
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

static void func_ov043_020be404(void* arg1, Sprite* sprite, DepartExitArgs* args) {
    SpriteAnimation anim = data_ov043_020cae08;

    anim.dataType = args->dataType;
    anim.unk_2A   = 1;

    _Sprite_Load(sprite, &anim);
}

static s32 DepartExit_Init(TaskPool* pool, Task* task, void* args) {
    DepartExit*     departExit     = task->data;
    DepartExitArgs* departExitArgs = args;

    departExit->unk_40 = 1;
    departExit->depart = departExitArgs->depart;
    func_ov043_020be404(departExit, &departExit->sprite, departExitArgs);
    return 1;
}

static s32 DepartExit_Update(TaskPool* pool, Task* task, void* args) {
    DepartExit*   departExit = task->data;
    DepartObject* depart     = departExit->depart;

    if (depart->unk_30 == 1) {
        func_ov043_020bd938(&departExit->sprite, 2);
        if (depart->unk_31 != 0) {
            depart->unk_31--;
        } else {
            depart->unk_30 = 0;
        }
    } else {
        func_ov043_020bd938(&departExit->sprite, 1);
    }
    Sprite_Update(&departExit->sprite);
    return 1;
}

static s32 DepartExit_Render(TaskPool* pool, Task* task, void* args) {
    DepartExit* departExit = task->data;

    Sprite_RenderFrame(&departExit->sprite);
    return 1;
}

static s32 DepartExit_Release(TaskPool* pool, Task* task, void* args) {
    DepartExit* departExit = task->data;

    Sprite_Release(&departExit->sprite);
    return 1;
}

static s32 DepartExit_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = DepartExit_Init,
        .update     = DepartExit_Update,
        .render     = DepartExit_Render,
        .cleanup    = DepartExit_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 DepartExit_CreateTask(TaskPool* pool, s32 dataType, DepartObject* depart) {
    DepartExitArgs args;
    args.dataType = dataType;
    args.depart   = depart;
    return EasyTask_CreateTask(pool, &data_ov043_020cadec, NULL, 0, NULL, &args);
}
