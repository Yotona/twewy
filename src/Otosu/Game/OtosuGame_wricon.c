#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ s32    dataType;
    /* 0x04 */ Sprite sprite;
    /* 0x44 */ s32    visible;
} OtosuGame_wricon; // Size: 0x48

extern s32 func_ov040_0209cb5c(void);

static s32              OtosuGame_wricon_RunTask(TaskPool* pool, Task* task, void* data, s32 stage);
static SpriteFrameInfo* OtosuGame_wricon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_wricon = {"Tsk_OtosuGame_wricon", OtosuGame_wricon_RunTask, sizeof(OtosuGame_wricon)};

static const SpriteAnimation OtosuGame_wricon_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x80,
    .posY              = 0x60,
    .frameInfoCallback = OtosuGame_wricon_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
    .unk_18            = 2,
    .packIndex         = 0xB,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 5,
};

static SpriteFrameInfo* OtosuGame_wricon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 3);
}

static void OtosuGame_wricon_Load(OtosuGame_wricon* self, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_wricon_Anim;

    anim.owner    = self;
    anim.dataType = (u16)self->dataType;
    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_wricon_Init(TaskPool* pool, Task* task, void* args) {
    OtuTaskArgs1*     taskArgs = args;
    OtosuGame_wricon* self     = task->data;

    self->visible  = 1;
    self->dataType = taskArgs->dataType;
    OtosuGame_wricon_Load(self, &self->sprite);
    return 1;
}

static s32 OtosuGame_wricon_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 OtosuGame_wricon_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_wricon* self = task->data;

    if (self->visible != 0) {
        Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 5 - func_ov040_0209cb5c(), self->sprite.cellTable);
        Sprite_Update(&self->sprite);
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_wricon_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_wricon* self = task->data;

    Sprite_Release(&self->sprite);
    return 1;
}

static s32 OtosuGame_wricon_RunTask(TaskPool* pool, Task* task, void* data, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_wricon_Init,
        .update     = OtosuGame_wricon_Update,
        .render     = OtosuGame_wricon_Render,
        .cleanup    = OtosuGame_wricon_Destroy,
    };

    return stages.iter[stage](pool, task, data);
}

s32 OtosuGame_wricon_CreateTask(TaskPool* pool, s32 dataType) {
    OtuTaskArgs1 args;

    args.dataType = dataType;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_wricon, NULL, 0, NULL, &args);
}
