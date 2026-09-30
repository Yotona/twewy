#include "Interface/Menu/Save.h"
#include "Player/Inventory.h"
#include "Save.h"

u16  Inventory_GetNoiseReportCount(void);
u16  Inventory_GetMasteredPinCount(void);
u16  Inventory_GetCollectedItemCount(void);
BOOL Savefile_AcquiredAllSecretReports(void);

typedef struct {
    /* 0x000 */ Sprite          sprites[8];
    /* 0x200 */ BOOL            visible[8];
    /* 0x220 */ SaveMenuObject* save;
} Save_chara; // Size: 0x224

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_chara_Args;

static SpriteFrameInfo* Save_chara_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_chara_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_chara = {"Tsk_Save_chara", Save_chara_RunTask, sizeof(Save_chara)};

static const SpriteAnimation Save_chara_Anim = {
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
    .frameInfoCallback = Save_chara_GetFrameInfo,
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

static SpriteFrameInfo* Save_chara_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Save_chara_Load(Save_chara* chara, Sprite* sprites, Save_chara_Args* args) {
    SpriteAnimation anim = Save_chara_Anim;
    s16             i;
#ifdef REGION_USA
    SaveMenuObject* save = chara->save;
#endif
    s16 posX[8] = {113, 143, 86, 171, 196, 215, 56, 131};

    anim.dataType = args->dataType;
    for (i = 0; i < 8; i++) {
        if (i == 5 || i == 7) {
            anim.unk_02.unk_10 = 1;
        } else {
            anim.unk_02.unk_10 = 2;
        }
        anim.animIndex = i + 7;
        anim.posX      = posX[i];
        anim.posY      = 148;
        _Sprite_Load(&sprites[i], &anim);
    }

    for (i = 0; i < 8; i++) {
        chara->visible[i] = FALSE;
    }
    chara->visible[0] = TRUE;
    chara->visible[1] = TRUE;
#ifdef REGION_USA
    if (Inventory_GetOwnedCount(ITEM_STICKER_GAME_CLEARED) != 0 || save->prevChapter >= 8) {
        chara->visible[2] = TRUE;
    }
    if (Inventory_GetOwnedCount(ITEM_STICKER_GAME_CLEARED) != 0 || save->prevChapter >= 15) {
        chara->visible[3] = TRUE;
    }
#else
    if (Inventory_GetOwnedCount(ITEM_STICKER_GAME_CLEARED) != 0 || gSaveData.chapter >= 7) {
        chara->visible[2] = TRUE;
    }
    if (Inventory_GetOwnedCount(ITEM_STICKER_GAME_CLEARED) != 0 || gSaveData.chapter >= 14) {
        chara->visible[3] = TRUE;
    }
#endif
    if (Inventory_GetCollectedItemCount() == 472) {
        chara->visible[4] = TRUE;
    }
    if (Inventory_GetNoiseReportCount() == 96) {
        chara->visible[5] = TRUE;
    }
    if (Savefile_AcquiredAllSecretReports() == TRUE) {
        chara->visible[6] = TRUE;
    }
    if (Inventory_GetMasteredPinCount() == 304) {
        chara->visible[7] = TRUE;
    }
}

static s32 Save_chara_Init(TaskPool* pool, Task* task, void* args) {
    Save_chara*      chara     = task->data;
    Save_chara_Args* charaArgs = args;

    chara->save = charaArgs->save;
    Save_chara_Load(chara, chara->sprites, charaArgs);
    return 1;
}

static s32 Save_chara_Update(TaskPool* pool, Task* task, void* args) {
    Save_chara* chara = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_Update(&chara->sprites[i]);
    }
    return 1;
}

static s32 Save_chara_Render(TaskPool* pool, Task* task, void* args) {
    Save_chara* chara = task->data;

    for (s16 i = 0; i < 8; i++) {
        if (chara->visible[i]) {
            Sprite_RenderFrame(&chara->sprites[i]);
        }
    }
    return 1;
}

static s32 Save_chara_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_chara* chara = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_Release(&chara->sprites[i]);
    }
    return 1;
}

static s32 Save_chara_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_chara_Init,
        .update     = Save_chara_Update,
        .render     = Save_chara_Render,
        .cleanup    = Save_chara_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_chara_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_chara_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_chara, NULL, 0, NULL, &args);
}
