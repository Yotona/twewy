#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/MenuBadge.h"
#include "SndMgr.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
} MenuBadge_gbgBox; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_gbgBox_Args;

static SpriteFrameInfo* MenuBadge_gbgBox_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_gbgBox_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_gbgBox = {"Tsk_MenuBadge_gbgBox", MenuBadge_gbgBox_RunTask, sizeof(MenuBadge_gbgBox)};

static const SpriteAnimation data_ov043_020c89b0 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0xF1,
    .unk_06     = 0x1D,
    .unk_08     = MenuBadge_gbgBox_GetFrameInfo,
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

static SpriteFrameInfo* MenuBadge_gbgBox_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuBadge_gbgBox_Load(Sprite* sprite, MenuBadge_gbgBox_Args* args) {
    SpriteAnimation anim = data_ov043_020c89b0;

    anim.dataType = args->dataType;
    anim.unk_2A   = 33;

    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_gbgBox_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_gbgBox*      gbgBox     = task->data;
    MenuBadge_gbgBox_Args* gbgBoxArgs = args;

    MenuBadge_gbgBox_Load(&gbgBox->sprite, gbgBoxArgs);
    gbgBox->menuBadge = gbgBoxArgs->menuBadge;

    if (gbgBox->menuBadge->unk_AEE8 & 0x20) {
        gbgBox->visible = TRUE;
    } else {
        gbgBox->visible = FALSE;
    }
    return 1;
}

static s32 MenuBadge_gbgBox_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_gbgBox* gbgBox    = task->data;
    MenuBadgeObject*  menuBadge = gbgBox->menuBadge;
    TouchCoord        coord;

    if (menuBadge->unk_AF20 != 0) {
        return 1;
    }

    if (TouchInput_WasTouchPressed() != 0) {
        TouchInput_GetCoord(&coord);
        if (func_ov043_020918c8(coord.x, coord.y) == 1) {
            SndMgr_StartPlayingSE(0x11A);
            if (menuBadge->unk_AEE8 & 0x20) {
                gbgBox->visible = FALSE;
                menuBadge->unk_AEE8 &= ~0x20;
            } else {
                gbgBox->visible = TRUE;
                menuBadge->unk_AEE8 |= 0x20;
            }
        }
    }

    Sprite_Update(&gbgBox->sprite);
    return 1;
}

static s32 MenuBadge_gbgBox_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_gbgBox* gbgBox = task->data;

    if (gbgBox->visible != 0) {
        Sprite_RenderFrame(&gbgBox->sprite);
    }
    return 1;
}

static s32 MenuBadge_gbgBox_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_gbgBox* gbgBox = task->data;

    Sprite_Release(&gbgBox->sprite);
    return 1;
}

static s32 MenuBadge_gbgBox_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_gbgBox_Init,
        .update     = MenuBadge_gbgBox_Update,
        .render     = MenuBadge_gbgBox_Render,
        .cleanup    = MenuBadge_gbgBox_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_gbgBox_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge) {
    MenuBadge_gbgBox_Args args;

    args.dataType  = dataType;
    args.menuBadge = menuBadge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_gbgBox, NULL, 0, NULL, &args);
}
