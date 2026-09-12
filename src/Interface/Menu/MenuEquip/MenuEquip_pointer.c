#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuEquipObject* owner;
} MenuEquip_pointer; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_pointer_Args;

static SpriteFrameInfo* MenuEquip_pointer_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_pointer = {"Tsk_MenuEquip_pointer", MenuEquip_pointer_RunTask,
                                                 sizeof(MenuEquip_pointer)};

static const SpriteAnimation MenuEquip_pointer_Anim = {
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
    .unk_08     = MenuEquip_pointer_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c9758[2],
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xA,
    .unk_22     = 6,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuEquip_pointer_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

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

static void MenuEquip_pointer_Load(Sprite* sprite, MenuEquip_pointer_Args* args) {
    SpriteAnimation anim = MenuEquip_pointer_Anim;

    anim.dataType = args->dataType;
    anim.unk_2A   = 0x2D;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuEquip_pointer_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_pointer*      pointer     = task->data;
    MenuEquip_pointer_Args* pointerArgs = args;

    pointer->owner   = pointerArgs->owner;
    pointer->visible = FALSE;
    MenuEquip_pointer_Load(&pointer->sprite, pointerArgs);
    return 1;
}

static s32 MenuEquip_pointer_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_pointer* pointer = task->data;
    TouchCoord         coord;

    if (TouchInput_IsTouchActive() == FALSE) {
        pointer->visible = FALSE;
    } else {
        TouchInput_GetCoord(&coord);
        pointer->sprite.posX = coord.x;
        pointer->sprite.posY = coord.y;
        pointer->visible     = TRUE;
    }

    Sprite_Update(&pointer->sprite);
    return 1;
}

static s32 MenuEquip_pointer_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_pointer* pointer = task->data;

    if (pointer->visible == TRUE) {
        Sprite_RenderFrame(&pointer->sprite);
    }
    return 1;
}

static s32 MenuEquip_pointer_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_pointer* pointer = task->data;

    Sprite_Release(&pointer->sprite);
    return 1;
}

static s32 MenuEquip_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_pointer_Init,
        .update     = MenuEquip_pointer_Update,
        .render     = MenuEquip_pointer_Render,
        .cleanup    = MenuEquip_pointer_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_pointer_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_pointer_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_pointer, NULL, 0, NULL, &args);
}
