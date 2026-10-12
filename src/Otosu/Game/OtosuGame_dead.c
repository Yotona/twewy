#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ Vec2p  origin;
    /* 0x48 */ Vec2p  pos;
    /* 0x50 */ s32    pinId;
    /* 0x54 */ s32    visible;
    /* 0x58 */ s32    state;
} OtosuGame_dead; // Size: 0x5C

static s32              OtosuGame_dead_RunTask(TaskPool* pool, Task* task, void* data, s32 stage);
static SpriteFrameInfo* OtosuGame_dead_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_dead = {"Tsk_OtosuGame_dead", OtosuGame_dead_RunTask, sizeof(OtosuGame_dead)};

static const SpriteAnimation OtosuGame_dead_Anim = {
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
    .frameInfoCallback = OtosuGame_dead_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 4,
};

static SpriteFrameInfo* OtosuGame_dead_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_dead* owner = sprite->owner;

    Sprite_FrameInfoCallbackSorted(sprite, mode, OtuBoard_SpriteSortKey(3, owner->pos.y, 0));
}

static void OtosuGame_dead_Load(OtosuGame_dead* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_dead_Anim;

    anim.owner    = self;
    anim.dataType = args->dataType;
    anim.posX     = F2I(self->pos.x);
    anim.posY     = F2I(self->pos.y);
    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_dead_Init(TaskPool* pool, Task* task, void* args) {
    OtuPinSpriteArgs* taskArgs = args;
    OtosuGame_dead*   self     = task->data;

    self->pinId    = taskArgs->pinId;
    self->origin.x = 0;
    self->origin.y = 0;
    self->pos.x    = 0;
    self->pos.y    = 0;
    self->visible  = FALSE;
    self->state    = 0;
    OtosuGame_dead_Load(self, &self->sprite, taskArgs);
    return 1;
}

/** Follows the pin while playing, and hides once the animation has finished. */
static s32 OtosuGame_dead_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_dead* self = task->data;
    void*           pin  = EasyTask_GetTaskData(pool, self->pinId);

    if (pin != NULL) {
        switch (self->state) {
            case 0:
                self->visible = FALSE;
                break;

            case 1:
                OtosuGame_badge_GetOrigin(pin, &self->origin);
                OtosuGame_badge_GetPos(pin, &self->pos);
                if (self->sprite.isPlaying != 1) {
                    self->visible = TRUE;
                } else {
                    self->state   = 0;
                    self->visible = FALSE;
                }
                break;
        }
    } else {
        self->visible = FALSE;
    }

    if (self->visible) {
        Sprite_Update(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_dead_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_dead* self = task->data;

    if (self->visible) {
        self->sprite.posX = F2I(self->pos.x - self->origin.x);
        self->sprite.posY = F2I(self->pos.y - self->origin.y);
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_dead_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_dead* self = task->data;

    Sprite_Release(&self->sprite);
    return 1;
}

static s32 OtosuGame_dead_RunTask(TaskPool* pool, Task* task, void* data, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_dead_Init,
        .update     = OtosuGame_dead_Update,
        .render     = OtosuGame_dead_Render,
        .cleanup    = OtosuGame_dead_Destroy,
    };

    return stages.iter[stage](pool, task, data);
}

s32 OtosuGame_dead_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pinId;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_dead, NULL, 0, NULL, &args);
}

void OtosuGame_dead_Start(OtosuGame_dead* self) {
    self->state = 1;
    Sprite_SetAnimation(&self->sprite, self->sprite.animData, 4, self->sprite.cellTable);
}
