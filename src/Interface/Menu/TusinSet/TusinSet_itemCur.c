#include "Interface/Menu/TusinSet.h"

typedef struct {
    /* 0x00 */ Sprite          sprite;
    /* 0x40 */ BOOL            visible;
    /* 0x44 */ TusinSetObject* tusinSet;
} TusinSet_itemCur; // Size: 0x48

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_itemCur_Args;

static SpriteFrameInfo* TusinSet_itemCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_itemCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_itemCur = {"Tsk_TusinSet_itemCur", TusinSet_itemCur_RunTask, sizeof(TusinSet_itemCur)};

static const SpriteAnimation TusinSet_itemCur_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = TusinSet_itemCur_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 7,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* TusinSet_itemCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_itemCur_Load(TusinSet_itemCur* itemCur, TusinSet_itemCur_Args* args) {
    SpriteAnimation anim = TusinSet_itemCur_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 21;
    {
        const u16* pos = (const u16*)&TusinSet_SlotPositions[0];

        anim.posX = pos[0];
        anim.posY = pos[1];
    }
    _Sprite_Load(&itemCur->sprite, &anim);
}

static s32 TusinSet_itemCur_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_itemCur*      itemCur  = task->data;
    TusinSet_itemCur_Args* initArgs = args;

    itemCur->tusinSet = initArgs->tusinSet;
    itemCur->visible  = TRUE;
    TusinSet_itemCur_Load(itemCur, initArgs);
    return 1;
}

static s32 TusinSet_itemCur_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_itemCur* itemCur  = task->data;
    TusinSetObject*   tusinSet = itemCur->tusinSet;

    if (tusinSet->cursor >= tusinSet->scroll && tusinSet->cursor < tusinSet->scroll + 16) {
        u16 slot = tusinSet->cursor - tusinSet->scroll;

        itemCur->sprite.posX = TusinSet_SlotPositions[slot].x;
        itemCur->sprite.posY = TusinSet_SlotPositions[slot].y;
        itemCur->visible     = TRUE;
    } else {
        itemCur->visible = FALSE;
    }
    if (tusinSet->helpOpen != 0) {
        itemCur->visible = FALSE;
    }
    Sprite_Update(&itemCur->sprite);
    return 1;
}

static s32 TusinSet_itemCur_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_itemCur* itemCur = task->data;

    if (itemCur->visible) {
        Sprite_RenderFrame(&itemCur->sprite);
    }
    return 1;
}

static s32 TusinSet_itemCur_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_itemCur* itemCur = task->data;

    Sprite_Release(&itemCur->sprite);
    return 1;
}

static s32 TusinSet_itemCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_itemCur_Init,
        .update     = TusinSet_itemCur_Update,
        .render     = TusinSet_itemCur_Render,
        .cleanup    = TusinSet_itemCur_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_itemCur_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_itemCur_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_itemCur, NULL, 0, NULL, &args);
}
