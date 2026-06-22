#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x000 */ Sprite sprites[8];
    /* 0x200 */ s32    unk_200;
    /* 0x204 */ void*  unk_204;
} MenuTop_nameD; // Size: 0x208

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s32 unk_4;
} MenuTop_nameD_Args;

static SpriteFrameInfo* MenuTop_nameD_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_nameD = {"Tsk_MenuTop_nameD", MenuTop_nameD_RunTask, sizeof(MenuTop_nameD)};

static const SpriteAnimation data_ov043_020c7c90 = {
    .bits_0_1   = 1,
    .dataType   = 2,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuTop_nameD_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c7988,
    .unk_18     = 0,
    .packIndex  = 1,
    .unk_1C     = 0,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 4,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuTop_nameD_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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
static void MenuTop_nameD_Load(Sprite* sprites, MenuTop_nameD_Args* args) {
    SpriteAnimation anim = data_ov043_020c7c90;

    const Point data_ov043_020c7c70[8] = {
        {  8, 139},
        { 91, 139},
        {183, 139},
        {  8, 152},
        {  8, 165},
        {140, 165},
        {  6, 183},
        {135, 183},
    };

    const s16 data_ov043_020c7c60[8][2] = {
        {2, 3},
        {4, 5},
        {6, 7},
        {8, 9},
    };

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 8; i++) {
        anim.unk_2A = data_ov043_020c7c60[i][0];
        anim.unk_04 = data_ov043_020c7c70[i].x;
        anim.unk_06 = data_ov043_020c7c70[i].y;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuTop_nameD_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameD*      nameD    = task->data;
    MenuTop_nameD_Args* initArgs = args;

    nameD->unk_204 = (void*)initArgs->unk_4;
    nameD->unk_200 = 1;
    MenuTop_nameD_Load(nameD->sprites, initArgs);
    return 1;
}

static s32 MenuTop_nameD_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameD* nameD = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_Update(&nameD->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_nameD_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameD* nameD = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_RenderFrame(&nameD->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_nameD_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameD* nameD = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_Release(&nameD->sprites[i]);
    }
    return 1;
}

s32 MenuTop_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_nameD_Init,
        .update     = MenuTop_nameD_Update,
        .render     = MenuTop_nameD_Render,
        .cleanup    = MenuTop_nameD_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

void MenuTop_nameD_CreateTask(TaskPool* pool, s32 dataType, s32 arg2) {
    MenuTop_nameD_Args args;

    args.dataType = dataType;
    args.unk_4    = arg2;

    EasyTask_CreateTask(pool, &Tsk_MenuTop_nameD, NULL, 0, NULL, &args);
}
