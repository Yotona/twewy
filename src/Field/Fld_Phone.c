// USA 0x020bdec8-0x020be114: the phone HUD task. A single Sprite shows the
// phone icon; the animation switches to the "message" pose when the player has
// the required item.
#include "Engine/EasyTask.h"
#include "Field/Ov030Data.h"
#include "Player/Inventory.h"
#include "SpriteMgr.h"

extern const char data_ov030_021073b8[];

SpriteFrameInfo* func_ov030_020bdec8(Sprite* sprite, s32 arg1, s32 arg2);
void             func_ov030_020bdf64(Sprite* sprite, s32* arg1);
s32              func_ov030_020bdfec(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020be004(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020be058(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020be06c(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020be080(TaskPool* pool, Task* task, void* args, s32 stage);
s32              func_ov030_020be0c8(TaskPool* pool, s32 arg1);
void             func_ov030_020be0f8(Task* task);

const TaskHandle data_ov030_020e62d0 = {data_ov030_021073b8, func_ov030_020be080, sizeof(Sprite)};

const TaskStages data_ov030_020e62dc = {
    {func_ov030_020bdfec, func_ov030_020be004, func_ov030_020be058, func_ov030_020be06c}
};

const SpriteAnimation data_ov030_020e62ec = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0C00,
    .posX              = 0xC0,
    .posY              = 0,
    .frameInfoCallback = func_ov030_020bdec8,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 4,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 0x10,
};

SpriteFrameInfo* func_ov030_020bdec8(Sprite* sprite, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(sprite, arg2);
}

void func_ov030_020bdf64(Sprite* sprite, s32* arg1) {
    SpriteAnimation anim = data_ov030_020e62ec;

    anim.dataType = (u16)*arg1;
    if (Inventory_HasRequiredQuantity(0x2A7, 1, 0) == 0) {
        anim.animIndex = 0x12;
    }
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020bdfec(TaskPool* pool, Task* task, void* args) {
    func_ov030_020bdf64(task->data, args);
    return 1;
}

s32 func_ov030_020be004(TaskPool* pool, Task* task, void* args) {
    Sprite* sprite = task->data;

    if (sprite->animIndex == 0x12 && Inventory_HasRequiredQuantity(0x2A7, 1, 0) != 0) {
        Sprite_SetAnimation(sprite, sprite->animData, 0x10, sprite->cellTable);
    }
    Sprite_Update(sprite);
    return 1;
}

s32 func_ov030_020be058(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

s32 func_ov030_020be06c(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

s32 func_ov030_020be080(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov030_020e62dc;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020be0c8(TaskPool* pool, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(pool, &data_ov030_020e62d0, NULL, 0, NULL, &param);
}

void func_ov030_020be0f8(Task* task) {
    Sprite* sprite = task->data;

    Sprite_SetAnimation(sprite, sprite->animData, 0x11, sprite->cellTable);
}
