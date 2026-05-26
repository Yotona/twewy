#include "Interface/Save.h"

typedef struct {
    /* 0x000 */ s32    unk_000;
    /* 0x004 */ Sprite sprites[8];
    /* 0x204 */ s32    unk_204[8];
    /* 0x224 */ s32    unk_224;
} Save_BdgU; // Size: 0x228

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
    /* 0x8 */ s32 unk_8;
} Save_BdgU_Args;

s32 Save_BdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_bdgU = {"Tsk_Save_bdgU", Save_BdgU_RunTask, sizeof(Save_BdgU)};

void func_ov043_020c53a0(void* arg1, s32* arg2, s32 arg3) {
    // Not yet implemented
}

SpriteFrameInfo* Save_BdgU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* temp = &data_0206b408;

    switch (mode) {
        case 1: {
            temp->unk_00 = 1;
            return temp;
        } break;

        case 2: {
            temp         = &data_0206b408;
            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            return temp;
        } break;
    }

    return NULL;
}

void Save_BdgU_Load(Save_BdgU* bdgU, Sprite* sprite, Save_BdgU_Args* bdgUArgs) {
    // Not yet implemented
}

s32 Save_BdgU_Init(TaskPool* pool, Task* task, void* args) {
    Save_BdgU*      bdgU     = task->data;
    Save_BdgU_Args* bdgUArgs = args;

    bdgU->unk_000 = bdgUArgs->unk_0;
    bdgU->unk_224 = bdgUArgs->unk_4;
    Save_BdgU_Load(bdgU, bdgU->sprites, bdgUArgs);
    return 1;
}

s32 Save_BdgU_Update(TaskPool* pool, Task* task, void* args) {
    Save_BdgU* bdgU = task->data;

    for (u16 i = 0; i < 8; i++) {
        Sprite_Update(&bdgU->sprites[i]);
    }
    return 1;
}

s32 Save_BdgU_Render(TaskPool* pool, Task* task, void* args) {
    Save_BdgU* bdgU = task->data;

    for (u16 i = 0; i < 8; i++) {
        if (bdgU->unk_204[i] != 0) {
            Sprite_RenderFrame(&bdgU->sprites[i]);
        }
    }
    return 1;
}

s32 Save_BdgU_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_BdgU* bdgU = task->data;

    for (u16 i = 0; i < 8; i++) {
        Sprite_Release(&bdgU->sprites[i]);
    }
    return 1;
}

s32 Save_BdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Save_BdgU_Init,
        .update     = Save_BdgU_Update,
        .render     = Save_BdgU_Render,
        .cleanup    = Save_BdgU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_BdgU_CreateTask(TaskPool* pool, s32 arg1, u16 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c5c98(TaskPool* pool, u32 taskId) {

    Sprite* sprite = EasyTask_GetTaskData(pool, taskId) + 0x1C4;

    Sprite_Release(sprite);
}
