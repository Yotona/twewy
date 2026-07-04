#include "Interface/Menu/Shop.h"
#include "common_data.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[2];
    /* 0x080 */ s32              unk_080;
    /* 0x084 */ s32              unk_084;
    /* 0x088 */ UnkOv31Struct    unk_088;
    /* 0x104 */ BOOL             unk_104;
    /* 0x108 */ u16              unk_108;
    /* 0x10A */ char             unk_10A[2];
    /* 0x10C */ Data*            unk_10C;
    /* 0x110 */ void*            unk_110;
    /* 0x114 */ PaletteResource* unk_114;
} Shop_windowU; // Size: 0x118

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s32 unk_4;
    /* 0x8 */ u16 unk_8;
} Shop_windowU_Args;

static SpriteFrameInfo* Shop_windowU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
s32                     Shop_windowU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_windowU = {"Tsk_Shop_windowU", Shop_windowU_RunTask, sizeof(Shop_windowU)};

static const SpriteAnimation data_ov043_020caca8 = {
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

static SpriteFrameInfo* Shop_windowU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;

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

static void func_ov043_020bc3ac(Shop_windowU* window) {
    func_ov031_0210aaac(&window->unk_088, 3, 1);
    func_ov031_0210ab54(&window->unk_088, 1, 0);
}

static void func_ov043_020bc3d8(Shop_windowU* window) {
    s32 sp4;
    s32 sp0;

    func_ov031_0210b630(&window->unk_088, window->unk_108);
    func_ov031_0210a220(window, &sp4, &sp0);

    s32 offsetX = func_ov031_0210c338(&window->unk_088);
    s32 offsetY = func_ov031_0210c5b4(&window->unk_088);

    func_ov031_0210ab3c(&window->unk_088, 0, offsetX);

    s32 negOffsetX = -offsetX;
    s32 negOffsetY = -offsetY;
    func_ov031_0210ab28(&window->unk_088, (u16)((negOffsetX / 2) - sp4), (u16)((negOffsetY / 2) - sp0));
}

// Nonmatching: regswap
static void Shop_windowU_Load(Shop_windowU* window, Sprite* sprite, Shop_windowU_Args* windowArgs) {
    SpriteAnimation anim = data_ov043_020caca8;

    anim.dataType  = windowArgs->dataType;
    anim.packIndex = 10;
    anim.unk_20    = 1;
    anim.unk_26    = 3;
    anim.unk_1C    = 2;
    anim.unk_2A    = 4;
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

    window->unk_084 = windowArgs->unk_4;
    window->unk_108 = windowArgs->unk_8;
    window->unk_080 = 0;
    window->unk_104 = FALSE;

    Shop_windowU_Load(window, window->sprites, windowArgs);
    func_ov043_020bc3ac(window);
    func_ov043_020bc3d8(window);
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
        func_ov031_0210bed8(&window->unk_088, window, 0);
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
    func_ov031_0210aabc(&window->unk_088);
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

s32 Shop_windowU_CreateTask(TaskPool* arg0, s32 dataType, s16 arg2, s32 arg3) {
    Shop_windowU_Args args;
    args.dataType = dataType;
    args.unk_8    = arg2;
    args.unk_4    = arg3;
    return EasyTask_CreateTask(arg0, &Tsk_Shop_windowU, NULL, 0, NULL, &args);
}
