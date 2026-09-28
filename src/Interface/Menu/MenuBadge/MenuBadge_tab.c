#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/MenuBadge.h"
#include "SndMgr.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ s32              unk_C0;
    /* 0xC4 */ MenuBadgeObject* menuBadge;
} MenuBadge_tab; // Size: 0xC8

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_tab_Args;

static SpriteFrameInfo* MenuBadge_tab_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuBadge_tab_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_tab = {"Tsk_MenuBadge_tab", MenuBadge_tab_RunTask, sizeof(MenuBadge_tab)};

static const SpriteAnimation MenuBadge_tab_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .posX              = -0xD,
    .posY              = 0xC,
    .frameInfoCallback = MenuBadge_tab_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuBadge_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 0xD,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuBadge_tab_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuBadge_tab_Load(Sprite* sprites, MenuBadge_tab_Args* args) {
    SpriteAnimation anim = MenuBadge_tab_Anim;

    anim.dataType = args->dataType;

    anim.animIndex = 0x22;
    anim.posX      = 0x5F;
    anim.posY      = 6;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 0x25;
    anim.posX      = 0x80;
    anim.posY      = 6;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 0x27;
    anim.posX      = 0xA1;
    anim.posY      = 6;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 MenuBadge_tab_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tab*      tab      = task->data;
    MenuBadge_tab_Args* initArgs = args;

    MenuBadge_tab_Load(tab->sprites, initArgs);
    tab->menuBadge = initArgs->menuBadge;
    return 1;
}

static s32 MenuBadge_tab_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tab*   tab       = task->data;
    MenuBadgeObject* menuBadge = tab->menuBadge;

    if (menuBadge->windowMessage != 0) {
        return 1;
    }

    if (TouchInput_WasTouchPressed() != 0) {
        TouchCoord coord;

        TouchInput_GetCoord(&coord);
        u32 newTab = MenuBadge_GetTabAtPoint(coord.x, coord.y);
        if (newTab != -1 && newTab != menuBadge->infoTab) {
            SndMgr_StartPlayingSE(0x11A);

            MenuBadge_SetSpriteFrameFromPack(&tab->sprites[menuBadge->infoTab], menuBadge->infoTab * 2 + 0x23, 3, 2);
            MenuBadge_SetSpriteFrameFromPack(&tab->sprites[newTab], newTab * 2 + 0x22, 3, 2);
            menuBadge->infoTab = newTab;
            menuBadge->flags |= MENUBADGE_FLAG_REDRAW_INFO;
            menuBadge->flags |= MENUBADGE_FLAG_INFO_TAB_CHANGED;

            s32 pageArg;
            if (menuBadge->infoTab == 0) {
                pageArg = 0;
            } else if (menuBadge->infoTab == 1) {
                pageArg = 2;
            } else {
                pageArg = 3;
            }
            MenuBadge_ReloadBgScreen(&menuBadge->resources[3], menuBadge->resources[3].data, DISPLAY_SUB, 3, pageArg + 2);
        }
    }

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&tab->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_tab_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tab* tab = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&tab->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_tab_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_tab* tab = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&tab->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_tab_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_tab_Init,
        .update     = MenuBadge_tab_Update,
        .render     = MenuBadge_tab_Render,
        .cleanup    = MenuBadge_tab_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_tab_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge) {
    MenuBadge_tab_Args args;

    args.dataType  = dataType;
    args.menuBadge = menuBadge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_tab, NULL, 0, NULL, &args);
}
