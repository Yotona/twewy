#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ Vec2p  origin;
    /* 0x48 */ Vec2p  pos;
    /* 0x50 */ s32    pinId;
    /* 0x54 */ s32    frame;
} OtosuGame_marker; // Size: 0x58

static s32              OtosuGame_marker_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_marker_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_marker = {"Tsk_OtosuGame_marker", OtosuGame_marker_RunTask, sizeof(OtosuGame_marker)};

static const SpriteAnimation OtosuGame_marker_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = OtosuGame_marker_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 8,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 2,
};

static SpriteFrameInfo* OtosuGame_marker_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_marker* owner = sprite->owner;
    Sprite_FrameInfoCallbackSorted(sprite, mode, I2F(((owner->pos.y >> 12) & 0x7FF) + 1));
}

static void OtosuGame_marker_Load(OtosuGame_marker* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_marker_Anim;

    anim.owner    = self;
    anim.dataType = args->dataType;
    anim.posX     = F2I(self->pos.x);
    anim.posY     = F2I(self->pos.y);

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_marker_Init(TaskPool* pool, Task* task, void* args) {
    OtuPinSpriteArgs* taskArgs = args;
    OtosuGame_marker* self     = task->data;

    self->pinId    = taskArgs->pinId;
    self->origin.x = 0;
    self->origin.y = 0;
    self->pos.x    = 0;
    self->pos.y    = 0;
    self->frame    = 0;

    OtosuGame_marker_Load(self, &self->sprite, taskArgs);
    return 1;
}

static s32 OtosuGame_marker_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_marker* self = task->data;
    void*             pin  = EasyTask_GetTaskData(pool, self->pinId);

    if (pin != NULL) {
        s32 prev    = self->frame;
        s32 frame   = OtosuGame_badge_GetMeteoAimState(pin);
        self->frame = frame;

        if (frame > 0) {
            if (frame != prev) {
                Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, (s16)frame, self->sprite.cellTable);
            }
            OtosuGame_badge_GetOrigin(pin, &self->origin);
            OtosuGame_badge_GetPos(pin, &self->pos);
            Sprite_Update(&self->sprite);
        }
    } else {
        self->frame = 0;
    }
    return 1;
}

static s32 OtosuGame_marker_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_marker* self = task->data;

    if (self->frame != 0) {
        self->sprite.posX = F2I(self->pos.x - self->origin.x);
        self->sprite.posY = F2I(self->pos.y - self->origin.y);
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_marker_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(&((OtosuGame_marker*)task->data)->sprite);
    return 1;
}

static s32 OtosuGame_marker_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_marker_Init,
        .update     = OtosuGame_marker_Update,
        .render     = OtosuGame_marker_Render,
        .cleanup    = OtosuGame_marker_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_marker_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pinId;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_marker, NULL, 0, NULL, &args);
}
