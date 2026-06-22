#include "Display.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Top.h"
#include "SndMgr.h"

extern s16 func_ov043_02084e20(s16 x, s16 y);

typedef struct {
    /* 0x00 */ Sprite sprites[5];
    /* 0x140 */ s32   visibleFlags[5];
    /* 0x154 */ void* owner;
    /* 0x158 */ s16   selectedIndex;
    /* 0x15A */ u16   state;
    /* 0x15C */ s16   delay;
    /* 0x15E */ s16   lastFrame;
    /* 0x160 */ s32   lastVisible;
} MenuTop_selPtrAI; // Size: 0x164

typedef struct {
    /* 0x00 */ s32   dataType;
    /* 0x04 */ void* owner;
    /* 0x08 */ s16   selectedIndex;
} MenuTop_selPtrAI_Args;

static SpriteFrameInfo* MenuTop_selPtrAI_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_selPtrAI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_selPtrAI = {"Tsk_MenuTop_selPtrAI", MenuTop_selPtrAI_RunTask, sizeof(MenuTop_selPtrAI)};

static const SpriteAnimation data_ov043_020c7fd0 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuTop_selPtrAI_GetFrameInfo,
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

static s16 func_ov043_02089374(u16 arg0) {
    s16 data_ov043_020c7fac[4] = {
        46,
        78,
        110,
        142,
    };
    return data_ov043_020c7fac[arg0];
}

static SpriteFrameInfo* MenuTop_selPtrAI_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

        case 2:
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
            break;
    }

    return info;
}

static void MenuTop_selPtrAI_Load(MenuTop_selPtrAI* taskData, Sprite* sprites, MenuTop_selPtrAI_Args* args) {
    SpriteAnimation anim = data_ov043_020c7fd0;

    anim.dataType = args->dataType;

    for (s32 i = 0; i < 5; i++) {
        taskData->visibleFlags[i] = 1;
    }

    taskData->visibleFlags[0] = 0;
    taskData->lastVisible     = 0;

    anim.unk_2A = 0x3C;
    anim.unk_04 = 0xDA;
    anim.unk_06 = func_ov043_02089374(args->selectedIndex);
    _Sprite_Load(&sprites[0], &anim);
    taskData->lastFrame = anim.unk_06;

    anim.unk_2A = 0x34;
    anim.unk_04 = 0xDA;
    anim.unk_06 = func_ov043_02089374(0);
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = 0x37;
    anim.unk_04 = 0xDA;
    anim.unk_06 = func_ov043_02089374(1);
    _Sprite_Load(&sprites[2], &anim);

    anim.unk_2A = 0x36;
    anim.unk_04 = 0xDA;
    anim.unk_06 = func_ov043_02089374(2);
    _Sprite_Load(&sprites[3], &anim);

    anim.unk_2A = 0x35;
    anim.unk_04 = 0xDA;
    anim.unk_06 = func_ov043_02089374(3);
    _Sprite_Load(&sprites[4], &anim);
}

static s32 MenuTop_selPtrAI_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_selPtrAI*      taskData = task->data;
    MenuTop_selPtrAI_Args* initArgs = args;

    taskData->owner         = initArgs->owner;
    taskData->selectedIndex = initArgs->selectedIndex;
    taskData->state         = 0;
    taskData->delay         = 0xA;
    MenuTop_selPtrAI_Load(taskData, taskData->sprites, initArgs);
    return 1;
}

// TODO: Properly type `ownerBytes` and remove casts in this function
static s32 MenuTop_selPtrAI_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_selPtrAI* taskData   = task->data;
    u16*              ownerBytes = taskData->owner;
    u16               state      = taskData->state;

    switch (state) {
        case 0:
            if (taskData->delay > 0) {
                taskData->delay--;
            } else {
                taskData->state = 1;
            }
            break;

        case 1: {
            TouchCoord coord;

            if (TouchInput_IsTouchActive() != FALSE) {
                TouchInput_GetCoord(&coord);
                taskData->selectedIndex = func_ov043_02084e20(coord.x, coord.y);

                if (taskData->selectedIndex == -1) {
                    taskData->visibleFlags[0] = 0;
                } else {
                    taskData->visibleFlags[0] = 1;
                    taskData->sprites[0].posY = func_ov043_02089374(taskData->selectedIndex);
                }
            } else {
                TouchInput_GetCoord(&coord);
                taskData->selectedIndex = func_ov043_02084e20(coord.x, coord.y);

                if (taskData->selectedIndex != -1) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
                    taskData->delay = 0x14;
                    taskData->state = 2;
                } else {
                    ownerBytes[0] &= ~8;
                    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~1;
                    *(u16*)(ownerBytes + 0x26) = 0;
                    return 0;
                }
            }
        } break;

        case 2:
            if (taskData->delay > 0) {
                taskData->delay--;
                break;
            }

            *(u16*)(ownerBytes + 10) = taskData->selectedIndex;
            ownerBytes[0] &= ~8;
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~1;
            *(u16*)(ownerBytes + 0x26) = 0;
            return 0;
    }

    if (taskData->lastFrame != taskData->sprites[0].posY || (taskData->lastVisible == 0 && taskData->visibleFlags[0] == 1)) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
    }

    taskData->lastFrame   = taskData->sprites[0].posY;
    taskData->lastVisible = taskData->visibleFlags[0];

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_selPtrAI_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_selPtrAI* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (taskData->visibleFlags[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_selPtrAI_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_selPtrAI* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }
    return 1;
}

s32 MenuTop_selPtrAI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_selPtrAI_Init,
        .update     = MenuTop_selPtrAI_Update,
        .render     = MenuTop_selPtrAI_Render,
        .cleanup    = MenuTop_selPtrAI_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_selPtrAI_CreateTask(TaskPool* pool, s32 dataType, void* ownerContext) {
    MenuTop_selPtrAI_Args args;

    args.dataType      = dataType;
    args.owner         = ownerContext;
    args.selectedIndex = *(u16*)((u8*)ownerContext + 0x14);

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_selPtrAI, NULL, 0, NULL, &args);
}
