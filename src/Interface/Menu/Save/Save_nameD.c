#include "Interface/Menu/Save.h"

typedef struct {
    /* 0x00 */ Sprite          sprite;
    /* 0x40 */ BOOL            visible;
    /* 0x44 */ SaveMenuObject* save;
} Save_nameD; // Size: 0x48

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_nameD_Args;

static SpriteFrameInfo* Save_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_nameD = {"Tsk_Save_nameD", Save_nameD_RunTask, sizeof(Save_nameD)};

static const SpriteAnimation Save_nameD_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Save_nameD_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Save_BinIdentifiers[3],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 13,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* Save_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Save_nameD_Load(Save_nameD* nameD, Save_nameD_Args* args) {
    SpriteAnimation anim = Save_nameD_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 2;
    anim.posX      = 8;
    anim.posY      = 31;
    _Sprite_Load(&nameD->sprite, &anim);
}

static s32 Save_nameD_Init(TaskPool* pool, Task* task, void* args) {
    Save_nameD*      nameD    = task->data;
    Save_nameD_Args* initArgs = args;

    nameD->visible = TRUE;
    nameD->save    = initArgs->save;
    Save_nameD_Load(nameD, initArgs);
    return 1;
}

static s32 Save_nameD_Update(TaskPool* pool, Task* task, void* args) {
    Save_nameD* nameD = task->data;

    Sprite_Update(&nameD->sprite);
    return 1;
}

static s32 Save_nameD_Render(TaskPool* pool, Task* task, void* args) {
    Save_nameD* nameD = task->data;

    Sprite_RenderFrame(&nameD->sprite);
    return 1;
}

static s32 Save_nameD_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_nameD* nameD = task->data;

    Sprite_Release(&nameD->sprite);
    return 1;
}

static s32 Save_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_nameD_Init,
        .update     = Save_nameD_Update,
        .render     = Save_nameD_Render,
        .cleanup    = Save_nameD_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_nameD_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_nameD_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_nameD, NULL, 0, NULL, &args);
}
