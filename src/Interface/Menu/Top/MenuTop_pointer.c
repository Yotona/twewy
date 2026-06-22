#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite             sprite;
    /* 0x40 */ BOOL               shouldRender;
    /* 0x44 */ UnkStruct_TopMenu* unk_44;
} MenuTop_pointer; // Size: 0x48

typedef struct {
    /* 0x0 */ s32                dataType;
    /* 0x4 */ UnkStruct_TopMenu* unk_4;
} MenuTop_pointer_Args;

static SpriteFrameInfo* MenuTop_pointer_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuTop_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_pointer = {"Tsk_MenuTop_pointer", MenuTop_pointer_RunTask, sizeof(MenuTop_pointer)};

static const SpriteAnimation data_ov043_020c812c = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuTop_pointer_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c7988,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 4,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuTop_pointer_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuTop_pointer_Load(Sprite* sprite, MenuTop_pointer_Args* args) {
    SpriteAnimation anim = data_ov043_020c812c;

    anim.dataType = args->dataType;
    anim.unk_2A   = 72;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_pointer_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_pointer*      pointer  = task->data;
    MenuTop_pointer_Args* initArgs = args;

    pointer->unk_44       = initArgs->unk_4;
    pointer->shouldRender = FALSE;
    MenuTop_pointer_Load(&pointer->sprite, initArgs);
    return 1;
}

static s32 MenuTop_pointer_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_pointer* pointer = task->data;
    TouchCoord       coord;

    if (TouchInput_IsTouchActive()) {
        TouchInput_GetCoord(&coord);
        pointer->sprite.posX  = coord.x;
        pointer->sprite.posY  = coord.y;
        pointer->shouldRender = TRUE;
    } else {
        pointer->shouldRender = FALSE;
    }

    Sprite_Update(&pointer->sprite);
    return 1;
}

static s32 MenuTop_pointer_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_pointer* pointer = task->data;

    if (pointer->shouldRender == TRUE) {
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

s32 MenuTop_pointer_CreateTask(TaskPool* pool, s32 dataType, UnkStruct_TopMenu* arg2) {
    MenuTop_pointer_Args args;

    args.dataType = dataType;
    args.unk_4    = arg2;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_pointer, NULL, 0, NULL, &args);
}
