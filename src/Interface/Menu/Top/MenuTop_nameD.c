#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x000 */ Sprite         sprites[8];
    /* 0x200 */ s32            unk_200;
    /* 0x204 */ MenuTopObject* topMenu;
} MenuTop_nameD; // Size: 0x208

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
} MenuTop_nameD_Args;

static SpriteFrameInfo* MenuTop_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_nameD = {"Tsk_MenuTop_nameD", MenuTop_nameD_RunTask, sizeof(MenuTop_nameD)};

static const SpriteAnimation MenuTop_nameD_Anim = {
    .bits_0_1          = 1,
    .dataType          = 2,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuTop_nameD_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[3],
    .unk_18            = 0,
    .packIndex         = 1,
    .unk_1C            = 0,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 4,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuTop_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Nonmatching
static void MenuTop_nameD_Load(Sprite* sprites, MenuTop_nameD_Args* args) {
    SpriteAnimation anim = MenuTop_nameD_Anim;

    const Point positions[8] = {
        {  8, 139},
        { 91, 139},
        {183, 139},
        {  8, 152},
        {  8, 165},
        {140, 165},
        {  6, 183},
        {135, 183},
    };

    const s16 frames[8][2] = {
        {2, 3},
        {4, 5},
        {6, 7},
        {8, 9},
    };

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 8; i++) {
        anim.animIndex = frames[i][0];
        anim.posX      = positions[i].x;
        anim.posY      = positions[i].y;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuTop_nameD_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameD*      nameD    = task->data;
    MenuTop_nameD_Args* initArgs = args;

    nameD->topMenu = initArgs->topMenu;
    nameD->unk_200 = 1;
    MenuTop_nameD_Load(nameD->sprites, initArgs);
    return 1;
}

static s32 MenuTop_nameD_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameD* nameD = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_Update(&nameD->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_nameD_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameD* nameD = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_RenderFrame(&nameD->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_nameD_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameD* nameD = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_Release(&nameD->sprites[i]);
    }
    return 1;
}

s32 MenuTop_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_nameD_Init,
        .update     = MenuTop_nameD_Update,
        .render     = MenuTop_nameD_Render,
        .cleanup    = MenuTop_nameD_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_nameD_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_nameD_Args args;

    args.dataType = dataType;
    args.topMenu  = topMenu;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_nameD, NULL, 0, NULL, &args);
}
