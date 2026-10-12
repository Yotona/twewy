#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x00 */ Sprite  sprite;
    /* 0x40 */ s32     posX; // 20.12 fixed point
    /* 0x44 */ s32     targetPosX;
    /* 0x48 */ u16     slideFrames;
    /* 0x4A */ u8      drawDelay;
    /* 0x4C */ void*   message;
    /* 0x50 */ SysFont font;
} Fld_GetPlate;

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s32 messageIndex;
} Fld_GetPlate_Args;

s32                  Fld_GetPlate_Init(TaskPool* pool, Task* task, void* args);
s32                  Fld_GetPlate_Render(TaskPool* pool, Task* task, void* args);
s32                  Fld_GetPlate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
SpriteFrameInfo*     Fld_GetPlate_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
extern BinIdentifier data_ov030_020d9944;

static const TaskHandle      Tsk_Fld_GetPlate  = {"Tsk_Fld_GetPlate", Fld_GetPlate_RunTask, sizeof(Fld_GetPlate)};
static const SpriteAnimation Fld_GetPlate_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x0,
    .posY              = 0x10,
    .frameInfoCallback = Fld_GetPlate_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 11,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 2,
};

extern void func_020265d4(void* value, s32 target, u16 step);

SpriteFrameInfo* Fld_GetPlate_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void Fld_GetPlate_Load(Sprite* sprite, Fld_GetPlate_Args* args, s16 posX) {
    SpriteAnimation animation = Fld_GetPlate_Anim;
    animation.dataType        = args->dataType;
    animation.posX            = posX;
    _Sprite_Load(sprite, &animation);
}

s32 Fld_GetPlate_Init(TaskPool* pool, Task* task, void* args) {
    Fld_GetPlate*      plate     = task->data;
    Fld_GetPlate_Args* plateArgs = args;
    s32                width;

    SysFont_InitWithFont(&plate->font, 1, TRUE);
    SysFont_SetSpacing(&plate->font, TRUE, 0);
    plate->message    = func_ov030_020c4ae8(&plate->font, 0x5DCD, (u16)plateArgs->messageIndex);
    width             = SysFont_MeasureWidth(&plate->font, plate->message);
    plate->targetPosX = (width - 0x50) << 12;
    if (plate->targetPosX >= 0x80000) {
        plate->targetPosX = 0x80000;
    }
    plate->posX        = plate->targetPosX - ((width + 0x30) << 12);
    plate->slideFrames = 0x14;
    Fld_GetPlate_Load(&plate->sprite, plateArgs, plate->posX >> 12);
    Sprite_Update(&plate->sprite);
    plate->drawDelay = 2;
    SysFont_SetHAlign(&plate->font, 1, width);
    SysFont_SetPos(&plate->font, 0x100 - (width + 0x20), 7);
    return 1;
}

s32 Fld_GetPlate_Update(TaskPool* pool, Task* task, void* args) {
    Fld_GetPlate* plate = task->data;

    if (plate->slideFrames != 0) {
        func_020265d4(&plate->posX, plate->targetPosX, plate->slideFrames);
        plate->slideFrames--;
        plate->sprite.posX = plate->posX >> 12;
    }
    return 1;
}

s32 Fld_GetPlate_Render(TaskPool* pool, Task* task, void* args) {
    Fld_GetPlate* plate = task->data;

    Sprite_RenderFrame(&plate->sprite);
    if (plate->drawDelay != 0) {
        plate->drawDelay--;
        if (plate->drawDelay == 0) {
            SysFont_DrawToSprite(&plate->font, plate->message, &plate->sprite, TRUE);
            Mem_Free(&gDebugHeap, plate->message);
            SysFont_Destroy(&plate->font);
        }
    }
    return 1;
}

s32 Fld_GetPlate_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Destroy(task->data);
    return 1;
}

s32 Fld_GetPlate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_GetPlate_Init,
        .update     = Fld_GetPlate_Update,
        .render     = Fld_GetPlate_Render,
        .cleanup    = Fld_GetPlate_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_GetPlate_CreateTask(TaskPool* pool, s32 dataType, s32 messageIndex) {
    Fld_GetPlate_Args args;
    args.dataType     = dataType;
    args.messageIndex = messageIndex;
    return EasyTask_CreateTask(pool, &Tsk_Fld_GetPlate, NULL, 0, NULL, &args);
}
