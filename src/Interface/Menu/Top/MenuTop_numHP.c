#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"
#include "Save.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x000 */ Sprite         sprites[4];
    /* 0x100 */ BOOL           visible[4];
    /* 0x110 */ MenuTopObject* topMenu;
    /* 0x114 */ u16            initialHealth;
} MenuTop_numHP; // Size: 0x118

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ u16            health;
} MenuTop_numHP_Args;

static SpriteFrameInfo* MenuTop_numHP_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_numHP_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_numHP = {"Tsk_MenuTop_numHP", MenuTop_numHP_RunTask, sizeof(MenuTop_numHP)};

static const SpriteAnimation MenuTop_numHP_Anim = {
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
    .frameInfoCallback = MenuTop_numHP_GetFrameInfo,
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

void MenuTop_numHP_Refresh(MenuTop_numHP* taskData) {
    MenuTopObject* topMenu = taskData->topMenu;
    u16            digits[4];
    u16            hp;
    u16            remainder;
    u16            i;

    topMenu->health = gSaveData.playerStats.baseHealth + (s16)(((s16)topMenu->currentLevel - 1) * 50 + 200);
    hp              = topMenu->health;
    if (hp > 9999) {
        hp = 9999;
    }

    digits[0] = hp / 1000;
    remainder = hp % 1000;
    digits[1] = remainder / 100;
    remainder = remainder % 100;
    digits[2] = remainder / 10;
    digits[3] = remainder % 10;

    for (i = 0; i < 4; i++) {
        taskData->visible[i] = TRUE;
    }

    for (i = 0; i < 3; i++) {
        if (digits[i] != 0) {
            break;
        }
        taskData->visible[i] = FALSE;
    }

    for (i = 0; i < 4; i++) {
        MenuTop_SetSpriteFrame(&taskData->sprites[i], digits[i] + 10);
    }
}

static SpriteFrameInfo* MenuTop_numHP_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void MenuTop_numHP_Load(MenuTop_numHP* taskData, Sprite* sprites, MenuTop_numHP_Args* args) {
    MenuTopObject*  topMenu = taskData->topMenu;
    SpriteAnimation anim    = MenuTop_numHP_Anim;
    u16             digits[4];
    u16             hp;
    u16             remainder;
    u16             i;

    anim.dataType = args->dataType;

    topMenu->health = gSaveData.playerStats.baseHealth + (s16)(((s16)topMenu->currentLevel - 1) * 50 + 200);
    hp              = topMenu->health;
    if (hp > 9999) {
        hp = 9999;
    }

    digits[0] = hp / 1000;
    remainder = hp % 1000;
    digits[1] = remainder / 100;
    remainder = remainder % 100;
    digits[2] = remainder / 10;
    digits[3] = remainder % 10;

    for (i = 0; i < 4; i++) {
        taskData->visible[i] = TRUE;
    }

    for (i = 0; i < 3; i++) {
        if (digits[i] != 0) {
            break;
        }
        taskData->visible[i] = FALSE;
    }

    for (i = 0; i < 4; i++) {
        anim.animIndex = digits[i] + 10;
#ifdef REGION_USA
        anim.posX = i * 6 + 212;
#else
        anim.posX = i * 8 + 210;
#endif
        anim.posY = 139;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuTop_numHP_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_numHP*      taskData = task->data;
    MenuTop_numHP_Args* initArgs = args;

    taskData->topMenu       = initArgs->topMenu;
    taskData->initialHealth = initArgs->health;
    MenuTop_numHP_Load(taskData, taskData->sprites, initArgs);
    return 1;
}

static s32 MenuTop_numHP_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_numHP* taskData = task->data;

    MenuTop_numHP_Refresh(taskData);

    for (s32 i = 0; i < 4; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_numHP_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_numHP* taskData = task->data;

    for (s32 i = 0; i < 4; i++) {
        if (taskData->visible[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_numHP_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_numHP* taskData = task->data;

    for (s32 i = 0; i < 4; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }
    return 1;
}

s32 MenuTop_numHP_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_numHP_Init,
        .update     = MenuTop_numHP_Update,
        .render     = MenuTop_numHP_Render,
        .cleanup    = MenuTop_numHP_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_numHP_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_numHP_Args args;

    args.dataType = dataType;
    args.topMenu  = topMenu;
    args.health   = topMenu->health;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_numHP, NULL, 0, NULL, &args);
}
