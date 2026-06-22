#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite sprites[2];
    /* 0x80 */ s32    unk_80[2];
    /* 0x88 */ s32    unk_88;
} MenuTop_helpCurU; // Size: 0x8C

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s32 unk_4;
} MenuTop_helpCurU_Args;

static SpriteFrameInfo* MenuTop_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_helpCurU = {"Tsk_MenuTop_helpCurU", MenuTop_helpCurU_RunTask, sizeof(MenuTop_helpCurU)};

static const SpriteAnimation MenuTop_helpCurU_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuTop_helpCurU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c79a8,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuTop_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

static void MenuTop_helpCurU_Load(Sprite* sprites, MenuTop_helpCurU_Args* args) {
    SpriteAnimation anim = MenuTop_helpCurU_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 27;
    anim.unk_04 = 7;
    anim.unk_06 = 96;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 28;
    anim.unk_04 = 249;
    anim.unk_06 = 96;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 MenuTop_helpCurU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCurU*      helpCurU = task->data;
    MenuTop_helpCurU_Args* helpArgs = args;

    helpCurU->unk_88 = helpArgs->unk_4;
    MenuTop_helpCurU_Load(helpCurU->sprites, helpArgs);
    return 1;
}

static s32 MenuTop_helpCurU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCurU* helpCurU = task->data;

    u8 val = *(u8*)(helpCurU->unk_88 + 0x60);
    if (val == 0) {
        helpCurU->unk_80[0] = 0;
        helpCurU->unk_80[1] = 1;
    } else if (val == 6) {
        helpCurU->unk_80[0] = 1;
        helpCurU->unk_80[1] = 0;
    } else {
        helpCurU->unk_80[0] = 1;
        helpCurU->unk_80[1] = 1;
    }

    for (u16 i = 0; i < 2; i++) {
        Sprite_Update(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_helpCurU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        if (helpCurU->unk_80[i] != 0) {
            Sprite_RenderFrame(&helpCurU->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_helpCurU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_Release(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_helpCurU_Init,
        .update     = MenuTop_helpCurU_Update,
        .render     = MenuTop_helpCurU_Render,
        .cleanup    = MenuTop_helpCurU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_helpCurU_CreateTask(TaskPool* pool, s32 dataType, s32 arg2) {
    MenuTop_helpCurU_Args args;
    args.dataType = dataType;
    args.unk_4    = arg2;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_helpCurU, NULL, 0, NULL, &args);
}