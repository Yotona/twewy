#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ BOOL           visible;
    /* 0x44 */ MenuTopObject* topMenu;
} MenuTop_pointer; // Size: 0x48

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
} MenuTop_pointer_Args;

static SpriteFrameInfo* MenuTop_pointer_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_pointer = {"Tsk_MenuTop_pointer", MenuTop_pointer_RunTask, sizeof(MenuTop_pointer)};

static const SpriteAnimation MenuTop_pointer_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuTop_pointer_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[3],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 4,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuTop_pointer_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_pointer_Load(Sprite* sprite, MenuTop_pointer_Args* args) {
    SpriteAnimation anim = MenuTop_pointer_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 72;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_pointer_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_pointer*      pointer  = task->data;
    MenuTop_pointer_Args* initArgs = args;

    pointer->topMenu = initArgs->topMenu;
    pointer->visible = FALSE;
    MenuTop_pointer_Load(&pointer->sprite, initArgs);
    return 1;
}

static s32 MenuTop_pointer_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_pointer* pointer = task->data;
    TouchCoord       coord;

    if (TouchInput_IsTouchActive()) {
        TouchInput_GetCoord(&coord);
        pointer->sprite.posX = coord.x;
        pointer->sprite.posY = coord.y;
        pointer->visible     = TRUE;
    } else {
        pointer->visible = FALSE;
    }

    Sprite_Update(&pointer->sprite);
    return 1;
}

static s32 MenuTop_pointer_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_pointer* pointer = task->data;

    if (pointer->visible == TRUE) {
        Sprite_RenderFrame(&pointer->sprite);
    }
    return 1;
}

static s32 MenuTop_pointer_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_pointer* pointer = task->data;

    Sprite_Release(&pointer->sprite);
    return 1;
}

static s32 MenuTop_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_pointer_Init,
        .update     = MenuTop_pointer_Update,
        .render     = MenuTop_pointer_Render,
        .cleanup    = MenuTop_pointer_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_pointer_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_pointer_Args args;

    args.dataType = dataType;
    args.topMenu  = topMenu;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_pointer, NULL, 0, NULL, &args);
}
