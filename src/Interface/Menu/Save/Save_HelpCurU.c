#include "Interface/Menu/Save.h"

typedef struct {
    /* 0x00 */ Sprite sprites[2];
    /* 0x80 */ s32    unk_80[2];
    /* 0x88 */ void*  unk_88;
} SaveHelpCurU; // Size: 0x8C

typedef struct {
    /* 0x00 */ s32   dataType;
    /* 0x04 */ void* unk_04;
} HelpCurUArgs;

SpriteFrameInfo* Save_HelpCurU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
s32              Save_HelpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Save_HelpCurU_Animation = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Save_HelpCurU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020caf88,
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

static const TaskHandle Tsk_Save_helpCurU = {"Tsk_Save_helpCurU", Save_HelpCurU_RunTask, sizeof(SaveHelpCurU)};

SpriteFrameInfo* Save_HelpCurU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

void Save_HelpCurU_Load(SaveHelpCurU* helpCurU, HelpCurUArgs* args) {
    SpriteAnimation anim = Save_HelpCurU_Animation;
    anim.dataType        = args->dataType;

    anim.unk_2A = 33;
    anim.unk_04 = 7;
    anim.unk_06 = 96;
    _Sprite_Load(&helpCurU->sprites[0], &anim);

    anim.unk_2A = 34;
    anim.unk_04 = 249;
    anim.unk_06 = 96;
    _Sprite_Load(&helpCurU->sprites[1], &anim);
}

s32 Save_HelpCurU_Init(TaskPool* pool, Task* task, void* args) {
    SaveHelpCurU* helpCurU = task->data;
    HelpCurUArgs* curUArgs = args;

    helpCurU->unk_88 = curUArgs->unk_04;
    Save_HelpCurU_Load(helpCurU, curUArgs);
    return 1;
}

s32 Save_HelpCurU_Update(TaskPool* pool, Task* task, void* args) {
    SaveHelpCurU* helpCurU = task->data;

    if (*(u8*)(helpCurU->unk_88 + 0x1A4) == 0) {
        helpCurU->unk_80[0] = 0;
        helpCurU->unk_80[1] = 1;
    } else if (*(u8*)(helpCurU->unk_88 + 0x1A4) == 3) {
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

s32 Save_HelpCurU_Render(TaskPool* pool, Task* task, void* args) {
    SaveHelpCurU* helpCurU = task->data;
    for (u16 i = 0; i < 2; i++) {
        if (helpCurU->unk_80[i] != 0) {
            Sprite_RenderFrame(&helpCurU->sprites[i]);
        }
    }
    return 1;
}

s32 Save_HelpCurU_Destroy(TaskPool* pool, Task* task, void* args) {
    SaveHelpCurU* helpCurU = task->data;
    for (u16 i = 0; i < 2; i++) {
        Sprite_Release(&helpCurU->sprites[i]);
    }
    return 1;
}

s32 Save_HelpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Save_HelpCurU_Init,
        .update     = Save_HelpCurU_Update,
        .render     = Save_HelpCurU_Render,
        .cleanup    = Save_HelpCurU_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Save_HelpCurU_CreateTask(TaskPool* pool, s32 dataType, void* arg2) {
    HelpCurUArgs args;
    args.dataType = dataType;
    args.unk_04   = arg2;
    return EasyTask_CreateTask(pool, &Tsk_Save_helpCurU, NULL, 0, NULL, &args);
}
