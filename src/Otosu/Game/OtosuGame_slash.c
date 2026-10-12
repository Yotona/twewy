#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ OamAffineParam affine; // rotation follows the aim, scaleY its length
    /* 0x50 */ Vec2p          origin;
    /* 0x58 */ Vec2p          pos;
    /* 0x60 */ s32            pinId;
    /* 0x64 */ s32            visible;
    /* 0x68 */ s32            state; // 0 start, 1 follow, 2 settle
    /* 0x6C */ s32            timer; // settle frames left
} OtosuGame_slash;                   // Size: 0x70

static s32              OtosuGame_slash_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_slash_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_slash = {"Tsk_OtosuGame_slash", OtosuGame_slash_RunTask, sizeof(OtosuGame_slash)};

static const SpriteAnimation OtosuGame_slash_Anim = {
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
    .frameInfoCallback = OtosuGame_slash_GetFrameInfo,
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
    .animIndex         = 3,
};

static SpriteFrameInfo* OtosuGame_slash_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_slash* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affine, OtuBoard_SpriteSortKey(6, owner->pos.y, 0));
}

static void OtosuGame_slash_Load(OtosuGame_slash* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_slash_Anim;

    anim.owner    = self;
    anim.dataType = args->dataType;
    anim.posX     = self->pos.x >> 12;
    anim.posY     = self->pos.y >> 12;

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_slash_Init(TaskPool* pool, Task* task, void* args) {
    OtosuGame_slash*  self = task->data;
    OtuPinSpriteArgs* a    = args;

    self->pinId           = a->pinId;
    self->pos.x           = 0;
    self->pos.y           = 0;
    self->affine.rotation = 0;
    self->affine.scaleX   = 0x1000;
    self->affine.scaleY   = 0x1000;
    self->affine.unk_0C   = 0;
    self->affine.unk_0E   = 0;
    self->visible         = 0;
    self->timer           = 0;
    self->state           = 0;

    OtosuGame_slash_Load(self, &self->sprite, a);
    return 1;
}

/**
 * Aims the arrow from the pin along its pending shot: 0 waits for the pin to
 * start aiming, 1 follows the aim (and plays the arrow sound panned by x and
 * pitched by length once the frame finishes), 2 shrinks the arrow back over
 * `timer` frames after the pin stops. The cases fall through into each other.
 */
static s32 OtosuGame_slash_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_slash* self = task->data;
    OtosuGame_badge* pin;
    s32              len;
    s32              pan;

    pin = EasyTask_GetTaskData(pool, self->pinId);

    if (pin != NULL) {
        Vec2p vel;
        Vec2p off;
        Vec2p dir;

        s32 phase = OtosuGame_badge_IsAiming(pin);

        OtosuGame_badge_GetHomeOffset(pin, &self->origin);

        switch (self->state) {
            case 0:
                if (phase != 0) {
                    Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 2, self->sprite.cellTable);
                    self->visible = 1;
                    self->state   = 1;
                } else {
                    self->visible = 0;
                    break;
                }
                // Falls through.

            case 1:
                if (phase != 0) {
                    OtosuGame_badge_GetAimLine(pin, &vel, &off);

                    pan = off.x >> 12;

                    OtuVec_Add(&vel, &self->origin, &vel);
                    OtuVec_Add(&off, &self->origin, &off);
                    OtuVec_Sub(&off, &vel, &dir);
                    OtuVec_ScaleAdd(0x800, &dir, &vel, &self->pos);

                    len                   = OtuVec_Length(&dir);
                    self->affine.scaleY   = len / 48;
                    self->affine.rotation = (u16)(FX_Atan2Idx(dir.y, dir.x) + 0x4000);

                    if (OtosuGame_badge_IsCharged(pin) != 0) {
                        Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 3, self->sprite.cellTable);
                    }

                    if (SpriteMgr_IsFrameFinished(&self->sprite)) {
                        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_ARROW);

                        if (pan < 0) {
                            pan = 0;
                        } else if (pan > 0xFF) {
                            pan = 0xFF;
                        }

                        SndMgr_UpdateSEPan(SEIDX_SE_BAYBADGE_ARROW, pan);
                        SndMgr_SetSequenceSoundPitch(SEIDX_SE_BAYBADGE_ARROW, ((len >> 12) * 3 * 0x100) / 0x100);
                    }
                    break;
                }

                self->timer = 0x10;
                self->state = 2;
                // Falls through.

            case 2:
                if (phase == 1) {
                    self->state = 1;
                    break;
                }

                self->timer = self->timer - 1;

                if (self->timer <= 0) {
                    self->state = 0;
                } else {
                    self->affine.scaleY = self->affine.scaleY + (0x200 - self->affine.scaleY) / self->timer;
                }
                break;
        }
    } else {
        self->visible = 0;
    }

    if (self->visible != 0) {
        Sprite_Update(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_slash_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_slash* self = task->data;

    if (self->visible != 0) {
        self->sprite.posX = (self->pos.x - self->origin.x) >> 12;
        self->sprite.posY = (self->pos.y - self->origin.y) >> 12;
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_slash_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(&((OtosuGame_slash*)task->data)->sprite);
    return 1;
}

static s32 OtosuGame_slash_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_slash_Init,
        .update     = OtosuGame_slash_Update,
        .render     = OtosuGame_slash_Render,
        .cleanup    = OtosuGame_slash_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_slash_CreateTask(TaskPool* pool, s32 dataType, s32 pin) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pin;

    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_slash, NULL, 0, NULL, &args);
}
