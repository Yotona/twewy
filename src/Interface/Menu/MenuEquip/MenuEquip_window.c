#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuEquipObject* owner;
} MenuEquip_window; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_window_Args;

static SpriteFrameInfo* MenuEquip_window_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_window_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_window = {"Tsk_MenuEquip_window", MenuEquip_window_RunTask, sizeof(MenuEquip_window)};

static const SpriteAnimation MenuEquip_window_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_window_GetFrameInfo,
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

static SpriteFrameInfo* MenuEquip_window_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuEquip_window_Load(Sprite* sprite, MenuEquip_window_Args* args) {
    SpriteAnimation anim = MenuEquip_window_Anim;

    anim.dataType = args->dataType;
    anim.unk_2A   = 0x2A;
    anim.unk_04   = 0x80;
    anim.unk_06   = 0x40;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuEquip_window_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_window*      window     = task->data;
    MenuEquip_window_Args* windowArgs = args;

    window->visible = TRUE;
    window->owner   = windowArgs->owner;
    MenuEquip_window_Load(&window->sprite, windowArgs);
    return 1;
}

static s32 MenuEquip_window_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_window* window = task->data;

    if (window->owner->dirtyFlags & 0x200) {
        window->visible = TRUE;
    } else {
        window->visible = FALSE;
    }

    Sprite_Update(&window->sprite);
    return 1;
}

static s32 MenuEquip_window_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_window* window = task->data;

    if (window->visible != 0) {
        Sprite_RenderFrame(&window->sprite);
    }
    return 1;
}

static s32 MenuEquip_window_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_window* window = task->data;

    Sprite_Release(&window->sprite);
    return 1;
}

static s32 MenuEquip_window_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_window_Init,
        .update     = MenuEquip_window_Update,
        .render     = MenuEquip_window_Render,
        .cleanup    = MenuEquip_window_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_window_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_window_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_window, NULL, 0, NULL, &args);
}
