#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ TaskPool*      pool;
    /* 0x04 */ Sprite         sprite;
    /* 0x44 */ OamAffineParam affine;
    /* 0x54 */ Vec2p          origin;
    /* 0x5C */ Vec2p          pos;
    /* 0x64 */ s32            height;
    /* 0x68 */ s32            pinId;
    /* 0x6C */ s32            visible; // the pin is in phase 6
    /* 0x70 */ s32            chargeFrames;
    /* 0x74 */ s32            holdFrames;
    /* 0x78 */ s32            timer;
    /* 0x7C */ s32            holdTimer;
    /* 0x80 */ s32            state;       // 1 start, 2 charge, 3 hold, 4 release
    /* 0x84 */ s32            hahenIds[8]; // needlehahen
} OtosuGame_needle;                        // Size: 0xA4

static s32              OtosuGame_needle_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_needle_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_needle = {"Tsk_OtosuGame_needle", OtosuGame_needle_RunTask, sizeof(OtosuGame_needle)};

static const SpriteAnimation OtosuGame_needle_Anim = {
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
    .frameInfoCallback = OtosuGame_needle_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 7,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 0xE,
};

static SpriteFrameInfo* OtosuGame_needle_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_needle* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affine, OtuBoard_SpriteSortKey(4, owner->pos.y, 0));
}

static void OtosuGame_needle_Load(OtosuGame_needle* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_needle_Anim;

    anim.owner    = self;
    anim.dataType = args->dataType;
    anim.posX     = F2I(self->pos.x);
    anim.posY     = F2I(self->pos.y);
    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_needle_Init(TaskPool* pool, Task* task, void* args) {
    OtuPinSpriteArgs* taskArgs = args;
    OtosuGame_needle* self     = task->data;

    self->pool            = pool;
    self->visible         = 0;
    self->pinId           = taskArgs->pinId;
    self->origin.x        = 0;
    self->origin.y        = 0;
    self->pos.x           = 0;
    self->pos.y           = 0;
    self->height          = 0;
    self->affine.rotation = 0;
    self->affine.scaleX   = 0x1000;
    self->affine.scaleY   = 0x1000;
    self->affine.unk_0C   = 0;
    self->affine.unk_0E   = 0;
    self->state           = 0;

    OtosuGame_needle_Load(self, &self->sprite, taskArgs);
    return 1;
}

/**
 * Follows the phase-6 pin through the needle's states: 1 starts the charge,
 * 2 grows the needle to full size over `chargeFrames`, 3 holds it for
 * `holdFrames`, 4 shrinks it back. The cases fall through on the frame a state
 * finishes.
 */
static s32 OtosuGame_needle_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_needle* self = task->data;

    void* pin = EasyTask_GetTaskData(pool, self->pinId);

    if (pin != NULL) {
        self->visible = OtosuGame_badge_IsUsingNeedle(pin);
        if (self->visible == 0) {
            self->state = 0;
        } else {
            OtosuGame_badge_GetOrigin(pin, &self->origin);
            OtosuGame_badge_GetPos(pin, &self->pos);
            self->height = OtosuGame_badge_GetHeight(pin);
        }

        switch (self->state) {
            case 0:
                self->visible = 0;
                break;

            case 1:
                self->affine.scaleY = 0x99A;
                self->affine.scaleX = 0x99A;
                Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 0xE, self->sprite.cellTable);
                self->timer = self->chargeFrames;
                self->state = 2;
                /* fall through */
            case 2:
                self->affine.scaleX = self->affine.scaleX + (0x1000 - self->affine.scaleX) / self->timer;
                self->affine.scaleY = self->affine.scaleX;

                self->timer = self->timer - 1;

                if (self->timer > 0) {
                    break;
                }

                self->sprite.animationMode = ANIM_MODE_ONCE;
                Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 0xF, self->sprite.cellTable);
                OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_NEEDLE01, &self->pos, &self->origin);
                self->timer     = self->holdFrames;
                self->holdTimer = 0x1E;
                self->state     = 3;
                /* fall through */
            case 3:
                if (self->holdTimer > 0) {
                    self->holdTimer = self->holdTimer - 1;
                }
                self->timer = self->timer - 1;

                if (self->timer > 0) {
                    break;
                }

                self->sprite.animationMode = ANIM_MODE_ONCE;
                Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 2, self->sprite.cellTable);
                OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_NEEDLE03, &self->pos, &self->origin);
                self->timer = 0xE;
                self->state = 4;
                /* fall through */
            case 4:
                self->affine.scaleX = self->affine.scaleX + (0x99A - self->affine.scaleX) / self->timer;
                self->affine.scaleY = self->affine.scaleX;

                self->timer = self->timer - 1;

                if (self->timer <= 0) {
                    self->state = 0;
                }
                break;
        }

        if (self->visible) {
            Sprite_Update(&self->sprite);
        }
    } else {
        self->visible = 0;
    }
    return 1;
}

static s32 OtosuGame_needle_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_needle* self = task->data;

    if (self->visible) {
        self->sprite.posX = F2I(self->pos.x - self->origin.x);
        self->sprite.posY = F2I(self->pos.y + self->height - self->origin.y);
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_needle_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_needle* self = task->data;

    Sprite_Release(&self->sprite);
    for (s32 i = 0; i < 8; i++) {
        EasyTask_DeleteTask(pool, self->hahenIds[i]);
    }
    return 1;
}

static s32 OtosuGame_needle_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_needle_Init,
        .update     = OtosuGame_needle_Update,
        .render     = OtosuGame_needle_Render,
        .cleanup    = OtosuGame_needle_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

/** Creates the needle and its eight fragment tasks. */
s32 OtosuGame_needle_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pinId;

    s32               handle = EasyTask_CreateTask(pool, &Tsk_OtosuGame_needle, NULL, 0, NULL, &args);
    OtosuGame_needle* self   = EasyTask_GetTaskData(pool, handle);

    for (s32 i = 0; i < 8; i++) {
        self->hahenIds[i] = OtosuGame_needlehahen_CreateTask(pool, dataType, pinId);
    }
    return handle;
}

/** The needle's tip while it is held, with a fixed scale of 32.0. */
s32 OtosuGame_needle_GetHitRecord(OtosuGame_needle* self, OtuPinRecord* out) {
    if (self->state != 3) {
        return 0;
    }

    s32 y = self->pos.y;
    s32 x = self->pos.x;

    out->pos.x = x;
    out->pos.y = y;
    out->scale = 0x20000;
    return 1;
}

/** Starts the needle: charge for `chargeFrames`, hold for `holdFrames`. */
void OtosuGame_needle_Start(OtosuGame_needle* self, s32 chargeFrames, s32 holdFrames) {
    self->chargeFrames = chargeFrames;
    self->holdFrames   = holdFrames;
    self->state        = 1;
}

/** While held, cuts the hold short to what is left of its first 30 frames. */
void OtosuGame_needle_EndHold(OtosuGame_needle* self) {
    if (self->state == 3) {
        self->timer = self->holdTimer;
    }
}

/** True while the needle is out. */
s32 OtosuGame_needle_IsActive(OtosuGame_needle* self) {
    return self->state != 0;
}

/** Throws the eight fragments from the needle's tip. */
void OtosuGame_needle_Shatter(OtosuGame_needle* self) {
    for (s32 i = 0; i < 8; i++) {
        OtosuGame_needlehahen_Start(EasyTask_GetTaskData(self->pool, self->hahenIds[i]), &self->pos);
    }
}
