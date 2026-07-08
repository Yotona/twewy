#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
} MenuBadge_pointer; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_pointer_Args;

static SpriteFrameInfo* MenuBadge_pointer_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_pointer = {"Tsk_MenuBadge_pointer", MenuBadge_pointer_RunTask,
                                                 sizeof(MenuBadge_pointer)};

static const SpriteAnimation data_ov043_020c8dd8 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_pointer_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_pointer_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

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

static void MenuBadge_pointer_Load(Sprite* sprite, MenuBadge_pointer_Args* args) {
    SpriteAnimation anim = data_ov043_020c8dd8;

    anim.dataType = args->dataType;
    anim.unk_2A   = 0x5D;

    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_pointer_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_pointer*      pointer     = task->data;
    MenuBadge_pointer_Args* pointerArgs = args;

    pointer->menuBadge = pointerArgs->menuBadge;
    pointer->visible   = FALSE;
    MenuBadge_pointer_Load(&pointer->sprite, pointerArgs);
    return 1;
}

static s32 MenuBadge_pointer_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_pointer* pointer = task->data;
    TouchCoord         coord;

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

static s32 MenuBadge_pointer_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_pointer* pointer = task->data;

    if (pointer->visible == 1) {
        Sprite_RenderFrame(&pointer->sprite);
    }
    return 1;
}

static s32 MenuBadge_pointer_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_pointer* pointer = task->data;

    Sprite_Release(&pointer->sprite);
    return 1;
}

static s32 MenuBadge_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_pointer_Init,
        .update     = MenuBadge_pointer_Update,
        .render     = MenuBadge_pointer_Render,
        .cleanup    = MenuBadge_pointer_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_pointer_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_pointer_Args args;

    args.dataType  = dataType;
    args.menuBadge = badge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_pointer, NULL, 0, NULL, &args);
}
