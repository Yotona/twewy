#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite             sprite;
    /* 0x40 */ s32                unk_40;
    /* 0x44 */ UnkStruct_TopMenu* unk_44;
} MenuTop_brdLogoU; // Size: 0x48

typedef struct {
    /* 0x0 */ s32                dataType;
    /* 0x4 */ UnkStruct_TopMenu* unk_4;
} MenuTop_brdLogoU_Args;

static SpriteFrameInfo* MenuTop_brdLogoU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_brdLogoU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_brdLogoU = {"Tsk_MenuTop_brdLogoU", MenuTop_brdLogoU_RunTask, sizeof(MenuTop_brdLogoU)};

static const SpriteAnimation MenuTop_brdLogoU_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x800,
    .unk_04     = 0x3E,
    .unk_06     = 0x3C,
    .unk_08     = MenuTop_brdLogoU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c79d8,
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

static SpriteFrameInfo* MenuTop_brdLogoU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

static void MenuTop_brdLogoU_Load(MenuTop_brdLogoU* brdLogoU, Sprite* sprite, MenuTop_brdLogoU_Args* args) {
    UnkStruct_TopMenu* topMenu = brdLogoU->unk_44;
    SpriteAnimation    anim    = MenuTop_brdLogoU_Anim;

    anim.dataType = args->dataType;

    if (topMenu->unk_4E < 21) {
        anim.packIndex   = topMenu->unk_20[0] + 1;
        brdLogoU->unk_40 = 1;
    } else if (topMenu->unk_4E >= 22) {
        anim.packIndex   = 1;
        brdLogoU->unk_40 = 0;
    } else {
        anim.packIndex           = 1;
        *(u32*)&brdLogoU->unk_40 = 0;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_brdLogoU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_brdLogoU*      brdLogoU    = task->data;
    MenuTop_brdLogoU_Args* brdLogoArgs = args;

    brdLogoU->unk_44 = brdLogoArgs->unk_4;
    MenuTop_brdLogoU_Load(brdLogoU, &brdLogoU->sprite, brdLogoArgs);
    return 1;
}

static s32 MenuTop_brdLogoU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_brdLogoU* brdLogoU = task->data;

    Sprite_Update(&brdLogoU->sprite);
    return 1;
}

static s32 MenuTop_brdLogoU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_brdLogoU* brdLogoU = task->data;

    if (brdLogoU->unk_40 != 0) {
        Sprite_RenderFrame(&brdLogoU->sprite);
    }
    return 1;
}

static s32 MenuTop_brdLogoU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_brdLogoU* brdLogoU = task->data;

    Sprite_Release(&brdLogoU->sprite);
    return 1;
}

static s32 MenuTop_brdLogoU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_brdLogoU_Init,
        .update     = MenuTop_brdLogoU_Update,
        .render     = MenuTop_brdLogoU_Render,
        .cleanup    = MenuTop_brdLogoU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void MenuTop_brdLogoU_CreateTask(TaskPool* pool, s32 arg1, UnkStruct_TopMenu* arg2) {
    MenuTop_brdLogoU_Args args;
    args.dataType = arg1;
    args.unk_4    = arg2;
    EasyTask_CreateTask(pool, &Tsk_MenuTop_brdLogoU, NULL, 0, NULL, &args);
}
