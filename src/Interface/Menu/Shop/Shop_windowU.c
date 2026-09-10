#include "Interface/Menu/Shop.h"
#include "Util/SysFont.h"
#include "common_data.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[2];
    /* 0x080 */ s32              unk_080;
    /* 0x084 */ ShopObject*      shop;
    /* 0x088 */ SysFont          font;
    /* 0x104 */ BOOL             unk_104;
    /* 0x108 */ u16              msgIndex;
    /* 0x10A */ char             unk_10A[2];
    /* 0x10C */ Data*            unk_10C;
    /* 0x110 */ void*            unk_110;
    /* 0x114 */ PaletteResource* unk_114;
} Shop_windowU; // Size: 0x118

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
    /* 0x8 */ u16         msgIndex;
} Shop_windowU_Args;

static SpriteFrameInfo* Shop_windowU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              Shop_windowU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_windowU = {"Tsk_Shop_windowU", Shop_windowU_RunTask, sizeof(Shop_windowU)};

static const SpriteAnimation Shop_windowU_Anim = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 1,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 80,
    .unk_06     = 92,
    .unk_08     = Shop_windowU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1e0,
    .unk_18     = 2,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 3,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* Shop_windowU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* frameInfo = &data_0206b408;
            frameInfo->unk_04          = 0;
            frameInfo->unk_08          = 0;
            frameInfo->unk_0C          = 0;
            frameInfo->unk_10          = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                frameInfo->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                frameInfo->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            info = frameInfo;
        } break;
    }

    return info;
}

static void Shop_windowU_InitFont(Shop_windowU* window) {
    SysFont_InitWithFont(&window->font, 3, TRUE);
    SysFont_SetSpacing(&window->font, TRUE, 0);
}

// Centres the message on the window sprite.
static void Shop_windowU_CenterMsg(Shop_windowU* window) {
    s32 minX;
    s32 minY;

    SysFont_SetMsg(&window->font, window->msgIndex);
    SysFont_GetCellBoundsMin(window->sprites, &minX, &minY);

    s32 width  = SysFont_MeasureCurrentWidth(&window->font);
    s32 height = SysFont_MeasureCurrentHeight(&window->font);

    SysFont_SetHAlign(&window->font, 0, width);
    SysFont_SetPos(&window->font, (u16)((-width / 2) - minX), (u16)((-height / 2) - minY));
}

static void Shop_windowU_Load(Shop_windowU* window, Sprite* sprite, Shop_windowU_Args* windowArgs) {
    SpriteAnimation anim = Shop_windowU_Anim;

    anim.dataType  = windowArgs->dataType;
    anim.unk_2A    = 4;
    anim.packIndex = 10;
    anim.unk_20    = 1;
    anim.unk_1C    = 2;
    anim.unk_26    = 3;
    anim.unk_28    = 4;
    _Sprite_Load(&sprite[0], &anim);

    anim.unk_04 += 3;
    anim.unk_06 += 3;
    _Sprite_Load(&sprite[1], &anim);

    Sprite_UpdateAndCheck(sprite);

    window->unk_10C = DatMgr_LoadPackEntry(windowArgs->dataType, NULL, 0, &data_ov043_020ca1e0, 23, 0);
    window->unk_110 = Data_GetPackEntryData(window->unk_10C, 1);
    window->unk_114 = PaletteMgr_AcquireContiguous(g_PaletteManagers[DISPLAY_SUB], window->unk_110, 5, 1);
}

static s32 Shop_windowU_Init(TaskPool* pool, Task* task, void* args) {
    Shop_windowU*      window     = task->data;
    Shop_windowU_Args* windowArgs = args;

    window->shop     = windowArgs->shop;
    window->msgIndex = windowArgs->msgIndex;
    window->unk_080  = 0;
    window->unk_104  = FALSE;

    Shop_windowU_Load(window, window->sprites, windowArgs);
    Shop_windowU_InitFont(window);
    Shop_windowU_CenterMsg(window);
    return 1;
}

static s32 Shop_windowU_Update(TaskPool* pool, Task* task, void* args) {
    Shop_windowU* window = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&window->sprites[i]);
    }
    return 1;
}

static s32 Shop_windowU_Render(TaskPool* pool, Task* task, void* args) {
    Shop_windowU* window = task->data;

    Sprite_RenderFrame(&window->sprites[0]);
    Sprite_RenderAltPalette(&window->sprites[1], window->unk_114, window->unk_110, 0);
    if (window->unk_104 == FALSE) {
        SysFont_DrawCurrentToSprite(&window->font, window->sprites, FALSE);
        window->unk_104 = TRUE;
    }
    return 1;
}

static s32 Shop_windowU_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_windowU* window = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&window->sprites[i]);
    }
    if (window->unk_114 != NULL) {
        PaletteMgr_ReleaseResource(g_PaletteManagers[1], window->unk_114);
    }
    DatMgr_ReleaseData(window->unk_10C);
    SysFont_Destroy(&window->font);
    return 1;
}

static s32 Shop_windowU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_windowU_Init,
        .update     = Shop_windowU_Update,
        .render     = Shop_windowU_Render,
        .cleanup    = Shop_windowU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_windowU_CreateTask(TaskPool* pool, s32 dataType, s16 msgIndex, ShopObject* shop) {
    Shop_windowU_Args args;
    args.dataType = dataType;
    args.msgIndex = msgIndex;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_windowU, NULL, 0, NULL, &args);
}
