#include "Interface/Menu/TusinSet.h"

typedef struct {
    /* 0x00 */ Sprite          sprite;
    /* 0x40 */ BOOL            visible;
    /* 0x44 */ TusinSetObject* tusinSet;
} TusinSet_item; // Size: 0x48

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
    /* 0x8 */ u16             slot;
    /* 0xA */ u16             itemId;
    /* 0xC */ u16             graphicIndex;
} TusinSet_item_Args;

static SpriteFrameInfo* TusinSet_item_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_item_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_item = {"Tsk_TusinSet_item", TusinSet_item_RunTask, sizeof(TusinSet_item)};

static const SpriteAnimation TusinSet_item_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = TusinSet_item_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[10],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* TusinSet_item_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x384000);
}

// Nonmatching: r2/r3 swap on the slot position table base
static void TusinSet_item_Load(TusinSet_item* item, Sprite* sprite, TusinSet_item_Args* args) {
    SpriteAnimation anim = TusinSet_item_Anim;
    const u16*      pos;

    anim.dataType = args->dataType;
    pos           = (const u16*)&TusinSet_SlotPositions[args->slot];
    anim.posX     = pos[0];
    anim.posY     = pos[1];
    anim.bits_7_9 = 6;
    if (args->itemId == 0xFFFF) {
        anim.packIndex = 1;
        item->visible  = FALSE;
    } else {
        anim.packIndex = args->graphicIndex + 1;
        item->visible  = TRUE;
    }
    _Sprite_Load(sprite, &anim);
}

static s32 TusinSet_item_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_item*      item     = task->data;
    TusinSet_item_Args* initArgs = args;

    item->tusinSet = initArgs->tusinSet;
    TusinSet_item_Load(item, &item->sprite, initArgs);
    return 1;
}

static s32 TusinSet_item_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_item* item = task->data;

    Sprite_Update(&item->sprite);
    return 1;
}

static s32 TusinSet_item_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_item* item = task->data;

    if (item->visible) {
        Sprite_RenderFrame(&item->sprite);
    }
    return 1;
}

static s32 TusinSet_item_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_item* item = task->data;

    Sprite_Release(&item->sprite);
    return 1;
}

static s32 TusinSet_item_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_item_Init,
        .update     = TusinSet_item_Update,
        .render     = TusinSet_item_Render,
        .cleanup    = TusinSet_item_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_item_CreateTask(TaskPool* pool, s32 dataType, u16 slot, TusinSetObject* tusinSet) {
    TusinSet_item_Args args;

    args.slot         = slot;
    args.tusinSet     = tusinSet;
    args.dataType     = dataType;
    args.itemId       = tusinSet->visible[slot]->itemId;
    args.graphicIndex = tusinSet->visible[slot]->graphicIndex;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_item, NULL, 0, NULL, &args);
}

void TusinSet_item_ReleaseSprite(TaskPool* pool, s32 taskId) {
    TusinSet_item* item = EasyTask_GetTaskData(pool, taskId);

    Sprite_Release(&item->sprite);
}
