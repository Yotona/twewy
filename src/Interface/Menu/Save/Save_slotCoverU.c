#include "Interface/Menu/Save.h"

typedef struct {
    /* 0x00 */ Sprite          sprite;
    /* 0x40 */ BOOL            visible;
    /* 0x44 */ SaveMenuObject* save;
} Save_slotCoverU; // Size: 0x48

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
    /* 0x8 */ u16             slot;
} Save_slotCoverU_Args;

static SpriteFrameInfo* Save_slotCoverU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_slotCoverU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_slotCoverU = {"Tsk_Save_slotCoverU", Save_slotCoverU_RunTask, sizeof(Save_slotCoverU)};

static const SpriteAnimation Save_slotCoverU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Save_slotCoverU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Save_BinIdentifiers[7],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* Save_slotCoverU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Save_slotCoverU_Load(Save_slotCoverU* slotCoverU, Sprite* sprite, Save_slotCoverU_Args* args) {
    SaveMenuObject* save         = slotCoverU->save;
    SpriteAnimation anim         = Save_slotCoverU_Anim;
    Point           positions[6] = {
        { 61, 165},
        { 95, 165},
        {129, 165},
        {163, 165},
        {197, 165},
        {231, 165},
    };

    anim.dataType  = args->dataType;
    anim.animIndex = 35;
    anim.posX      = positions[args->slot].x + 2;
    anim.posY      = positions[args->slot].y + 2;
    _Sprite_Load(sprite, &anim);
    slotCoverU->visible = args->slot >= save->badgeSlots;
}

static s32 Save_slotCoverU_Init(TaskPool* pool, Task* task, void* args) {
    Save_slotCoverU*      slotCoverU     = task->data;
    Save_slotCoverU_Args* slotCoverUArgs = args;

    slotCoverU->save = slotCoverUArgs->save;
    Save_slotCoverU_Load(slotCoverU, &slotCoverU->sprite, slotCoverUArgs);
    return 1;
}

static s32 Save_slotCoverU_Update(TaskPool* pool, Task* task, void* args) {
    Save_slotCoverU* slotCoverU = task->data;

    Sprite_Update(&slotCoverU->sprite);
    return 1;
}

static s32 Save_slotCoverU_Render(TaskPool* pool, Task* task, void* args) {
    Save_slotCoverU* slotCoverU = task->data;

    if (slotCoverU->visible) {
        Sprite_RenderFrame(&slotCoverU->sprite);
    }
    return 1;
}

static s32 Save_slotCoverU_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_slotCoverU* slotCoverU = task->data;

    Sprite_Release(&slotCoverU->sprite);
    return 1;
}

static s32 Save_slotCoverU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_slotCoverU_Init,
        .update     = Save_slotCoverU_Update,
        .render     = Save_slotCoverU_Render,
        .cleanup    = Save_slotCoverU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_slotCoverU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, SaveMenuObject* save) {
    Save_slotCoverU_Args args;

    args.dataType = dataType;
    args.save     = save;
    args.slot     = slot;

    return EasyTask_CreateTask(pool, &Tsk_Save_slotCoverU, NULL, 0, NULL, &args);
}
