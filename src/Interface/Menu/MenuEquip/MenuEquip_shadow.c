#include "Interface/Menu/MenuEquip.h"
#include "common_data.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuEquipObject* owner;
} MenuEquip_shadow; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_shadow_Args;

static SpriteFrameInfo* MenuEquip_shadow_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_shadow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_shadow = {"Tsk_MenuEquip_shadow", MenuEquip_shadow_RunTask, sizeof(MenuEquip_shadow)};

static const SpriteAnimation MenuEquip_shadow_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_shadow_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 4,
    .unk_1E            = 0,
    .unk_20            = 0xA,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 5,
    .unk_28            = 6,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuEquip_shadow_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x3CA000);
}

static void MenuEquip_shadow_Load(Sprite* sprite, MenuEquip_shadow_Args* args) {
    SpriteAnimation anim = MenuEquip_shadow_Anim;

    anim.dataType        = args->dataType;
    anim.unk_2A          = 0x3D;
    anim.unk_02.unk_02   = 1;
    data_0206a890.unk_0C = 0xA;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuEquip_shadow_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_shadow*      shadow     = task->data;
    MenuEquip_shadow_Args* shadowArgs = args;

    MenuEquip_shadow_Load(&shadow->sprite, shadowArgs);
    shadow->visible = TRUE;
    shadow->owner   = shadowArgs->owner;
    return 1;
}

static s32 MenuEquip_shadow_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_shadow* shadow = task->data;
    MenuEquipObject*  owner  = shadow->owner;

    shadow->sprite.posX = owner->touchPosX + 4;
    shadow->sprite.posY = owner->touchPosY + 6;
    Sprite_Update(&shadow->sprite);
    return 1;
}

static s32 MenuEquip_shadow_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_shadow* shadow = task->data;

    if (shadow->visible != 0) {
        Sprite_RenderFrame(&shadow->sprite);
    }
    return 1;
}

static s32 MenuEquip_shadow_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_shadow* shadow = task->data;

    Sprite_Release(&shadow->sprite);
    return 1;
}

static s32 MenuEquip_shadow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_shadow_Init,
        .update     = MenuEquip_shadow_Update,
        .render     = MenuEquip_shadow_Render,
        .cleanup    = MenuEquip_shadow_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_shadow_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_shadow_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_shadow, NULL, 0, NULL, &args);
}
