#include "Interface/Menu/Shop.h"

#include "Engine/EasyTask.h"

typedef struct {
    /* 0x00 */ Sprite      unk_00[3];
    /* 0xC0 */ char        unk_C0[0xC4 - 0xC0];
    /* 0xC4 */ ShopObject* unk_C4;
    /* 0xC8 */ u16         unk_C8;
    /* 0xCA */ u16         unk_CA;
} Shop_sbar2; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
} Shop_sbar2_Args;

static SpriteFrameInfo* Shop_sbar2_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Shop_sbar2_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle Tsk_Shop_sbar2 = {"Tsk_Shop_sbar2", Shop_sbar2_RunTask, sizeof(Shop_sbar2)};

static const SpriteAnimation Shop_sbar2_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Shop_sbar2_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020ca1b0,
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 3,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* Shop_sbar2_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Shop_sbar2_Load(Sprite* arg0, Shop_sbar2_Args* arg1) {
    SpriteAnimation anim = Shop_sbar2_Anim;

    anim.dataType = arg1->dataType;

    anim.unk_2A = 0x2E;
    anim.unk_04 = 0xD1;
    anim.unk_06 = 0x5F;
    _Sprite_Load(&arg0[0], &anim);

    anim.unk_2A = 0x2B;
    anim.unk_04 = 0xD1;
    anim.unk_06 = 0x1D;
    _Sprite_Load(&arg0[1], &anim);

    anim.unk_2A = 0x2C;
    anim.unk_04 = 0xD1;
    anim.unk_06 = 0x66;
    _Sprite_Load(&arg0[2], &anim);
}

static s32 Shop_sbar2_Init(TaskPool* arg0, Task* task, void* args) {
    Shop_sbar2_Args* sbarArgs = args;
    ShopObject*      shop     = sbarArgs->shop;

    Shop_sbar2* sbar = task->data;

    sbar->unk_C4 = shop;
    sbar->unk_C8 = shop->unk_850;
    Shop_sbar2_Load(sbar->unk_00, sbarArgs);
    return 1;
}

static s32 Shop_sbar2_Update(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_sbar2* sbar = arg1->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&sbar->unk_00[i]);
    }
    return 1;
}

static s32 Shop_sbar2_Render(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_sbar2* sbar = arg1->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&sbar->unk_00[i]);
    }
    return 1;
}

static s32 Shop_sbar2_Destroy(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_sbar2* sbar = arg1->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&sbar->unk_00[i]);
    }
    return 1;
}

static s32 Shop_sbar2_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    TaskStages stages = {
        .initialize = Shop_sbar2_Init,
        .update     = Shop_sbar2_Update,
        .render     = Shop_sbar2_Render,
        .cleanup    = Shop_sbar2_Destroy,
    };
    return stages.iter[arg3](arg0, arg1, arg2);
}

s32 Shop_sbar2_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_sbar2_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_sbar2, NULL, 0, NULL, &args);
}
