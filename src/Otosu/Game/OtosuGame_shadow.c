#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ OamAffineParam affine;
    /* 0x50 */ Vec2p          origin;
    /* 0x58 */ Vec2p          pos;
    /* 0x60 */ s32            pinId;
    /* 0x64 */ s32            visible;
} OtosuGame_shadow; // Size: 0x68

static s32              OtosuGame_shadow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_shadow_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_shadow = {"Tsk_OtosuGame_shadow", OtosuGame_shadow_RunTask, sizeof(OtosuGame_shadow)};

static const SpriteAnimation OtosuGame_shadow_Anim = {
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
    .frameInfoCallback = OtosuGame_shadow_GetFrameInfo,
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
    .animIndex         = 1,
};

static SpriteFrameInfo* OtosuGame_shadow_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_shadow* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affine, OtuBoard_SpriteSortKey(0, owner->pos.y, 0));
}

/**
 * The only one of the four loaders that touches the OAM priority word, which is
 * what keeps the shadow behind everything else. The dataType edit has to stay
 * after both position stores.
 */
static void OtosuGame_shadow_Load(OtosuGame_shadow* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_shadow_Anim;

    anim.owner           = self;
    anim.dataType        = args->dataType;
    anim.posX            = F2I(self->pos.x);
    anim.posY            = F2I(self->pos.y);
    anim.unk_02.unk_02   = 1;
    data_0206a890.unk_0C = 0xA;

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_shadow_Init(TaskPool* pool, Task* task, void* args) {
    OtuPinSpriteArgs* taskArgs = args;
    OtosuGame_shadow* self     = task->data;

    self->pinId           = taskArgs->pinId;
    self->origin.x        = 0;
    self->origin.y        = 0;
    self->pos.x           = 0;
    self->pos.y           = 0;
    self->affine.rotation = 0;
    self->affine.scaleX   = 0x1000;
    self->affine.scaleY   = 0x1000;
    self->affine.unk_0C   = 0;
    self->affine.unk_0E   = 0;

    OtosuGame_shadow_Load(self, &self->sprite, taskArgs);
    return 1;
}

/** Follows the pin and its scale, but only while OtosuGame_badge_IsShown says exactly 1. */
static s32 OtosuGame_shadow_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_shadow* self = task->data;
    void*             pin  = EasyTask_GetTaskData(pool, self->pinId);

    if (pin != NULL) {
        self->visible = OtosuGame_badge_IsShown(pin);

        if (self->visible == 1) {
            OtosuGame_badge_GetOrigin(pin, &self->origin);
            OtosuGame_badge_GetPos(pin, &self->pos);
            self->affine.scaleY = OtosuGame_badge_GetShadowScale(pin);
            self->affine.scaleX = self->affine.scaleY;
            Sprite_Update(&self->sprite);
        }
    } else {
        self->visible = 0;
    }
    return 1;
}

/** Draws the shadow three pixels down and right of the pin. */
static s32 OtosuGame_shadow_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_shadow* self = task->data;

    if (self->visible != 0) {
        self->sprite.posX = F2I(self->pos.x - self->origin.x) + 3;
        self->sprite.posY = F2I(self->pos.y - self->origin.y) + 3;
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_shadow_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(&((OtosuGame_shadow*)task->data)->sprite);
    return 1;
}

static s32 OtosuGame_shadow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_shadow_Init,
        .update     = OtosuGame_shadow_Update,
        .render     = OtosuGame_shadow_Render,
        .cleanup    = OtosuGame_shadow_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_shadow_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pinId;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_shadow, NULL, 0, NULL, &args);
}
