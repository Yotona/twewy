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

static SpriteFrameInfo* MenuBadge_tab_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_tab_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_tab = {"Tsk_MenuBadge_tab", MenuBadge_tab_RunTask, sizeof(MenuBadge_tab)};

static const SpriteAnimation MenuBadge_tab_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = -0xD,
    .unk_06     = 0xC,
    .unk_08     = MenuBadge_tab_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_tab_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

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

static void MenuBadge_tab_Load(Sprite* sprites, MenuBadge_tab_Args* args) {
    SpriteAnimation anim = MenuBadge_tab_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 0x22;
    anim.unk_04 = 0x5F;
    anim.unk_06 = 6;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x25;
    anim.unk_04 = 0x80;
    anim.unk_06 = 6;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = 0x27;
    anim.unk_04 = 0xA1;
    anim.unk_06 = 6;
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

    if (menuBadge->unk_AF20 != 0) {
        return 1;
    }

    if (TouchInput_WasTouchPressed() != 0) {
        TouchCoord coord;

        TouchInput_GetCoord(&coord);
        u32 newTab = func_ov043_02091928(coord.x, coord.y);
        if (newTab != -1 && newTab != menuBadge->unk_AEFE) {
            SndMgr_StartPlayingSE(0x11A);

            func_ov043_0208fa60(&tab->sprites[menuBadge->unk_AEFE], menuBadge->unk_AEFE * 2 + 0x23, 3, 2);
            func_ov043_0208fa60(&tab->sprites[newTab], newTab * 2 + 0x22, 3, 2);
            menuBadge->unk_AEFE = newTab;
            menuBadge->unk_AEE8 |= 0x80;
            menuBadge->unk_AEE8 |= 0x40;

            s32 pageArg;
            if (menuBadge->unk_AEFE == 0) {
                pageArg = 0;
            } else if (menuBadge->unk_AEFE == 1) {
                pageArg = 2;
            } else {
                pageArg = 3;
            }
            func_ov043_020921cc(&menuBadge->unk_B1E8, menuBadge->unk_B1E8, 1, 3, pageArg + 2);
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
