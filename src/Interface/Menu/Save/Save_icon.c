#include "Interface/Menu/Save.h"

typedef struct {
    /* 0x00 */ Sprite          sprites[2];
    /* 0x80 */ char            unk_80[0x84 - 0x80];
    /* 0x84 */ SaveMenuObject* save;
} Save_icon; // Size: 0x88

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_icon_Args;

static SpriteFrameInfo* Save_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Save_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_icon = {"Tsk_Save_icon", Save_icon_RunTask, sizeof(Save_icon)};

static const SpriteAnimation Save_icon_Anim = {
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
    .frameInfoCallback = Save_icon_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Save_BinIdentifiers[8],
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

static SpriteFrameInfo* Save_icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Save_icon_Load(Save_icon* icon, Save_icon_Args* args) {
    SpriteAnimation anim = Save_icon_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 3;
    anim.posX      = 218;
    anim.posY      = 12;
    _Sprite_Load(&icon->sprites[0], &anim);

    anim.animIndex = 5;
    anim.posX      = 243;
    anim.posY      = 12;
    _Sprite_Load(&icon->sprites[1], &anim);
}

static s32 Save_icon_Init(TaskPool* pool, Task* task, void* args) {
    Save_icon*      icon     = task->data;
    Save_icon_Args* initArgs = args;

    icon->save = initArgs->save;
    Save_icon_Load(icon, initArgs);
    return 1;
}

static s32 Save_icon_Update(TaskPool* pool, Task* task, void* args) {
    Save_icon*      icon = task->data;
    SaveMenuObject* save = icon->save;
    s32             i;

    for (i = 0; i < 2; i++) {
        if (save->iconPressed[i] == 1) {
            Save_SetSpriteFrame(&icon->sprites[i], i * 2 + 4);
            if (save->iconTimer != 0) {
                save->iconTimer--;
            } else {
                save->iconPressed[i] = 0;
            }
        } else {
            Save_SetSpriteFrame(&icon->sprites[i], i * 2 + 3);
        }
    }

    for (i = 0; i < 2; i++) {
        Sprite_Update(&icon->sprites[i]);
    }
    return 1;
}

static s32 Save_icon_Render(TaskPool* pool, Task* task, void* args) {
    Save_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_RenderFrame(&icon->sprites[i]);
    }
    return 1;
}

static s32 Save_icon_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&icon->sprites[i]);
    }
    return 1;
}

static s32 Save_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_icon_Init,
        .update     = Save_icon_Update,
        .render     = Save_icon_Render,
        .cleanup    = Save_icon_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_icon_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_icon_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_icon, NULL, 0, NULL, &args);
}
