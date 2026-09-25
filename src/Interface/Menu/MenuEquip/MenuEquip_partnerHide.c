#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuEquipObject* owner;
} MenuEquip_partnerHide; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_partnerHide_Args;

static SpriteFrameInfo* MenuEquip_partnerHide_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_partnerHide_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_partnerHide = {"Tsk_MenuEquip_partnerHide", MenuEquip_partnerHide_RunTask,
                                                     sizeof(MenuEquip_partnerHide)};

static const SpriteAnimation MenuEquip_partnerHide_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .unk_04            = 0xC0,
    .unk_06            = 0x44,
    .frameInfoCallback = MenuEquip_partnerHide_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 0xA,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuEquip_partnerHide_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuEquip_partnerHide_Load(Sprite* sprite, MenuEquip_partnerHide_Args* args) {
    SpriteAnimation anim = MenuEquip_partnerHide_Anim;

    anim.dataType = args->dataType;
    anim.unk_2A   = 0x2B;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuEquip_partnerHide_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_partnerHide*      partnerHide     = task->data;
    MenuEquip_partnerHide_Args* partnerHideArgs = args;

    partnerHide->owner   = partnerHideArgs->owner;
    partnerHide->visible = TRUE;
    MenuEquip_partnerHide_Load(&partnerHide->sprite, partnerHideArgs);
    return 1;
}

static s32 MenuEquip_partnerHide_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_partnerHide* partnerHide = task->data;

    Sprite_Update(&partnerHide->sprite);
    return 1;
}

static s32 MenuEquip_partnerHide_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_partnerHide* partnerHide = task->data;

    Sprite_RenderFrame(&partnerHide->sprite);
    return 1;
}

static s32 MenuEquip_partnerHide_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_partnerHide* partnerHide = task->data;

    Sprite_Release(&partnerHide->sprite);
    return 1;
}

static s32 MenuEquip_partnerHide_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_partnerHide_Init,
        .update     = MenuEquip_partnerHide_Update,
        .render     = MenuEquip_partnerHide_Render,
        .cleanup    = MenuEquip_partnerHide_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_partnerHide_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_partnerHide_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_partnerHide, NULL, 0, NULL, &args);
}
