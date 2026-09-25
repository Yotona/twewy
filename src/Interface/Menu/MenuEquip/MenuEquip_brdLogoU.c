#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuEquipObject* owner;
} MenuEquip_brdLogoU; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
    /* 0x8 */ u16              itemId;
    /* 0xA */ u8               brand;
    /* 0xB */ u8               category;
} MenuEquip_brdLogoU_Args;

static SpriteFrameInfo* MenuEquip_brdLogoU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_brdLogoU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_brdLogoU = {"Tsk_MenuEquip_brdLogoU", MenuEquip_brdLogoU_RunTask,
                                                  sizeof(MenuEquip_brdLogoU)};

static const SpriteAnimation MenuEquip_brdLogoU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .unk_04            = 0xA8,
    .unk_06            = 0x1C,
    .frameInfoCallback = MenuEquip_brdLogoU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[9],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuEquip_brdLogoU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuEquip_brdLogoU_Load(MenuEquip_brdLogoU* brdLogoU, Sprite* sprite, MenuEquip_brdLogoU_Args* args) {
    SpriteAnimation anim = MenuEquip_brdLogoU_Anim;

    anim.dataType = args->dataType;

    if (args->itemId == 0xFFFF) {
        anim.packIndex    = 1;
        brdLogoU->visible = FALSE;
    } else if (args->category == 1 && args->brand <= 12) {
        anim.packIndex    = args->brand + 1;
        brdLogoU->visible = TRUE;
    } else {
        anim.packIndex    = 1;
        brdLogoU->visible = FALSE;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuEquip_brdLogoU_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_brdLogoU*      brdLogoU     = task->data;
    MenuEquip_brdLogoU_Args* brdLogoUArgs = args;

    brdLogoU->owner = brdLogoUArgs->owner;
    MenuEquip_brdLogoU_Load(brdLogoU, &brdLogoU->sprite, brdLogoUArgs);
    return 1;
}

static s32 MenuEquip_brdLogoU_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_brdLogoU* brdLogoU = task->data;

    Sprite_Update(&brdLogoU->sprite);
    return 1;
}

static s32 MenuEquip_brdLogoU_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_brdLogoU* brdLogoU = task->data;

    if (brdLogoU->visible != FALSE) {
        Sprite_RenderFrame(&brdLogoU->sprite);
    }
    return 1;
}

static s32 MenuEquip_brdLogoU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_brdLogoU* brdLogoU = task->data;

    Sprite_Release(&brdLogoU->sprite);
    return 1;
}

static s32 MenuEquip_brdLogoU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_brdLogoU_Init,
        .update     = MenuEquip_brdLogoU_Update,
        .render     = MenuEquip_brdLogoU_Render,
        .cleanup    = MenuEquip_brdLogoU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_brdLogoU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_brdLogoU_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.itemId   = owner->cursorItem.itemId;
    args.category = owner->cursorItem.category;
    args.brand    = owner->cursorItem.brand;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_brdLogoU, NULL, 0, NULL, &args);
}
