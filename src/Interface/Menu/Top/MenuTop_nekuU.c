#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite             sprite;
    /* 0x40 */ BOOL               shouldRender;
    /* 0x44 */ UnkStruct_TopMenu* unk_44;
    /* 0x48 */ Point              unk_48;
    /* 0x4C */ BOOL               unk_4C;
} MenuTop_nekuU; // Size: 0x50

typedef struct {
    /* 0x0 */ s32                dataType;
    /* 0x4 */ UnkStruct_TopMenu* unk_4;
} MenuTop_nekuU_Args;

static SpriteFrameInfo* MenuTop_nekuU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_nekuU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_nekuU = {"Tsk_MenuTop_nekuU", MenuTop_nekuU_RunTask, sizeof(MenuTop_nekuU)};

static const SpriteAnimation MenuTop_nekuU_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x800,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuTop_nekuU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c79a8,
    .unk_18     = 0,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuTop_nekuU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

static void MenuTop_nekuU_Load(MenuTop_nekuU* nekuU, Sprite* sprite, MenuTop_nekuU_Args* args) {
    UnkStruct_TopMenu* unkTemp = nekuU->unk_44;
    SpriteAnimation    anim    = MenuTop_nekuU_Anim;

    anim.dataType = args->dataType;

    if (unkTemp->unk_4E < 21) {
        anim.unk_2A         = 13;
        anim.unk_04         = data_ov043_020c7914[unkTemp->unk_4E].x;
        anim.unk_06         = data_ov043_020c7914[unkTemp->unk_4E].y - 2;
        nekuU->shouldRender = TRUE;
        nekuU->unk_4C       = TRUE;
    } else {
        anim.unk_2A         = 13;
        anim.unk_04         = 0;
        anim.unk_06         = 0;
        nekuU->shouldRender = FALSE;
        nekuU->unk_4C       = FALSE;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_nekuU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_nekuU*      nekuU    = task->data;
    MenuTop_nekuU_Args* helpArgs = args;

    nekuU->unk_44   = helpArgs->unk_4;
    nekuU->unk_48.x = 40;
    nekuU->unk_48.y = 0;
    MenuTop_nekuU_Load(nekuU, &nekuU->sprite, helpArgs);
    return 1;
}

static s32 MenuTop_nekuU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_nekuU* nekuU = task->data;

    nekuU->shouldRender = nekuU->unk_44->unk_66;

    Sprite_Update(&nekuU->sprite);
    return 1;
}

static s32 MenuTop_nekuU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_nekuU* nekuU = task->data;

    if (nekuU->shouldRender) {
        Sprite_RenderFrame(&nekuU->sprite);
    }
    return 1;
}

static s32 MenuTop_nekuU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_nekuU* nekuU = task->data;

    Sprite_Release(&nekuU->sprite);
    return 1;
}

static s32 MenuTop_nekuU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_nekuU_Init,
        .update     = MenuTop_nekuU_Update,
        .render     = MenuTop_nekuU_Render,
        .cleanup    = MenuTop_nekuU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_nekuU_CreateTask(TaskPool* pool, s32 dataType, UnkStruct_TopMenu* arg2) {
    MenuTop_nekuU_Args args;
    args.dataType = dataType;
    args.unk_4    = arg2;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_nekuU, NULL, 0, NULL, &args);
}