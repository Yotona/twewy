#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Shop.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

extern s32 func_ov043_020b18e4(s16, s16);

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ BOOL        shouldRender;
    /* 0x44 */ ShopObject* shop;
} Shop_check; // Size: 0x48

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
} Shop_check_Args;

static SpriteFrameInfo* func_ov043_020b4838(Sprite* sprite, s32 arg, s32 mode);
s32                     func_ov043_020b4a7c(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle data_ov043_020ca558 = {"Tsk_Shop_check", func_ov043_020b4a7c, 0x48};

static const SpriteAnimation data_ov043_020ca574 = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .unk_04            = 0xF5,
    .unk_06            = 0x1B,
    .frameInfoCallback = func_ov043_020b4838,
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

static SpriteFrameInfo* func_ov043_020b4838(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Nonmatching: Instruction reordering
void func_ov043_020b48d4(Shop_check* check, Sprite* sprite, Shop_check_Args* args) {
    ShopObject*     shop = check->shop;
    SpriteAnimation anim = data_ov043_020ca574;

    anim.dataType       = args->dataType;
    check->shouldRender = (shop->unk_834 & 4) ? TRUE : FALSE;
    anim.unk_2A         = 47;
    _Sprite_Load(sprite, &anim);
}

s32 func_ov043_020b4964(TaskPool* pool, Task* task, void* args) {
    Shop_check*      check     = task->data;
    Shop_check_Args* checkArgs = args;

    check->shop = checkArgs->shop;
    func_ov043_020b48d4(check, &check->sprite, checkArgs);
    return 1;
}

s32 func_ov043_020b4984(TaskPool* pool, Task* task, void* args) {
    Shop_check* check = task->data;
    ShopObject* shop  = check->shop;

    if (shop->unk_836 != 0) {
        Sprite_Update(&check->sprite);
        return 1;
    }

    if (TouchInput_WasTouchPressed()) {
        TouchCoord coord;

        TouchInput_GetCoord(&coord);
        if (func_ov043_020b18e4(coord.x, coord.y) == 1) {
            SndMgr_StartPlayingSE(0x11A);
            if (shop->unk_834 & 4) {
                check->shouldRender = FALSE;
                shop->unk_834 &= ~4;
            } else {
                check->shouldRender = TRUE;
                shop->unk_834 |= 4;
            }
        }
    }

    Sprite_Update(&check->sprite);
    return 1;
}

s32 func_ov043_020b4a48(TaskPool* pool, Task* task, void* args) {
    Shop_check* check = task->data;

    if (check->shouldRender) {
        Sprite_RenderFrame(&check->sprite);
    }
    return 1;
}

s32 func_ov043_020b4a68(TaskPool* pool, Task* task, void* args) {
    Shop_check* check = task->data;

    Sprite_Release(&check->sprite);
    return 1;
}

s32 func_ov043_020b4a7c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = func_ov043_020b4964,
        .update     = func_ov043_020b4984,
        .render     = func_ov043_020b4a48,
        .cleanup    = func_ov043_020b4a68,
    };
    return stages.iter[stage](pool, task, args);
}

s32 func_ov043_020b4ac4(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_check_Args args;
    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &data_ov043_020ca558, NULL, 0, NULL, &args);
}
