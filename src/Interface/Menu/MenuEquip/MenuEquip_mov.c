#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuEquipObject* owner;
} MenuEquip_mov; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
    /* 0x8 */ u16              itemId;
    /* 0xA */ u16              graphicIndex;
} MenuEquip_mov_Args;

static SpriteFrameInfo* MenuEquip_mov_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_mov_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_mov = {"Tsk_MenuEquip_mov", MenuEquip_mov_RunTask, sizeof(MenuEquip_mov)};

static const SpriteAnimation MenuEquip_mov_Anim = {
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
    .frameInfoCallback = MenuEquip_mov_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[6],
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

static SpriteFrameInfo* MenuEquip_mov_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x3E8000);
}

static void MenuEquip_mov_Load(MenuEquip_mov* mov, Sprite* sprite, MenuEquip_mov_Args* args) {
    SpriteAnimation anim = MenuEquip_mov_Anim;

    anim.dataType = args->dataType;
    anim.bits_7_9 = MenuEquip_GetFreePaletteSlot();

    if (args->itemId == 0xFFFF) {
        return;
    }

    anim.packIndex = args->graphicIndex + 1;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuEquip_mov_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_mov*      mov     = task->data;
    MenuEquip_mov_Args* movArgs = args;

    mov->visible = TRUE;
    mov->owner   = movArgs->owner;
    MenuEquip_mov_Load(mov, &mov->sprite, movArgs);
    return 1;
}

static s32 MenuEquip_mov_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_mov*   mov   = task->data;
    MenuEquipObject* owner = mov->owner;

    mov->sprite.posX = owner->touchPosX;
    mov->sprite.posY = owner->touchPosY;
    Sprite_Update(&mov->sprite);
    return 1;
}

static s32 MenuEquip_mov_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_mov* mov = task->data;

    if (mov->visible != 0) {
        Sprite_RenderFrame(&mov->sprite);
    }
    return 1;
}

static s32 MenuEquip_mov_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_mov* mov = task->data;

    Sprite_Release(&mov->sprite);
    return 1;
}

static s32 MenuEquip_mov_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_mov_Init,
        .update     = MenuEquip_mov_Update,
        .render     = MenuEquip_mov_Render,
        .cleanup    = MenuEquip_mov_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_mov_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_mov_Args args;

    args.dataType     = dataType;
    args.owner        = owner;
    args.itemId       = owner->cursorItem.itemId;
    args.graphicIndex = owner->cursorItem.graphicIndex;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_mov, NULL, 0, NULL, &args);
}
