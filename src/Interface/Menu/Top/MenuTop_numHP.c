#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"
#include "Save.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x000 */ Sprite         sprites[4];
    /* 0x100 */ s32            visibleFlags[4];
    /* 0x110 */ MenuTopObject* topMenu;
    /* 0x114 */ u16            initialHealth;
} MenuTop_numHP; // Size: 0x118

typedef struct {
    /* 0x0 */ s32   dataType;
    /* 0x4 */ void* topMenu;
    /* 0x8 */ u16   health;
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
    .unk_04            = 0x50,
    .unk_06            = 0x50,
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
    .unk_2A            = 1,
};

// Nonmatching
void MenuTop_numHP_Refresh(void* taskDataPtr) {
    MenuTop_numHP* taskData = taskDataPtr;
    u16            digits[4];
    u32            hp;

    hp = gSaveData.playerStats.baseHealth + (((taskData->topMenu->currentLevel - 1) * 50) + 200);
    if (hp > 9999) {
        hp = 9999;
    }
    taskData->topMenu->health = hp;

    digits[0] = (u16)(hp / 1000);
    hp %= 1000;
    digits[1] = (u16)(hp / 100);
    hp %= 100;
    digits[2] = (u16)(hp / 10);
    digits[3] = (u16)(hp % 10);

    for (s32 i = 0; i < 4; i++) {
        taskData->visibleFlags[i] = 1;
    }

    for (s32 i = 0; i < 3 && digits[i] == 0; i++) {
        taskData->visibleFlags[i] = 0;
    }

    for (u16 i = 0; i < 4; i++) {
        MenuTop_SetSpriteFrame(&taskData->sprites[i], (s16)(digits[i] + 0xA));
    }
}

static SpriteFrameInfo* MenuTop_numHP_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Nonmatching
void MenuTop_numHP_Load(void* taskDataPtr, void* spritesPtr, void* argsPtr) {
    MenuTop_numHP*      taskData = taskDataPtr;
    Sprite*             sprites  = spritesPtr;
    MenuTop_numHP_Args* args     = argsPtr;
    SpriteAnimation     anim     = MenuTop_numHP_Anim;
    u16                 digits[4];
    s32                 hp;

    anim.dataType = (u16)args->dataType;

    hp = gSaveData.playerStats.baseHealth + (((taskData->topMenu->currentLevel - 1) * 50) + 200);
    if (hp > 9999) {
        hp = 9999;
    }
    taskData->topMenu->health = (u16)hp;

    digits[0] = (hp / 1000);
    hp %= 1000;
    digits[1] = (hp / 100);
    hp %= 100;
    digits[2] = (hp / 10);
    digits[3] = (hp % 10);

    for (s32 i = 0; i < 4; i++) {
        taskData->visibleFlags[i] = 1;
    }

    for (s32 i = 0; i < 3 && digits[i] == 0; i++) {
        taskData->visibleFlags[i] = 0;
    }

    for (u16 i = 0; i < 4; i++) {
        anim.unk_2A = digits[i] + 10;
        anim.unk_04 = (i * 6) + 212;
        anim.unk_06 = 139;
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
        if (taskData->visibleFlags[i] != 0) {
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
