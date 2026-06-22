#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ void*  unk_44;
} MenuTop_luckStar; // Size: 0x48

typedef struct {
    /* 0x0 */ s32   unk_0;
    /* 0x4 */ void* unk_4;
} MenuTop_luckStar_Args;

extern BinIdentifier data_ov043_020c79c8;

static SpriteFrameInfo* MenuTop_luckStar_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_luckStar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_luckStar = {"Tsk_MenuTop_luckStar", MenuTop_luckStar_RunTask, sizeof(MenuTop_luckStar)};

static const SpriteAnimation data_ov043_020c7c18 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0xCA,
    .unk_06     = 0xB6,
    .unk_08     = MenuTop_luckStar_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c79c8,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 11,
};

static SpriteFrameInfo* MenuTop_luckStar_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

static void MenuTop_luckStar_Load(Sprite* sprite, MenuTop_luckStar_Args* args) {
    SpriteAnimation anim = data_ov043_020c7c18;

    anim.dataType = args->unk_0;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_luckStar_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckStar*      luckStar = task->data;
    MenuTop_luckStar_Args* initArgs = args;

    MenuTop_luckStar_Load(&luckStar->sprite, initArgs);
    luckStar->unk_44 = initArgs->unk_4;
    return 1;
}

static s32 MenuTop_luckStar_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckStar* luckStar = task->data;

    Sprite_Update(&luckStar->sprite);
    return 1;
}

static s32 MenuTop_luckStar_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckStar* luckStar = task->data;

    Sprite_RenderFrame(&luckStar->sprite);
    return 1;
}

static s32 MenuTop_luckStar_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckStar* luckStar = task->data;

    Sprite_Release(&luckStar->sprite);
    return 1;
}

static s32 MenuTop_luckStar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_luckStar_Init,
        .update     = MenuTop_luckStar_Update,
        .render     = MenuTop_luckStar_Render,
        .cleanup    = MenuTop_luckStar_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

void MenuTop_luckStar_CreateTask(TaskPool* pool, s32 arg1, UnkStruct_TopMenu* arg2) {
    MenuTop_luckStar_Args args;

    args.unk_0 = arg1;
    args.unk_4 = arg2;
    EasyTask_CreateTask(pool, &Tsk_MenuTop_luckStar, NULL, 0, NULL, &args);
}
