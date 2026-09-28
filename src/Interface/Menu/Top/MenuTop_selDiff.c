#include "Display.h"
#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Top.h"
#include "SndMgr.h"

typedef struct {
    /* 0x00 */ Sprite          sprites[5];
    /* 0x140 */ BOOL           visible[5];
    /* 0x154 */ MenuTopObject* topMenu;
    /* 0x158 */ s16            selectedIndex;
    /* 0x15A */ s16            state;
    /* 0x15C */ s16            delay;
    /* 0x15E */ u16            unk_15E;
    /* 0x160 */ u8             unk_160[0xC];
    /* 0x16C */ s16            lastCursorY;
    /* 0x16E */ u16            unk_16E;
    /* 0x170 */ BOOL           lastVisible;
} MenuTop_selDiff;

typedef struct {
    /* 0x00 */ s32            dataType;
    /* 0x04 */ MenuTopObject* topMenu;
    /* 0x08 */ u16            selectedIndex;
} MenuTop_selDiff_Args;

static SpriteFrameInfo* MenuTop_selDiff_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_selDiff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_selDiff = {"Tsk_MenuTop_selDiff", MenuTop_selDiff_RunTask, sizeof(MenuTop_selDiff)};

static const SpriteAnimation MenuTop_selDiff_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuTop_selDiff_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[3],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 4,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static s16 MenuTop_selDiff_GetRowY(u16 row) {
    s16 rowY[4] = {0x8E, 0x6E, 0x4E, 0x2E};

    return rowY[row];
}

static SpriteFrameInfo* MenuTop_selDiff_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Nonmatching
static void MenuTop_selDiff_Load(MenuTop_selDiff* taskData, Sprite* sprites, MenuTop_selDiff_Args* args) {
    SpriteAnimation anim = MenuTop_selDiff_Anim;

    anim.dataType = args->dataType;
    anim.posX     = MenuTop_selDiff_GetRowY(args->selectedIndex);

    taskData->visible[0]  = FALSE;
    taskData->lastVisible = FALSE;
    _Sprite_Load(&sprites[0], &anim);

    taskData->lastCursorY = MenuTop_selDiff_GetRowY(args->selectedIndex);

    for (u16 i = 0; i < 4; i++) {
        u16 frameIndex = MenuTop_GetDifficultyForRow(i, taskData->topMenu->unlockedDifficulty);

        if (frameIndex == 0xFF) {
            anim.animIndex = 1;
            anim.posX      = 98;
            anim.posY      = MenuTop_selDiff_GetRowY(i);
            _Sprite_Load(&sprites[i + 1], &anim);
            taskData->visible[i + 1] = FALSE;
        } else {
            anim.animIndex = 0x2F - frameIndex;
            anim.posX      = 98;
            anim.posY      = MenuTop_selDiff_GetRowY(i);
            _Sprite_Load(&sprites[i + 1], &anim);
            taskData->visible[i + 1] = TRUE;
        }
    }
}

static s32 MenuTop_selDiff_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_selDiff*      taskData = task->data;
    MenuTop_selDiff_Args* initArgs = args;

    taskData->topMenu       = initArgs->topMenu;
    taskData->selectedIndex = initArgs->selectedIndex;
    taskData->state         = 0;
    taskData->delay         = 0xA;
    MenuTop_selDiff_Load(taskData, taskData->sprites, initArgs);
    return 1;
}

static s32 MenuTop_selDiff_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_selDiff* taskData = task->data;
    MenuTopObject*   topMenu  = taskData->topMenu;
    u16              state    = taskData->state;

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
                taskData->selectedIndex =
                    MenuTop_GetDifficultyRowAtPoint((s16)coord.x, (s16)coord.y, topMenu->unlockedDifficulty);
                if (taskData->selectedIndex == -1) {
                    taskData->visible[0] = FALSE;
                } else {
                    taskData->visible[0]      = TRUE;
                    taskData->sprites[0].posY = MenuTop_selDiff_GetRowY(taskData->selectedIndex);
                }
            } else {
                TouchInput_GetCoord(&coord);
                taskData->selectedIndex = MenuTop_GetDifficultyRowAtPoint(coord.x, coord.y, topMenu->unlockedDifficulty);

                if (taskData->selectedIndex != -1) {
                    SndMgr_StartPlayingSE(0x11C);
                    taskData->delay = 0x14;
                    taskData->state = 2;
                } else {
                    topMenu->flags &= ~8;
                    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~1;
                    topMenu->popupOpen = 0;
                    return 0;
                }
            }
        } break;

        case 2:
            if (taskData->delay > 0) {
                taskData->delay--;
                break;
            }

            topMenu->flags &= ~8;
            if (taskData->selectedIndex != -1) {
                topMenu->difficulty =
                    (u16)MenuTop_GetDifficultyForRow((u16)taskData->selectedIndex, topMenu->unlockedDifficulty);
            }
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~1;
            topMenu->popupOpen = 0;
            return 0;
    }

    if (taskData->lastCursorY != taskData->sprites[0].posY || (taskData->lastVisible == 0 && taskData->visible[0] == 1)) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
    }

    taskData->lastCursorY = (s16)taskData->sprites[0].posY;
    taskData->lastVisible = taskData->visible[0];

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_selDiff_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_selDiff* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (taskData->visible[i] != 0) {
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

s32 MenuTop_selDiff_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_selDiff_Args args;

    args.dataType      = dataType;
    args.topMenu       = topMenu;
    args.selectedIndex = topMenu->difficulty;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_selDiff, NULL, 0, NULL, &args);
}
