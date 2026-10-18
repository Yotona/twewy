#include "Field/Tasks/Fld_Phone.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "Player/Inventory.h"
#include "SpriteMgr.h"

static SpriteFrameInfo* Fld_Phone_GetFrameInfo(Sprite* sprite, s32 arg1, s32 arg2);
static s32              Fld_Phone_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Phone = {"Tsk_Fld_Phone", Fld_Phone_RunTask, sizeof(Sprite)};

static const SpriteAnimation Fld_Phone_Anim = {
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
    .frameInfoCallback = Fld_Phone_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
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

static SpriteFrameInfo* Fld_Phone_GetFrameInfo(Sprite* sprite, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(sprite, arg2);
}

static void Fld_Phone_Load(Sprite* sprite, s32* arg1) {
    SpriteAnimation anim = Fld_Phone_Anim;

    anim.dataType = (u16)*arg1;
    if (Inventory_HasRequiredQuantity(0x2A7, 1, 0) == 0) {
        anim.animIndex = 0x12;
    }
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_Phone_Init(TaskPool* pool, Task* task, void* args) {
    Fld_Phone_Load(task->data, args);
    return 1;
}

static s32 Fld_Phone_Update(TaskPool* pool, Task* task, void* args) {
    Sprite* sprite = task->data;

    if (sprite->animIndex == 0x12 && Inventory_HasRequiredQuantity(0x2A7, 1, 0) != 0) {
        Sprite_SetAnimation(sprite, sprite->animData, 0x10, sprite->cellTable);
    }
    Sprite_Update(sprite);
    return 1;
}

static s32 Fld_Phone_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_Phone_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_Phone_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Phone_Init,
        .update     = Fld_Phone_Update,
        .render     = Fld_Phone_Render,
        .cleanup    = Fld_Phone_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Phone_CreateTask(TaskPool* pool, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(pool, &Tsk_Fld_Phone, NULL, 0, NULL, &param);
}

void Fld_Phone_Press(Task* task) {
    Sprite* sprite = task->data;

    Sprite_SetAnimation(sprite, sprite->animData, 0x11, sprite->cellTable);
}
