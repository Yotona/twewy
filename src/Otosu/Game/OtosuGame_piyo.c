#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ Vec2p  origin;
    /* 0x48 */ Vec2p  pos;
    /* 0x50 */ s32    pinId;
    /* 0x54 */ s32    visible;
    /* 0x58 */ s32    stunMax; // the stun is banded as a percentage of this
    /* 0x5C */ s32    timer;   // frames until the next OtuBoard_PlaySEAt nudge
} OtosuGame_piyo;              // Size: 0x60

static s32              OtosuGame_piyo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_piyo_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_piyo = {"Tsk_OtosuGame_piyo", OtosuGame_piyo_RunTask, sizeof(OtosuGame_piyo)};

static const SpriteAnimation OtosuGame_piyo_Anim = {
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
    .frameInfoCallback = OtosuGame_piyo_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 2,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* OtosuGame_piyo_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_piyo* owner = sprite->owner;

    Sprite_FrameInfoCallbackSorted(sprite, mode, OtuBoard_SpriteSortKey(3, owner->pos.y, 0));
}

static void OtosuGame_piyo_Load(OtosuGame_piyo* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_piyo_Anim;

    anim.owner    = self;
    anim.dataType = args->dataType;
    anim.posX     = F2I(self->pos.x - self->origin.x);
    anim.posY     = F2I(self->pos.y - self->origin.y);
    anim.bits_7_9 = 5;

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_piyo_Init(TaskPool* pool, Task* task, void* args) {
    OtuPinSpriteArgs* taskArgs = args;
    OtosuGame_piyo*   self     = task->data;

    self->visible  = 0;
    self->pinId    = taskArgs->pinId;
    self->origin.x = 0;
    self->origin.y = 0;
    self->pos.x    = 0;
    self->pos.y    = 0;

    OtosuGame_piyo_Load(self, &self->sprite, taskArgs);
    return 1;
}

/**
 * Picks one of five animations from the pin's remaining stun as a percentage of
 * `lifeMax` (20/40/60/80 are the band edges), and nudges the pin every 30 frames.
 */
static s32 OtosuGame_piyo_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_piyo* self = task->data;
    void*           pin  = EasyTask_GetTaskData(pool, self->pinId);
    s16             frame;
    s32             band;
    s32             stun;

    if (pin != NULL) {
        stun = OtosuGame_badge_GetStun(pin);

        if (stun > 0) {
            OtosuGame_badge_GetOrigin(pin, &self->origin);
            OtosuGame_badge_GetPos(pin, &self->pos);

            band = stun * 100 / self->stunMax;

            if (band <= 0x14) {
                frame = 8;
            } else if (band <= 0x28) {
                frame = 7;
            } else if (band <= 0x3C) {
                frame = 6;
            } else if (band <= 0x50) {
                frame = 5;
            } else {
                frame = 4;
            }

            Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, frame, self->sprite.cellTable);
            Sprite_Update(&self->sprite);

            self->timer = self->timer - 1;
            if (self->timer <= 0) {
                OtuBoard_PlaySEAt(0x330, &self->pos, &self->origin);
                self->timer = 0x1E;
            }

            self->visible = 1;
        } else {
            self->visible = 0;
        }
    } else {
        self->visible = 0;
    }
    return 1;
}

static s32 OtosuGame_piyo_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_piyo* self = task->data;

    if (self->visible != 0) {
        self->sprite.posX = F2I(self->pos.x - self->origin.x);
        self->sprite.posY = F2I(self->pos.y - self->origin.y);
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_piyo_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(&((OtosuGame_piyo*)task->data)->sprite);
    return 1;
}

static s32 OtosuGame_piyo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_piyo_Init,
        .update     = OtosuGame_piyo_Update,
        .render     = OtosuGame_piyo_Render,
        .cleanup    = OtosuGame_piyo_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_piyo_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pinId;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_piyo, NULL, 0, NULL, &args);
}

/** Sets the stun the piyo bands against and forces a nudge on the next update. */
void OtosuGame_piyo_Start(OtosuGame_piyo* self, s32 stunMax) {
    self->stunMax = stunMax;
    self->timer   = 1;
}
