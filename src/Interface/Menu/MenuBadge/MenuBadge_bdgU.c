#include "Interface/Menu/MenuBadge.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x00 */ s32              dataType;
    /* 0x04 */ Sprite           sprite;
    /* 0x44 */ s32              visible;
    /* 0x48 */ MenuBadgeObject* menuBadge;
} MenuBadge_bdgU; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ u16              badgeId;
    /* 0xA */ u8               vramPage;
} MenuBadge_bdgU_Args;

static SpriteFrameInfo* MenuBadge_bdgU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_bdgU = {"Tsk_MenuBadge_bdgU", MenuBadge_bdgU_RunTask, sizeof(MenuBadge_bdgU)};

static const SpriteAnimation MenuBadge_bdgU_Anim = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_bdgU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c831c,
    .unk_18     = 2,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static void MenuBadge_bdgU_LoadPackedData(MenuBadge_bdgU* bdgU, Sprite* sprite, MenuBadge_bdgU_Args* args, s32 packIndex) {
    s32 dataType = args->dataType;

    Data* data;
    if (BinMgr_FindById((s32)&data_ov043_020c8314) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, &data_ov043_020c8314, packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, &data_ov043_020c8314, packIndex, 0);
    }

    u8*   src        = (u8*)Data_GetPackEntryData(data, 1) + 4;
    void* paletteSrc = Data_GetPackEntryData(data, 4);
    u8*   dest       = (u8*)sprite->unk34 + 4;
    MI_CpuCopyU8(paletteSrc, sprite->unk3C, 0x20);

    s32 outer = 0;
    while (outer < 4) {
        s32 copyCount;
        s32 x;
        s32 y = 0;
        while (y < 4) {
            x = 0;
            while (x < 8) {
                copyCount = 0;
                while (copyCount < 4) {
                    *dest = *(src + outer * 0x80 + y * 4 + x * 0x10 + copyCount);
                    dest++;
                    copyCount++;
                }
                x++;
            }
            y++;
        }
        outer++;
    }

    DatMgr_ReleaseData(data);
}

static SpriteFrameInfo* MenuBadge_bdgU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            info->unk_00 = 1;
            return info;
        } break;

        case 2: {
            info         = &data_0206b408;
            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            return info;
        } break;
    }

    return NULL;
}

static void MenuBadge_bdgU_Load(MenuBadge_bdgU* bdgU, Sprite* sprite, MenuBadge_bdgU_Args* args) {
    MenuBadgeObject* menuBadge = bdgU->menuBadge;
    SpriteAnimation  anim      = MenuBadge_bdgU_Anim;

    anim.dataType  = args->dataType;
    anim.unk_04    = 0xEA;
    anim.unk_06    = 0x1C;
    anim.packIndex = args->vramPage + 1;

    _Sprite_Load(sprite, &anim);

    if (args->badgeId == 0xFFFF) {
        bdgU->visible = FALSE;
        return;
    }

    MenuBadge_bdgU_LoadPackedData(bdgU, sprite, args, (u16)(menuBadge->unk_ADF2 + 1));
    bdgU->visible = TRUE;
}

static s32 MenuBadge_bdgU_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgU*      bdgU     = task->data;
    MenuBadge_bdgU_Args* bdgUArgs = args;

    bdgU->dataType  = bdgUArgs->dataType;
    bdgU->menuBadge = bdgUArgs->menuBadge;
    MenuBadge_bdgU_Load(bdgU, &bdgU->sprite, bdgUArgs);
    return 1;
}

static s32 MenuBadge_bdgU_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgU* bdgU = task->data;

    Sprite_Update(&bdgU->sprite);
    return 1;
}

static s32 MenuBadge_bdgU_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgU* bdgU = task->data;

    if (bdgU->visible) {
        Sprite_RenderFrame(&bdgU->sprite);
    }
    return 1;
}

static s32 MenuBadge_bdgU_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_bdgU* bdgU = task->data;

    Sprite_Release(&bdgU->sprite);
    return 1;
}

static s32 MenuBadge_bdgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_bdgU_Init,
        .update     = MenuBadge_bdgU_Update,
        .render     = MenuBadge_bdgU_Render,
        .cleanup    = MenuBadge_bdgU_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_bdgU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_bdgU_Args args;

    args.dataType   = dataType;
    args.menuBadge  = badge;
    args.badgeId    = badge->unk_ADD8;
    args.vramPage   = badge->unk_AF03;
    badge->unk_AF03 = 1 - badge->unk_AF03;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_bdgU, NULL, 0, NULL, &args);
}
