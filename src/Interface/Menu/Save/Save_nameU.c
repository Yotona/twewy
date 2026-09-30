#include "Interface/Menu/Save.h"

typedef struct {
    /* 0x000 */ Sprite          sprites[4];
    /* 0x100 */ BOOL            visible[4];
    /* 0x110 */ SaveMenuObject* save;
} Save_nameU; // Size: 0x114

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_nameU_Args;

static SpriteFrameInfo* Save_nameU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_nameU = {"Tsk_Save_nameU", Save_nameU_RunTask, sizeof(Save_nameU)};

static const SpriteAnimation Save_nameU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xc00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Save_nameU_GetFrameInfo,
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

static SpriteFrameInfo* Save_nameU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Save_nameU_Load(Save_nameU* nameU, Sprite* sprites, Save_nameU_Args* args) {
    SpriteAnimation anim         = Save_nameU_Anim;
    Point           positions[4] = {
        {  3,  8},
        {  3, 95},
        {131, 95},
        {105,  9},
    };

    anim.dataType = args->dataType;

    for (u16 i = 0; i < 4; i++) {
        nameU->visible[i] = TRUE;
    }

    anim.animIndex = 1;
    anim.posX      = positions[0].x;
    anim.posY      = positions[0].y;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 2;
    anim.posX      = positions[1].x;
    anim.posY      = positions[1].y;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 3;
    anim.posX      = positions[2].x;
    anim.posY      = positions[2].y;
    _Sprite_Load(&sprites[2], &anim);

    anim.animIndex = 6;
    anim.posX      = positions[3].x;
    anim.posY      = positions[3].y;
    _Sprite_Load(&sprites[3], &anim);
}

static s32 Save_nameU_Init(TaskPool* pool, Task* task, void* args) {
    Save_nameU_Args* nameUArgs = args;
    Save_nameU*      nameU     = task->data;

    nameU->save = nameUArgs->save;
    Save_nameU_Load(nameU, nameU->sprites, nameUArgs);
    return 1;
}

static s32 Save_nameU_Update(TaskPool* pool, Task* task, void* args) {
    Save_nameU* nameU = task->data;

    if (nameU->save->partner == 0xFF) {
        nameU->visible[2] = FALSE;
    } else {
        Save_SetSpriteFrame(&nameU->sprites[2], nameU->save->partner + 3);
        nameU->visible[2] = TRUE;
    }

    for (s16 i = 0; i < 4; i++) {
        Sprite_Update(&nameU->sprites[i]);
    }
    return 1;
}

static s32 Save_nameU_Render(TaskPool* pool, Task* task, void* args) {
    Save_nameU* nameU = task->data;

    for (s16 i = 0; i < 4; i++) {
        if (nameU->visible[i]) {
            Sprite_RenderFrame(&nameU->sprites[i]);
        }
    }
    return 1;
}

static s32 Save_nameU_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_nameU* nameU = task->data;

    for (s16 i = 0; i < 4; i++) {
        Sprite_Release(&nameU->sprites[i]);
    }
    return 1;
}

static s32 Save_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_nameU_Init,
        .update     = Save_nameU_Update,
        .render     = Save_nameU_Render,
        .cleanup    = Save_nameU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_nameU_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_nameU_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_nameU, NULL, 0, NULL, &args);
}
