#include "Display.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Top.h"
#include "SndMgr.h"

typedef struct {
    /* 0x00 */ Sprite          sprites[5];
    /* 0x140 */ BOOL           visible[5];
    /* 0x154 */ MenuTopObject* topMenu;
    /* 0x158 */ s16            selectedIndex;
    /* 0x15A */ u16            state;
    /* 0x15C */ s16            delay;
    /* 0x15E */ s16            lastCursorY;
    /* 0x160 */ BOOL           lastVisible;
} MenuTop_selPtrAI; // Size: 0x164

typedef struct {
    /* 0x00 */ s32            dataType;
    /* 0x04 */ MenuTopObject* topMenu;
    /* 0x08 */ s16            selectedIndex;
} MenuTop_selPtrAI_Args;

static SpriteFrameInfo* MenuTop_selPtrAI_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_selPtrAI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_selPtrAI = {"Tsk_MenuTop_selPtrAI", MenuTop_selPtrAI_RunTask, sizeof(MenuTop_selPtrAI)};

static const SpriteAnimation MenuTop_selPtrAI_Anim = {
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
    .frameInfoCallback = MenuTop_selPtrAI_GetFrameInfo,
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

static s16 MenuTop_selPtrAI_GetRowY(u16 row) {
    s16 rowY[4] = {
        46,
        78,
        110,
        142,
    };
    return rowY[row];
}

static SpriteFrameInfo* MenuTop_selPtrAI_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_selPtrAI_Load(MenuTop_selPtrAI* taskData, Sprite* sprites, MenuTop_selPtrAI_Args* args) {
    SpriteAnimation anim = MenuTop_selPtrAI_Anim;

    anim.dataType = args->dataType;

    for (s32 i = 0; i < 5; i++) {
        taskData->visible[i] = TRUE;
    }

    taskData->visible[0]  = FALSE;
    taskData->lastVisible = FALSE;

    anim.animIndex = 0x3C;
    anim.posX      = 0xDA;
    anim.posY      = MenuTop_selPtrAI_GetRowY(args->selectedIndex);
    _Sprite_Load(&sprites[0], &anim);
    taskData->lastCursorY = anim.posY;

    anim.animIndex = 0x34;
    anim.posX      = 0xDA;
    anim.posY      = MenuTop_selPtrAI_GetRowY(0);
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 0x37;
    anim.posX      = 0xDA;
    anim.posY      = MenuTop_selPtrAI_GetRowY(1);
    _Sprite_Load(&sprites[2], &anim);

    anim.animIndex = 0x36;
    anim.posX      = 0xDA;
    anim.posY      = MenuTop_selPtrAI_GetRowY(2);
    _Sprite_Load(&sprites[3], &anim);

    anim.animIndex = 0x35;
    anim.posX      = 0xDA;
    anim.posY      = MenuTop_selPtrAI_GetRowY(3);
    _Sprite_Load(&sprites[4], &anim);
}

static s32 MenuTop_selPtrAI_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_selPtrAI*      taskData = task->data;
    MenuTop_selPtrAI_Args* initArgs = args;

    taskData->topMenu       = initArgs->topMenu;
    taskData->selectedIndex = initArgs->selectedIndex;
    taskData->state         = 0;
    taskData->delay         = 0xA;
    MenuTop_selPtrAI_Load(taskData, taskData->sprites, initArgs);
    return 1;
}

static s32 MenuTop_selPtrAI_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_selPtrAI* taskData = task->data;
    MenuTopObject*    topMenu  = taskData->topMenu;
    u16               state    = taskData->state;

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
                taskData->selectedIndex = MenuTop_GetPartnerAIRowAtPoint(coord.x, coord.y);

                if (taskData->selectedIndex == -1) {
                    taskData->visible[0] = FALSE;
                } else {
                    taskData->visible[0]      = TRUE;
                    taskData->sprites[0].posY = MenuTop_selPtrAI_GetRowY(taskData->selectedIndex);
                }
            } else {
                TouchInput_GetCoord(&coord);
                taskData->selectedIndex = MenuTop_GetPartnerAIRowAtPoint(coord.x, coord.y);

                if (taskData->selectedIndex != -1) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
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

            topMenu->partnerAI = taskData->selectedIndex;
            topMenu->flags &= ~8;
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~1;
            topMenu->popupOpen = 0;
            return 0;
    }

    if (taskData->lastCursorY != taskData->sprites[0].posY || (taskData->lastVisible == 0 && taskData->visible[0] == 1)) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
    }

    taskData->lastCursorY = taskData->sprites[0].posY;
    taskData->lastVisible = taskData->visible[0];

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_selPtrAI_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_selPtrAI* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (taskData->visible[i] != 0) {
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

s32 MenuTop_selPtrAI_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_selPtrAI_Args args;

    args.dataType      = dataType;
    args.topMenu       = topMenu;
    args.selectedIndex = topMenu->partnerAI;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_selPtrAI, NULL, 0, NULL, &args);
}
