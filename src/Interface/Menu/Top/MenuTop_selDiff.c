#include "Display.h"
#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Top.h"
#include "SndMgr.h"

extern s16 func_ov043_02084c9c(u16, u16);
extern s16 func_ov043_02084cec(s16, s16, u16);

typedef struct {
    /* 0x00 */ Sprite sprites[5];
    /* 0x140 */ s32   visibleFlags[5];
    /* 0x154 */ void* owner;
    /* 0x158 */ s16   selectedIndex;
    /* 0x15A */ s16   state;
    /* 0x15C */ s16   delay;
    /* 0x15E */ u16   unk_15E;
    /* 0x160 */ u8    unk_160[0xC];
    /* 0x16C */ s16   lastFrame;
    /* 0x16E */ u16   unk_16E;
    /* 0x170 */ s32   lastVisible;
} MenuTop_selDiff;

typedef struct {
    /* 0x00 */ s32   dataType;
    /* 0x04 */ void* owner;
    /* 0x08 */ u16   selectedIndex;
} MenuTop_selDiff_Args;

static SpriteFrameInfo* MenuTop_selDiff_GetFrameInfo(Sprite* sprite, s32 arg1, s32 arg2);
static s32              MenuTop_selDiff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_selDiff = {"Tsk_MenuTop_selDiff", MenuTop_selDiff_RunTask, sizeof(MenuTop_selDiff)};

static const SpriteAnimation data_ov043_020c7f80 = {
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
    .unk_08     = MenuTop_selDiff_GetFrameInfo,
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

static s16 func_ov043_02088e04(u16 arg0) {
    s16 data_ov043_020c7f5c[4] = {0x8E, 0x6E, 0x4E, 0x2E};

    return data_ov043_020c7f5c[arg0];
}

static SpriteFrameInfo* MenuTop_selDiff_GetFrameInfo(Sprite* sprite, s32 arg1, s32 arg2) {
    SpriteFrameInfo* info = NULL;

    switch (arg2) {
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
static void MenuTop_selDiff_Load(MenuTop_selDiff* taskData, Sprite* sprites, MenuTop_selDiff_Args* args) {
    SpriteAnimation anim = data_ov043_020c7f80;

    anim.dataType = (u16)args->dataType;
    anim.unk_04   = (u16)func_ov043_02088e04(args->selectedIndex);

    taskData->visibleFlags[0] = 0;
    taskData->lastVisible     = 0;
    _Sprite_Load(&sprites[0], &anim);

    taskData->lastFrame = (s16)func_ov043_02088e04(args->selectedIndex);

    for (u16 i = 0; i < 4; i++) {
        s16 frameIndex = func_ov043_02084c9c(i, *(u16*)((u8*)taskData->owner + 0x58));

        if (frameIndex == 0xFF) {
            anim.unk_2A = 1;
            anim.unk_04 = 98;
            anim.unk_06 = func_ov043_02088e04(i);
            _Sprite_Load(&sprites[i + 1], &anim);
            taskData->visibleFlags[i + 1] = 0;
        } else {
            anim.unk_2A = 0x2F - frameIndex;
            anim.unk_04 = 98;
            anim.unk_06 = func_ov043_02088e04(i);
            _Sprite_Load(&sprites[i + 1], &anim);
            taskData->visibleFlags[i + 1] = 1;
        }
    }
}

static s32 MenuTop_selDiff_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_selDiff*      taskData = task->data;
    MenuTop_selDiff_Args* initArgs = args;

    taskData->owner         = initArgs->owner;
    taskData->selectedIndex = initArgs->selectedIndex;
    taskData->state         = 0;
    taskData->delay         = 0xA;
    MenuTop_selDiff_Load(taskData, taskData->sprites, initArgs);
    return 1;
}

static s32 MenuTop_selDiff_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_selDiff* taskData   = task->data;
    u16*             ownerBytes = taskData->owner;
    u16              state      = taskData->state;

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
                taskData->selectedIndex = func_ov043_02084cec((s16)coord.x, (s16)coord.y, *(u16*)(ownerBytes + 0x2c));
                if (taskData->selectedIndex == -1) {
                    taskData->visibleFlags[0] = 0;
                } else {
                    taskData->visibleFlags[0] = 1;
                    taskData->sprites[0].posY = func_ov043_02088e04(taskData->selectedIndex);
                }
            } else {
                TouchInput_GetCoord(&coord);
                taskData->selectedIndex = func_ov043_02084cec(coord.x, coord.y, *(u16*)(ownerBytes + 0x2c));

                if (taskData->selectedIndex != -1) {
                    SndMgr_StartPlayingSE(0x11C);
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

            ownerBytes[0] &= ~8;
            if (taskData->selectedIndex != -1) {
                *(u16*)(ownerBytes + 0x9) = (u16)func_ov043_02084c9c((u16)taskData->selectedIndex, *(u16*)(ownerBytes + 0x2C));
            }
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~1;
            *(u16*)(ownerBytes + 0x26) = 0;
            return 0;
    }

    if (taskData->lastFrame != taskData->sprites[0].posY || (taskData->lastVisible == 0 && taskData->visibleFlags[0] == 1)) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
    }

    taskData->lastFrame   = (s16)taskData->sprites[0].posY;
    taskData->lastVisible = taskData->visibleFlags[0];

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_selDiff_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_selDiff* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (taskData->visibleFlags[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_selDiff_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_selDiff* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_selDiff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_selDiff_Init,
        .update     = MenuTop_selDiff_Update,
        .render     = MenuTop_selDiff_Render,
        .cleanup    = MenuTop_selDiff_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_selDiff_CreateTask(TaskPool* pool, s32 dataType, void* ownerContext) {
    MenuTop_selDiff_Args args;

    args.dataType      = dataType;
    args.owner         = ownerContext;
    args.selectedIndex = *(u16*)((u8*)ownerContext + 0x12);

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_selDiff, NULL, 0, NULL, &args);
}
