#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ BOOL           visible;
    /* 0x44 */ MenuTopObject* topMenu;
    /* 0x48 */ u16            initialPartnerAI;
} MenuTop_drawPtrAI; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ u16            partnerAI;
} MenuTop_drawPtrAI_Args;

static SpriteFrameInfo* MenuTop_drawPtrAI_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_drawPtrAI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_drawPtrAI = {"Tsk_MenuTop_drawPtrAI", MenuTop_drawPtrAI_RunTask,
                                                 sizeof(MenuTop_drawPtrAI)};

static const SpriteAnimation MenuTop_drawPtrAI_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuTop_drawPtrAI_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[3],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 4,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static s16 MenuTop_drawPtrAI_GetFrame(void* arg0) {
    MenuTop_drawPtrAI* taskData = arg0;

    s16 frames[4] = {0x38, 0x3B, 0x3A, 0x39};

    return frames[taskData->topMenu->partnerAI];
}

static SpriteFrameInfo* MenuTop_drawPtrAI_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_drawPtrAI_Load(MenuTop_drawPtrAI* taskData, Sprite* sprite, MenuTop_drawPtrAI_Args* args) {
    SpriteAnimation anim = MenuTop_drawPtrAI_Anim;

    s32 val = MenuTop_drawPtrAI_GetFrame(taskData);

    anim.dataType     = args->dataType;
    taskData->visible = TRUE;
    anim.animIndex    = val;
    anim.posX         = 218;
    anim.posY         = 164;

    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_drawPtrAI_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawPtrAI*      taskData = task->data;
    MenuTop_drawPtrAI_Args* initArgs = args;

    taskData->topMenu          = initArgs->topMenu;
    taskData->initialPartnerAI = initArgs->partnerAI;
    MenuTop_drawPtrAI_Load(taskData, &taskData->sprite, initArgs);
    return 1;
}

static s32 MenuTop_drawPtrAI_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawPtrAI* taskData = task->data;

    MenuTop_SetSpriteFrame(&taskData->sprite, MenuTop_drawPtrAI_GetFrame(taskData));
    Sprite_Update(&taskData->sprite);
    return 1;
}

static s32 MenuTop_drawPtrAI_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawPtrAI* taskData = task->data;

    if (taskData->visible != 0) {
        Sprite_RenderFrame(&taskData->sprite);
    }
    return 1;
}

static s32 MenuTop_drawPtrAI_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawPtrAI* taskData = task->data;

    Sprite_Release(&taskData->sprite);
    return 1;
}

s32 MenuTop_drawPtrAI_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_drawPtrAI_Init,
        .update     = MenuTop_drawPtrAI_Update,
        .render     = MenuTop_drawPtrAI_Render,
        .cleanup    = MenuTop_drawPtrAI_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_drawPtrAI_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_drawPtrAI_Args args;

    args.dataType  = dataType;
    args.topMenu   = topMenu;
    args.partnerAI = topMenu->partnerAI;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_drawPtrAI, NULL, 0, NULL, &args);
}
